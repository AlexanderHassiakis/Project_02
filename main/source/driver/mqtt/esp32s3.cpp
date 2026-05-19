#include <cstdint>
#include <cstdio>
#include <functional> 
#include <string>     

#include "driver/mqtt/esp32s3.h"
#include "esp_log.h"
#include "mqtt_client.h"

// TAG för ESP_LOG
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

  esp_mqtt_client_config_t mqttCfg = {};
  mqttCfg.broker.address.uri =
      "mqtt://broker.hivemq.com"; 

  myClient = esp_mqtt_client_init(&mqttCfg);
  if (myClient == nullptr) {
    ESP_LOGE(TAG, "Fail to create MQTT-client!");
    return;
  }
  esp_mqtt_client_register_event(myClient, MQTT_EVENT_ANY,
                                 Esp32s3::mqttEventHandler, this);

  if (esp_mqtt_client_start(myClient) != ESP_OK) {
    ESP_LOGE(TAG, "Could not start MQTT Client!!");
  }
}

bool Esp32s3::isConnected() noexcept { return myConnectionStatus; }

void Esp32s3::send(const std::string &topic, const std::uint8_t *bytes,
                   std::uint16_t byteLen) noexcept {

  if (bytes == nullptr || myClient == nullptr || !myConnectionStatus ||
      byteLen == 0) {
    ESP_LOGE(TAG, "Could not send message to MQTT client!!");
    return;
  }

  const char *data_ptr = reinterpret_cast<const char *>(bytes);

  
  esp_mqtt_client_publish(myClient, topic.c_str(), data_ptr, byteLen, 1, 0);
}

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
    esp_mqtt_client_subscribe(event->client, "ESP32_Commands", 1);
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