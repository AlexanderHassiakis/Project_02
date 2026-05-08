
#include <cstdint>

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#include "driver/tempsensor/tmp36.h"
#include "driver/adc/esp32s3.h"



namespace driver::tempsensor {

 Tmp36::Tmp36(driver::adc::Interface& adc, uint8_t channel) noexcept 
        :refBorrowAdc(adc), channelAdc(channel)
    {
      ESP_LOGI("TMP36", "ADC-resurser Skapade.");
    }

    Tmp36::~Tmp36() = default;

    int Tmp36::readTemperature() noexcept {
    int sum {0};
    int samples{8};

        for (uint8_t i = 0; i < samples; ++i) {

            sum += refBorrowAdc.readRaw(channelAdc);
        }

    int avg_raw = sum >> 3; // Bit 

    // calc millivolt (3300mV as referens)
    int voltage_mv = (avg_raw * 3300) / 4095;

    ESP_LOGI("TMP36", "Sensor avläst %i.", voltage_mv);

    // Return only whole numbers.
    return ((voltage_mv - 500)/10);
    }

} // namespace driver::tempsensor