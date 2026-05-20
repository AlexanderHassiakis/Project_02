#include <cstdint>
#include <cstdio>
#include <cstring> // Behövs för strcpy
#include <functional>
#include <string>

#include "driver/mqtt/esp32s3.h"
#include "esp_log.h"
#include "esp_netif.h" // (ESP Network Interface)
#include "esp_wifi.h"  // Wifi ESP
#include "mqtt_client.h"
#include "nvs_flash.h" // Wi-Fi-stacken kräver NVS för att lagra kalibreringsdata


// ESP_LOG tag
static const char *TAG = "MQTT_DRV";

namespace driver::mqtt {

Esp32s3::Esp32s3() noexcept
    : myConnectionStatus(false), myClient(nullptr), myDataCallback(nullptr) {}

Esp32s3::~Esp32s3() noexcept {
  if (myClient != nullptr) {
    esp_mqtt_client_stop(myClient);
    esp_mqtt_client_destroy(myClient);
  }
}

void Esp32s3::mqttInit() noexcept {
  if (myClient != nullptr) {
    return;
  }

  // Startar Wi-Fi processen först
  initWifi();

  esp_mqtt_client_config_t mqttCfg = {};
  mqttCfg.broker.address.uri = "mqtt://broker.hivemq.com";

  myClient = esp_mqtt_client_init(&mqttCfg);
  if (myClient == nullptr) {
    ESP_LOGE(TAG, "Fail to create MQTT-client!");
    return;
  }
  esp_mqtt_client_register_event(myClient, MQTT_EVENT_ANY,
                                 Esp32s3::mqttEventHandler, this);
}

void Esp32s3::wifiEventHandler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  auto *instance = static_cast<Esp32s3 *>(arg);
  if (instance == nullptr) {
    return;
  }

  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGW(TAG, "Tappade Wi-Fi! Försöker ansluta igen...");
    instance->myConnectionStatus =
        false; // Sätt status till false om vi tappar nätet
    esp_wifi_connect();
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "Wi-Fi Anslutet! Fick IP: " IPSTR,
             IP2STR(&event->ip_info.ip));

    // ÄNDRING: Ändrat till instance->myClient och lagt till en null-check för
    // säkerhet
    if (instance->myClient != nullptr) {
      if (esp_mqtt_client_start(instance->myClient) != ESP_OK) {
        ESP_LOGE(TAG, "Could not start MQTT Client!!");
      }
    }
  }
}

void Esp32s3::initWifi() noexcept {
  // 1. Initiera NVS (Non-Volatile Storage)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    nvs_flash_init();
  }

  // 2. Initiera nätverksstacken och den centrala eventloopen
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();

  // 3. Konfigurera Wi-Fi i Station Mode (STA)
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  // 4. Registrera Wi-Fi-eventen
  // ÄNDRING: Ändrat 'nullptr' till 'this' (fjärde argumentet) för att skicka
  // med klassobjektet
  esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &Esp32s3::wifiEventHandler, this, nullptr);
  esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &Esp32s3::wifiEventHandler, this, nullptr);

  // 5. Sätt SSID och Lösenord
  wifi_config_t wifi_config = {};
  strcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), "Esp32test");
  strcpy(reinterpret_cast<char *>(wifi_config.sta.password), "hassiakis");
  wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

  // 6. Starta Wi-Fi!
  ESP_LOGI(TAG, "Startar Wi-Fi...");
  esp_wifi_start();
}

bool Esp32s3::isConnected() noexcept { return myConnectionStatus; }

void Esp32s3::send(const std::string &topic, const std::uint8_t *bytes,
                   std::uint16_t byteLen) noexcept {
  if (bytes == nullptr) { ESP_LOGE(TAG, "Send fail: bytes is nullptr"); return; }
  if (byteLen == 0) { ESP_LOGE(TAG, "Send fail: byteLen is 0"); return; }
  if (myClient == nullptr) { ESP_LOGE(TAG, "Send fail: myClient is nullptr"); return; }
  if (!myConnectionStatus) { ESP_LOGE(TAG, "Send fail: Not connected to MQTT broker yet!"); return; }

  const char *data_ptr = reinterpret_cast<const char *>(bytes);
  esp_mqtt_client_publish(myClient, topic.c_str(), data_ptr, byteLen, 1, 0);
}

// void Esp32s3::send(const std::string &topic, const std::uint8_t *bytes,
//                    std::uint16_t byteLen) noexcept {
//   if (bytes == nullptr || myClient == nullptr || !myConnectionStatus ||
//       byteLen == 0) {
//     ESP_LOGE(TAG, "Could not send message to MQTT client!!");
//     return;
//   }

//   const char *data_ptr = reinterpret_cast<const char *>(bytes);
//   esp_mqtt_client_publish(myClient, topic.c_str(), data_ptr, byteLen, 1, 0);
// }


void Esp32s3::registerCallback(
    std::function<void(const std::string &topic, const std::string &data)>
        cb) noexcept {
  myDataCallback = cb;
}

void Esp32s3::mqttEventHandler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  auto *instance = static_cast<Esp32s3 *>(handler_args);
  auto *event = static_cast<esp_mqtt_event_handle_t>(event_data);

  switch (event->event_id) {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED: Connected to Broker!");
    instance->myConnectionStatus = true;
    esp_mqtt_client_subscribe(event->client, "ESP32_Commands", 1);  // HARDCODE FOR RECIVED TOPICS!! ALLWAYS USE ESP32_Commands In topics.
    break;

  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED: Disconnected to Broker!");
    instance->myConnectionStatus = false;
    break;

  case MQTT_EVENT_DATA: {
    std::string received_topic(event->topic, event->topic_len);
    std::string received_data(event->data, event->data_len);

    if (instance->myDataCallback != nullptr) {
      instance->myDataCallback(received_topic, received_data);
    }
    break;
  }
  default:
    break;
  }
}

} // namespace driver::mqtt