
#include <stdbool.h>
#include <stddef.h>

#include "test/serial_mock.h"

static bool driver_installed = false;

// -----------------------------------------------------------------------------
esp_err_t usb_serial_jtag_driver_install(usb_serial_jtag_driver_config_t* cfg)
{
	// Check the config, return false if NULL.
    if (NULL == cfg) { return ESP_ERR_INVALID_ARG; }
	driver_installed = true;
	return ESP_OK;
}

// -----------------------------------------------------------------------------
void usb_serial_jtag_driver_uninstall()
{
	driver_installed = false;
}

// -----------------------------------------------------------------------------
bool serial_driver_installed(void) { return driver_installed; }

// -----------------------------------------------------------------------------
void usb_serial_jtag_write_bytes(buf, copyLen, pdMS_TO_TICKS(delay_ms))
{
	
	return ESP_OK;
}

void usb_serial_jtag_read_bytes(buf, bufLen, delay_zero)
{

	return ESP_OK;
}
