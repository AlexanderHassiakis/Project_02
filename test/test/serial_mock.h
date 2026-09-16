#ifndef SERIAL_MOCK_H_
#define SERIAL_MOCK_H_

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

typedef struct
{
	size_t tx_buffer_size;
	size_t rx_buffer_size;
} usb_serial_jtag_driver_config_t;

esp_err_t usb_serial_jtag_driver_install(usb_serial_jtag_driver_config_t* cfg);

void usb_serial_jtag_driver_uninstall(void);

bool serial_driver_installed(void);

void usb_serial_jtag_write_bytes(buf, copyLen, pdMS_TO_TICKS(delay_ms));

#ifdef __cplusplus
}
#endif

#endif /** SERIAL_MOCK_H_ */