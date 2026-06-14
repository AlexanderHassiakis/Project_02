/**
 * @brief ESP32-S3 MQTT driver implementation details.
 */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>

#include "driver/mqtt/esp32s3.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "mqtt_client.h"
#include "nvs_flash.h" 


//! @note Added anonymous namespace => preferred over static in .cpp files.
namespace
{
/** ESP_LOG tag */
//! @note Changed to constexpr.
//! @note I would name this constant Tag, but OK.
constexpr const char* TAG{"MQTT_DRV"};
}

namespace driver::mqtt 
{
// -----------------------------------------------------------------------------
//! @note Changed () to {} in member initializer list.
Esp32s3::Esp32s3() noexcept
    : myConnectionStatus{false}
    , myClient{nullptr}
    , myDataCallback{nullptr}
{}

// -----------------------------------------------------------------------------
Esp32s3::~Esp32s3() noexcept
{
    // Fixed: Yoda notation + bracket on next line
    if (nullptr != myClient)
    {
        esp_mqtt_client_stop(myClient);
        esp_mqtt_client_destroy(myClient);
    }
}

// -----------------------------------------------------------------------------
void Esp32s3::mqttInit() noexcept
{
    //! @note Please use Yoda notation like this.
    if (nullptr != myClient) { return; }

    // Start  Wi-Fi processes first
    initWifi();

    esp_mqtt_client_config_t mqttCfg{};
    mqttCfg.broker.address.uri = "mqtt://broker.hivemq.com";
    myClient = esp_mqtt_client_init(&mqttCfg);

    if (myClient == nullptr)
    {
        ESP_LOGE(TAG, "Fail to create MQTT-client!");
        return;
    }
    esp_mqtt_client_register_event(myClient, MQTT_EVENT_ANY, Esp32s3::mqttEventHandler, this);
}

// -----------------------------------------------------------------------------
    void Esp32s3::wifiEventHandler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data)
{
    auto *instance = static_cast<Esp32s3 *>(arg);

    //! @note Please use Yoda notation like this.
    if (nullptr == instance) { return; }

    //! @note Placed parentheses around comparisions => no relying on operation precedence,
    //!       clearer intent + no warnings abour operator precedence from code analyzers.
    if ((WIFI_EVENT == event_base) && (WIFI_EVENT_STA_START == event_id))
    {
        esp_wifi_connect();
    }
    else if ((WIFI_EVENT == event_base) && (WIFI_EVENT_STA_DISCONNECTED == event_id))
    {
        ESP_LOGW(TAG, "Lost Wi-Fi! Trying again...");
        instance->myConnectionStatus = false; // Sets status to false if we loose wifi
        esp_wifi_connect();
    } 
    else if ((IP_EVENT == event_base) && (IP_EVENT_STA_GOT_IP == event_id))
    {
        //! @note Replaced C-style cast with static_cast => safer.
        auto *const event{static_cast<ip_event_got_ip_t *>(event_data)};
        ESP_LOGI(TAG, "Wi-Fi Connected! Set IP: " IPSTR, IP2STR(&event->ip_info.ip));

        if (nullptr != instance->myClient)
        {
            //! @note Redundant if statement => you only want the else.
            //!        This could be changed so the error is printed 
            //!        if (ESP_OK != esp_mqtt_client_start(instance->myClient).
            if (ESP_OK == esp_mqtt_client_start(instance->myClient))
            {
                
            }
            else
            {
                ESP_LOGE(TAG, "Could not start MQTT Client!!");
            }
        }
    }
}

// -----------------------------------------------------------------------------
void Esp32s3::initWifi() noexcept
{
    // 1. Init NVS (Non-Volatile Storage)
    const auto ret = nvs_flash_init();

    //! @note Use Yoda notation + paranthese around conditions.
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // 2. Initialize the network stack and the central event loop
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    // 3. Configuring Wi-Fi in Station Mode 
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);


    // 4. Register the Wi-Fi event
    esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &Esp32s3::wifiEventHandler, this, nullptr);
    esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &Esp32s3::wifiEventHandler, this, nullptr);

    // 5. Set SSID and password
    //! @note Changed = {} to {}.
    wifi_config_t wifi_config{};
    
    //! @note Changed to std::scrcpy from <cstring>.
    std::strcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), "Esp32test"); 
    std::strcpy(reinterpret_cast<char *>(wifi_config.sta.password), "hassiakis");
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    // 6. Starting Wi-Fi!
    ESP_LOGI(TAG, "Starting Wi-Fi...");
    esp_wifi_start();
}

// -----------------------------------------------------------------------------
bool Esp32s3::isConnected() noexcept { return myConnectionStatus; }

// -----------------------------------------------------------------------------
void Esp32s3::send(const std::string& topic, const std::uint8_t* bytes,
                   std::uint16_t byteLen) noexcept
{
    //! @note Yoda, please.
    if (bytes == nullptr) { ESP_LOGE(TAG, "Send fail: bytes is nullptr"); return; }
    if (byteLen == 0) { ESP_LOGE(TAG, "Send fail: byteLen is 0"); return; }
    if (myClient == nullptr) { ESP_LOGE(TAG, "Send fail: myClient is nullptr"); return; }
    
    if (!myConnectionStatus)
    { 
        ESP_LOGE(TAG, "Send fail: Not connected to MQTT broker yet!"); 
        return; 
    }

    //! @note Feel free to use auto here, i.e. const auto data_ptr = ...
    //! @note Please use camelCase for variables, for instance dataPtr.
    const char *data_ptr = reinterpret_cast<const char *>(bytes);
    esp_mqtt_client_publish(myClient, topic.c_str(), data_ptr, byteLen, 1, 0);
}

// -----------------------------------------------------------------------------
void Esp32s3::registerCallback(Callback cb) noexcept { myDataCallback = cb; }

// -----------------------------------------------------------------------------
//! @note Use camelCase for input arguments as well.
void Esp32s3::mqttEventHandler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
    auto *instance = static_cast<Esp32s3 *>(handler_args);
    auto *event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (event->event_id) 
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED: Connected to Broker!");
        instance->myConnectionStatus = true;
        esp_mqtt_client_subscribe(event->client, "ESP32_Commands", 1);  // HARDCODE FOR RECIVED TOPICS!! ALLWAYS USE ESP32_Commands In topics.
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED: Disconnected to Broker!");
        instance->myConnectionStatus = false;
        break;

    case MQTT_EVENT_DATA:
    {
        std::string received_topic(event->topic, event->topic_len);
        std::string received_data(event->data, event->data_len);

        //! @note Yoda :)
        if (instance->myDataCallback != nullptr) 
        {
            instance->myDataCallback(received_topic, received_data);
        }
        break;
    }
    default:
        break;
    }
}
} // namespace driver::mqtt
