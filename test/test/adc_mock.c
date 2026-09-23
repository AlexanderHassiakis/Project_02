/**
 * @file ESP32 ADC driver mock implementation details.
 */

#include <stddef.h>

#include "test/adc_mock.h"

/** Fake raw ADC value returned by adc_oneshot_read(). */
static uint32_t raw_value = 0;

/** Last ADC channel configured/read. */
static adc_channel_t last_channel = 0;

/** Last ADC channel configuration. */
static adc_oneshot_chan_cfg_t last_config;

/** Fake ADC handle storage. */
static uint16_t fake_handle = 0;

// -----------------------------------------------------------------------------
esp_err_t adc_oneshot_new_unit(
    const adc_oneshot_unit_init_cfg_t* init_config,
    adc_oneshot_unit_handle_t* handle)
{
    if ((NULL == init_config) || (NULL == handle))
    {
        return ESP_ERR_INVALID_ARG;
    }

    *handle = &fake_handle;

    return ESP_OK;
}

// -----------------------------------------------------------------------------
esp_err_t adc_oneshot_del_unit(adc_oneshot_unit_handle_t handle)
{
    if (NULL == handle)
    {
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

// -----------------------------------------------------------------------------
esp_err_t adc_oneshot_config_channel(
    adc_oneshot_unit_handle_t handle,
    const adc_channel_t channel,
    const adc_oneshot_chan_cfg_t* config)
{
    if ((NULL == handle) || (NULL == config))
    {
        return ESP_ERR_INVALID_ARG;
    }

    last_channel = channel;
    last_config = *config;

    return ESP_OK;
}

// -----------------------------------------------------------------------------
esp_err_t adc_oneshot_read(
    adc_oneshot_unit_handle_t handle,
    const adc_channel_t channel,
    int* out_raw)
{
    if ((NULL == handle) || (NULL == out_raw))
    {
        return ESP_ERR_INVALID_ARG;
    }

    last_channel = channel;
    *out_raw = raw_value;

    return ESP_OK;
}

// -----------------------------------------------------------------------------
void adc_mock_set_raw(const int value)
{
    raw_value = value;
}

// -----------------------------------------------------------------------------
adc_channel_t adc_mock_last_channel(void)
{
    return last_channel;
}

// -----------------------------------------------------------------------------
const adc_oneshot_chan_cfg_t* adc_mock_last_config(void)
{
    return &last_config;
}