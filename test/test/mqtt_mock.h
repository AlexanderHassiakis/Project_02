#ifndef MQTT_MOCK_H_
#define MQTT_MOCK_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Flytta denna till din hw_platform.h-fil senare, kolla vad vi gjorde i L06.
typedef int esp_err_t;

/** Error code indicating success. */
#define ESP_OK 0

/** Error code indicating invalid argument. */
#define ESP_ERR_INVALID_ARG -1
    // Denna makron till hw_platform.h

// ---------------------------------------------
//MQTT 
typedef struct
{
} esp_mqtt_client_handle_t;


typedef struct
{
} esp_mqtt_client_config_t;



void esp_mqtt_client_stop(void);
void esp_mqtt_client_destroy(void);
void esp_mqtt_client_init();
void esp_mqtt_client_register_event();
// ---------------------------------------------
// WIFI
void esp_wifi_set_mode(void);
void esp_wifi_set_config(void);
void esp_netif_init();
void esp_event_loop_create_default();
void esp_netif_create_default_wifi_sta();
// ---------------------------------------------
#ifdef __cplusplus
}
#endif

#endif /** MQTT_MOCK_H_ */