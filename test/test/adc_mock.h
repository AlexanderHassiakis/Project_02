/**
 * @file ESP32 ADC driver mock.
 */
#ifndef ESP_ADC_MOCK_H_
#define ESP_ADC_MOCK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/** Error code indicating success. */
#define ESP_OK 0

/** Error code indicating invalid argument. */
#define ESP_ERR_INVALID_ARG -1

/** Alias for ESP error codes. */
typedef int esp_err_t;

/** ADC unit type. */
typedef int adc_unit_t;

/** ADC channel type. */
typedef int adc_channel_t;

/** ADC bit width type. */
typedef int adc_bitwidth_t;

/** ADC attenuation type. */
typedef int adc_atten_t;

/** ADC ULP mode type. */
typedef int adc_ulp_mode_t;

/** Values used by the real ESP-IDF driver. */
#define ADC_UNIT_1 1
#define ADC_ULP_MODE_DISABLE 0
#define ADC_BITWIDTH_DEFAULT 12
#define ADC_ATTEN_DB_12 12

/** Fake ADC unit handle. */
typedef void* adc_oneshot_unit_handle_t;

/**
 * @brief ADC unit initialization configuration.
 */
typedef struct
{
    adc_unit_t unit_id;
    adc_ulp_mode_t ulp_mode;
} adc_oneshot_unit_init_cfg_t;

/**
 * @brief ADC channel configuration.
 */
typedef struct
{
    adc_bitwidth_t bitwidth;
    adc_atten_t atten;
}   adc_oneshot_chan_cfg_t;

/**
 * @brief Create an ADC unit.
 */
esp_err_t adc_oneshot_new_unit(
    const adc_oneshot_unit_init_cfg_t* init_config,
    adc_oneshot_unit_handle_t* handle);

/**
 * @brief Delete an ADC unit.
 */
esp_err_t adc_oneshot_del_unit(
    adc_oneshot_unit_handle_t handle);

/**
 * @brief Configure an ADC channel.
 */
esp_err_t adc_oneshot_config_channel(
    adc_oneshot_unit_handle_t handle,
    adc_channel_t channel,
    const adc_oneshot_chan_cfg_t* config);

/**
 * @brief Read a raw ADC value.
 */
esp_err_t adc_oneshot_read(
    adc_oneshot_unit_handle_t handle,
    adc_channel_t channel,
    int* out_raw);

/**
 * @brief Set the value that adc_oneshot_read() should return.
 *
 * @param value Fake raw ADC value.
 */
void adc_mock_set_raw(int value);

/**
 * @brief Get the last configured ADC channel.
 */
adc_channel_t adc_mock_last_channel(void);

/**
 * @brief Get the last ADC channel configuration.
 */
const adc_oneshot_chan_cfg_t* adc_mock_last_config(void);

#ifdef __cplusplus
}
#endif

#endif /** ESP_ADC_MOCK_H_ */