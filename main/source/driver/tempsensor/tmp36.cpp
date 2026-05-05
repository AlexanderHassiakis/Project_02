
#include <cstdint>

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#include "driver/tempsensor/tmp36.h"

/**
 * @brief Guide
 *
 * ADC_CHANNEL_0 = GPIO 1
 * ADC_CHANNEL_1 = GPIO 2
 * ADC_CHANNEL_2 = GPIO 3
 * ADC_CHANNEL_3 = GPIO 4
 *
 */

namespace driver::tempsensor {

    Tmp36::Tmp36(adc_channel_t channel) noexcept
        : m_channel(channel), m_adc_handle(nullptr) 
    {
        initAdc();
        ESP_LOGI("TMP36", "ADC-resurser Skapade.");
    }

    Tmp36::~Tmp36() {
    if (m_adc_handle != nullptr) 
    {
        adc_oneshot_del_unit(m_adc_handle);
        ESP_LOGI("TMP36", "ADC-resurser frigjorda.");
    }
    } 

    int Tmp36::readTemperature() noexcept {
    int avg_raw = 0;
    int sum = 0;

    for (uint8_t i = 0; i < 8; ++i) {
        int val = 0;
        // Glöm inte: lägg till '&' före val
        adc_oneshot_read(m_adc_handle, m_channel, &val);
        sum += val;
    }

    avg_raw = sum >> 3; // Bit 

    // Beräkna millivolt (3300mV som referens)
    int voltage_mv = (avg_raw * 3300) / 4095;

    ESP_LOGI("TMP36", "Sensor avläst %i.", voltage_mv);

    // Returnerar tiondels grader (t.ex. 225 för 22.5°C)
    return (voltage_mv - 500);
  
    }

    void Tmp36::initAdc() noexcept {
        adc_oneshot_unit_init_cfg_t init_config = {};
        init_config.unit_id = ADC_UNIT_1;  //används oftast för GPIO 1-10.
        init_config.ulp_mode = ADC_ULP_MODE_DISABLE;

        esp_err_t err = adc_oneshot_new_unit(&init_config, &m_adc_handle); 
        if (err != ESP_OK) {
            ESP_LOGE("TMP36", "Misslyckades att initiera ADC: %s", esp_err_to_name(err));
            return;
        }

        adc_oneshot_chan_cfg_t config = {};
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten = ADC_ATTEN_DB_12; // Dämpar signalen så vi kan mäta upp till ca 3.3V

        err = adc_oneshot_config_channel(m_adc_handle, m_channel, &config);
        if (err != ESP_OK) {
            ESP_LOGE("TMP36", "Misslyckades att konfigurera ADC-kanal: %s", esp_err_to_name(err));
        } 

    }

} // namespace driver::tempsensor