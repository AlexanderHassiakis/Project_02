#pragma once

#include <cstdint>
#include "driver/adc/interface.h"
#include "driver/tempsensor/interface.h"





namespace driver::tempsensor {

class Tmp36 final : public Interface {
    public:
        /**
         * @brief Construct a new Tmp36 object
         * @param channel ADC-kanalen (t.ex. ADC_CHANNEL_0 för GPIO 1)
         */
      explicit Tmp36(driver::adc::Interface &adc, uint8_t channelAdc) noexcept;

      /**
       * @brief Destroy the Tmp36 object
       */
      ~Tmp36() noexcept override = default;

      /**
       * @brief Reads temperature in deci-Celsius (225 = 22.5C)
       * @return int
       */
      int readTemperature() noexcept override;

    private:
      driver::adc::Interface& refBorrowAdc;
      uint8_t channelAdc;
    };

} // namespace driver::tempsensor