/**
 * @brief ESP32-S3 ADC implementation details.
 */

//! @note Included <cstdint> directly.
#include <cstdint>

//! @note Sorted headers.
#include "driver/adc/esp32s3.h"
#include "driver/adc/interface.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

namespace driver::adc
{
namespace
{
/** Reference voltage (ESP32 typically 3.3V).  */
constexpr float SupplyVoltage{3.3f};

/** 12-bit ADC → range 0–4095. */
constexpr std::uint16_t AdcMax{4095U};

/** ADC hardware handle. */
adc_oneshot_unit_handle_t myHandle{};

/** Per-channel configuration. */
adc_oneshot_chan_cfg_t myConfig{};

// -----------------------------------------------------------------------------
bool init() noexcept
{
    adc_oneshot_unit_init_cfg_t init_config{};

    // Select ADC unit (ADC1 is typically used for GPIOs)
    init_config.unit_id = ADC_UNIT_1;

    // Disable ultra-low-power mode (normal operation)
    init_config.ulp_mode = ADC_ULP_MODE_DISABLE;

    // Create a new ADC unit and store handle
    esp_err_t err = adc_oneshot_new_unit(&init_config, &myHandle); 

    // If initialization fails, log error and return false
    if (err != ESP_OK) 
    {
        ESP_LOGI("ESP32-S3 ADC driver", "failed to initiate ADC: %s", esp_err_to_name(err));
        return false;
    }

    // Configure default ADC resolution (usually 12-bit)
    myConfig.bitwidth = ADC_BITWIDTH_DEFAULT;

    // Set attenuation: 12 dB allows measuring voltages up to ~3.3V safely
    myConfig.atten = ADC_ATTEN_DB_12;
    return true;
}
} // namespace

// -----------------------------------------------------------------------------
Esp32s3::Esp32s3() noexcept
    : myInitialized{init()}
{}

// -----------------------------------------------------------------------------
Esp32s3::~Esp32s3() noexcept { adc_oneshot_del_unit(myHandle); }

// -----------------------------------------------------------------------------
bool Esp32s3::isInitialized() const noexcept { return myInitialized; }

// -----------------------------------------------------------------------------
std::uint16_t Esp32s3::readRaw(std::uint8_t pin) noexcept
{
    // Convert GPIO pin to ADC channel
    adc_channel_t channel{static_cast<adc_channel_t>(pin)};

    // Configure the ADC channel before reading, return 0 on failure.
    const int err{adc_oneshot_config_channel(myHandle, channel, &myConfig)};
    if (err != ESP_OK) 
    {
        ESP_LOGE("ESP32-S3 ADC driver", "Failed to configure ADC-channel: %s", esp_err_to_name(err));
        return 0U;
    } 

    // Perform ADC read, ronvert to uint16_t (safe for 12-bit range)
    int val{};
    adc_oneshot_read(myHandle, channel, &val);
    return static_cast<std::uint16_t>(val);
}

// -----------------------------------------------------------------------------
float Esp32s3::readVoltage(std::uint8_t pin) noexcept
{
    // voltage = (raw / max_adc_value) * supply_voltage
    return readRaw(pin) / static_cast<float>(AdcMax) * SupplyVoltage;
}
} // namespace driver::adc
