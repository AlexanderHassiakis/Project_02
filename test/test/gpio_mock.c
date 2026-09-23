#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "test/hw_test/test/hw_platform.h"
#include "test/gpio_mock.h"

/** Maximum valid pin number for ESP32-S3. */
#define PIN_MAX 48U

/** Pin registry (1 = enabled, 0 = disabled). */
static uint64_t pin_reg = 0U;

/** GPIO direction for each pin. */
static int pin_direction[PIN_MAX + 1U];

/** GPIO pull mode for each pin. */
static int pin_pull_mode[PIN_MAX + 1U];

/** Last GPIO configuration passed to gpio_config(). */
static gpio_config_t last_config;

// -----------------------------------------------------------------------------
static inline bool is_pin_valid(const gpio_num_t pin)
{
    return PIN_MAX >= pin;
}

// -----------------------------------------------------------------------------
esp_err_t gpio_set_level(const gpio_num_t pin, const uint32_t level)
{
    if (!is_pin_valid(pin)) { return ESP_ERR_INVALID_ARG; }

    if (0U != level)
    {
        pin_reg |= (1ULL << pin);
    }
    else
    {
        pin_reg &= ~(1ULL << pin);
    }

    return ESP_OK;
}

// -----------------------------------------------------------------------------
int gpio_get_level(const gpio_num_t pin)
{
    if (!is_pin_valid(pin)) { return ESP_ERR_INVALID_ARG; }
    return (pin_reg & (1ULL << pin)) ? 1 : 0;
}

// -----------------------------------------------------------------------------
esp_err_t gpio_set_direction(const gpio_num_t pin, const int mode)
{
    return ESP_OK;
}

// -----------------------------------------------------------------------------
esp_err_t gpio_set_pull_mode(const gpio_num_t pin, const int pull_mode)
{
    return ESP_OK;
}
