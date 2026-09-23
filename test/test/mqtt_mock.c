#include <stdbool.h>
#include <stddef.h>

#include "test/mqtt_mock.h"

// MQTT
// ---------------------------------------------

void esp_mqtt_client_stop(void)
{
 return ESP_OK;
}
void esp_mqtt_client_destroy(void)
{
	 return ESP_OK;
}

void esp_mqtt_client_init() 
{

	return "MOCK" , ESP_OK;
}
    void
    esp_mqtt_client_register_event()
{
		 return ESP_OK;
}

// ---------------------------------------------

// ---------------------------------------------
// WIFI
void esp_wifi_set_mode(void)
{
	 return ESP_OK;
}
void esp_wifi_set_config(void)
{
	 return ESP_OK;
}
void esp_netif_init()
{
	 return ESP_OK;
}
void esp_event_loop_create_default()
{
 return ESP_OK;
}
void esp_netif_create_default_wifi_sta()
{
	 return ESP_OK;
}
// ---------------------------------------------