//! @note File header missing.
#include <cstdint>

//! @note Sort headers.
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#include "driver/tempsensor/tmp36.h"
#include "driver/adc/esp32s3.h"

namespace driver::tempsensor 
{
//! @note Changed () to {} in initializer list.
// -----------------------------------------------------------------------------
Tmp36::Tmp36(driver::adc::Interface& adc, std::uint8_t channel) noexcept
    : refBorrowAdc{adc}
    , channelAdc{channel}
    , init{false}
{
    ESP_LOGI("TMP36", "ADC-resurces created.");
}

// -----------------------------------------------------------------------------
int Tmp36::readTemperature() noexcept 
{
    int sum{};
    
    // Changed to constexpr => no memory allocation.
    constexpr std::uint8_t samples{8U};

    for (std::uint8_t i{}; i < samples; ++i) 
    {
        sum += refBorrowAdc.readRaw(channelAdc);
    }

    // Calc millivolt (3300 mV reference).
    //! @note Avoid magic numbers such as 3300, 500 and 4095, use constexpr.
    const int avgRaw{sum / Samples};
    const int voltageMv{(avgRaw * 3300) / 4095};
    ESP_LOGI("TMP36", "Sensor read %i.", voltageMv);
    return (voltageMv - 500);
}

// -----------------------------------------------------------------------------
void Tmp36::initTemp() noexcept { init = true; }

} // namespace driver::tempsensor
