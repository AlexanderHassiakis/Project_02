#pragma once
#include "driver/sensor/interface.h"
#include "esp_adc/adc_oneshot.h"
#include <cstdint>


namespace driver::tempsensor {

class Tmp36 final : public Interface {
public:
  /**
   * @brief Construct a new Tmp36 object
   * @param channel ADC-kanalen (t.ex. ADC_CHANNEL_0 för GPIO 1)
   */
  explicit Tmp36(
      adc_channel_t channel) noexcept; // Tog bort override, lade till parameter

  /**
   * @brief Destroy the Tmp36 object
   */
  ~Tmp36() noexcept override;

  /**
   * @brief Reads temperature in deci-Celsius (225 = 22.5C)
   * @return int
   */
  int readTemperature() noexcept override;

private:
  adc_channel_t m_channel;
  adc_oneshot_unit_handle_t m_adc_handle;

  /**
   * @brief Initierar ADC-hårdvaran
   */
  void initAdc() noexcept;
};

} // namespace driver::tempsensor