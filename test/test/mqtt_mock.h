// mqtt_mock.h

#ifndef MQTT_MOCK_H_
#define MQTT_MOCK_H_

#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

    using esp_err_t = int;

#define ESP_OK 0

    // ---------------- MQTT ----------------

    struct esp_mqtt_client
    {
    };

    using esp_mqtt_client_handle_t = esp_mqtt_client*;

    struct esp_mqtt_client_config_t
    {
        struct
        {
            struct
            {
                const char* uri;
            } address;
        } broker;
    };

    esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t* config);

    esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client);
    esp_err_t esp_mqtt_client_stop(esp_mqtt_client_handle_t client);
    esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client);

    int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char* topic,
                                const char* data, int len, int qos, int retain);

    int esp_mqtt_client_subscribe(esp_mqtt_client_handle_t client, const char* topic, int qos);

    // ---------------- WiFi ----------------

    esp_err_t esp_wifi_connect(void);
    esp_err_t esp_wifi_start(void);

#ifdef __cplusplus
}
#endif

#endif