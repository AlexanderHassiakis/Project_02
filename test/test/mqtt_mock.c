#include "test/mqtt_mock.h"

static esp_mqtt_client mockClient;

extern "C"
{
//--------------------MQTT---------------------------------------------------------------------------

    esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t*)
    { return &mockClient; }

    esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t) { return ESP_OK; }

    esp_err_t esp_mqtt_client_stop(esp_mqtt_client_handle_t) { return ESP_OK; }

    esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t) { return ESP_OK; }

    int esp_mqtt_client_publish(esp_mqtt_client_handle_t, const char*, const char*, int, int, int)
    { return 1; }

    int esp_mqtt_client_subscribe(esp_mqtt_client_handle_t, const char*, int) { return 1; }


//--------------------WIFI---------------------------------------------------------------------------

    esp_err_t esp_wifi_connect(void) { return ESP_OK; }

    esp_err_t esp_wifi_start(void) { return ESP_OK; }
}