#include <stdbool.h>
#include <stddef.h>

#include "test/mqtt_mock.h"

// MQTT
// ---------------------------------------------

void esp_mqtt_client_stop(void);
void esp_mqtt_client_destroy(void);

// ---------------------------------------------

// ---------------------------------------------
// WIFI
void esp_wifi_set_mode(void);
void esp_wifi_set_config(void);
void esp_netif_init();
void esp_event_loop_create_default();
void esp_netif_create_default_wifi_sta();
// ---------------------------------------------