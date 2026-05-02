#include "driver/tempsensor/tmp36.h"
#include "driver/sensor/interface.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include <cstdint>


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
} // Lade till den saknade måsvingen här

int Tmp36::readTemperature() noexcept {
  int avg_raw = 0;
  int sum = 0;

  for (uint8_t i = 0; i < 8; ++i) {
    int val = 0;
    // Viktigt: lägg till '&' före val
    adc_oneshot_read(m_adc_handle, m_channel, &val);
    sum += val;
  }

  avg_raw = sum >> 3;

  // Beräkna millivolt (3300mV som referens)
  int voltage_mv = (avg_raw * 3300) / 4095;

  ESP_LOGI("TMP36", "Sensor avläst %i.", voltage_mv);

  // Returnerar tiondels grader (t.ex. 225 för 22.5°C)
  return (voltage_mv - 500);
  
}

} // namespace driver::tempsensor