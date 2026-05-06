#pragma once 

#include "driver/adc/interface.h"

#include "esp_adc/adc_oneshot.h"

namespace driver::adc
{
namespace
{
constexpr float SupplyVoltage{3.3f};
constexpr std::uint16_t AdcMax{4095U};

adc_oneshot_unit_handle_t myHandle{};
adc_oneshot_chan_cfg_t myConfig{};

bool init() noexcept {
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;  //används oftast för GPIO 1-10.
    init_config.ulp_mode = ADC_ULP_MODE_DISABLE;

    esp_err_t err = adc_oneshot_new_unit(&init_config, &myHandle); 

    if (err != ESP_OK) {
        ESP_LOGE("ESP32-S3 ADC driver", "Misslyckades att initiera ADC: %s", esp_err_to_name(err));
        return false;
    }

    myConfig.bitwidth = ADC_BITWIDTH_DEFAULT;
    myConfig.atten = ADC_ATTEN_DB_12; // Dämpar signalen så vi kan mäta upp till ca 3.3V
    return true;
}
} // namespace

Esp32s3::Esp32s3() noexcept
    : myInitialized{init()}
{}

Esp32s3::~Esp32s3() noexcept 
{
    // Eventuellt kan vi nollställa pinnen.
}

bool Esp32s3::isInitialized() const noexcept{ return myInitialized; }

std::uint16_t Esp32s3::readRaw(std::uint8_t pin) noexcept
{
    // Convert pin to ADC channel.
    adc_channel_t channel{static_cast<adc_channel_t>(pin)};

    // Configure ADC channel, return 0 on failure.
    const int err{adc_oneshot_config_channel(myHandle, channel, &myConfig)};
    if (err != ESP_OK) {
        ESP_LOGE("ESP32-S3 ADC driver", "Misslyckades att konfigurera ADC-kanal: %s", esp_err_to_name(err));
        return 0U;
    } 

    // Read value, convert value to uint16_t.
    int val{};
    adc_oneshot_read(myAdcHandle, channel, &val);
    return static_cast<std::uint16_t>(val);
}

float Esp32s3::readVoltage(std::uint8_t pin) noexcept
{
    // Voltage = raw / ADC_MAX * VCC.
    return readRaw(pin) / static_cast<float>(AdcMax) * SupplyVoltage;
}
} // driver::adc
