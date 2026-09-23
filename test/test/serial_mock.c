#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "test/serial_mock.h"

static bool driver_installed = false;

esp_err_t usb_serial_jtag_driver_install(usb_serial_jtag_driver_config_t* cfg)
{
    if (cfg == NULL) { return ESP_ERR_INVALID_ARG; }

    driver_installed = true;

    return ESP_OK;
}

esp_err_t usb_serial_jtag_driver_uninstall(void)
{
    driver_installed = false;

    return ESP_OK;
}

bool serial_driver_installed(void) { return driver_installed; }

int usb_serial_jtag_write_bytes(const void* src, size_t size, uint32_t ticks_to_wait)
{
    (void)ticks_to_wait;

    if (src == NULL) { return 0; }

    return (int)size;
}

int usb_serial_jtag_read_bytes(void* buf, uint32_t length, uint32_t ticks_to_wait)
{
    (void)buf;
    (void)length;
    (void)ticks_to_wait;

    return 0;
}