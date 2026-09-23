#pragma once

// Use mocked headers when testing (when the TESTSUITE macro is defined).
#ifdef TESTSUITE

/** Error code indicating success. */
#define ESP_OK 0

/** Error code indicating invalid argument. */
#define ESP_ERR_INVALID_ARG -1

/** Alias for ESP error codes. */
typedef int esp_err_t;

#include "test/gpio_mock.h"

/** Else include real ESP32 headers. */
#else

#include "driver/gpio.h"

#endif /** TESTSUITE */