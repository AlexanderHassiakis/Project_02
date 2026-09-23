#ifndef SERIAL_MOCK_H_
#define SERIAL_MOCK_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef int esp_err_t;

#define ESP_OK 0
#define ESP_ERR_INVALID_ARG -1

#ifndef pdMS_TO_TICKS
#define pdMS_TO_TICKS(ms) (ms)
#endif

    typedef struct
    {
        uint32_t tx_buffer_size;
        uint32_t rx_buffer_size;
        int intr_priority;
    } usb_serial_jtag_driver_config_t;

    esp_err_t usb_serial_jtag_driver_install(usb_serial_jtag_driver_config_t* cfg);

    esp_err_t usb_serial_jtag_driver_uninstall(void);

    int usb_serial_jtag_write_bytes(const void* src, size_t size, uint32_t ticks_to_wait);

    int usb_serial_jtag_read_bytes(void* buf, uint32_t length, uint32_t ticks_to_wait);

    bool serial_driver_installed(void);

#ifdef __cplusplus
}
#endif

#endif