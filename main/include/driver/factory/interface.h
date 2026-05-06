#pragma once

#include <memory>
#include <cstdint>

#include "driver/adc/interface.h"
#include "driver/gpio/interface.h"
#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"

namespace include::driver::factory
{
    class Interface
    {
    public:
        /**
         * @brief Destructor.
         */
        virtual ~Interface() noexcept = default;

        /**
         * @brief Create ADC driver.
         */
        virtual std::unique_ptr<adc::Interface> adc(std::uint8_t pin) noexcept = 0;

        /**
         * @brief Create GPIO driver.
         */
        virtual std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept = 0;

        /**
         * @brief Create Serial driver.
         */
        virtual std::unique_ptr<serial::Interface> serial() noexcept = 0;

        /**
         * @brief Create Timer driver.
         */
        virtual std::unique_ptr<timer::Interface> timer() noexcept = 0;

        /**
         * @brief Watchdog func
         * 
         * @param ms 
         * @return std::unique_ptr<watchdog::Interface> 
         */
        virtual std::unique_ptr<watchdog::Interface> delay_ms(std::uint16_t ms) noexcept = 0;

            /**
             * @brief Create temperature sensor.
             *
             * @param pin ADC pin
             * @param adc ADC reference
             */
            virtual std::unique_ptr<tempsensor::Interface>
            tempSensor(std::uint8_t pin, adc::Interface &adc) noexcept = 0;
    };
} // namespace include::driver::factory