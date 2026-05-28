/**
 * @file stub.h
 * @brief Stub ADC driver used for simulation and testing.
 */

#pragma once

#include <cstdint>

#include "driver/adc/interface.h"

namespace driver::adc
{
    /**
     * @brief Stub implementation of the ADC interface.
     *
     * This class simulates a single ADC reading without any hardware dependency.
     */
    class Stub final : public Interface
    {
    public:
        /**
         * @brief Construct the stub ADC driver.
         *
         * @param adcMax Maximum raw ADC value used for conversions.
         * @param supplyVoltage Reference voltage used for conversions.
         */
        explicit Stub(const std::uint16_t adcMax = DefaultAdcMax,
                      const float supplyVoltage = DefaultVoltage) noexcept
            // Check if supply voltage is >= 0.0, if so, use default value 3.3
            : mySupplyVoltage{0.0 < supplyVoltage ? supplyVoltage : DefaultVoltage}
              // Check if adcMax is 0, if so, use default value 4095.
              ,
              myAdcMax{0U != adcMax ? adcMax : DefaultAdcMax}, myRawValue{}, myInitialized{true}
        {
        }

        /**
         * @brief Check if the stub is initialized.
         *
         * @return Always `true` for the current stub implementation.
         */
        bool isInitialized() const noexcept override
        {
            return myInitialized;
        }

        /**
         * @brief Return the simulated raw ADC value.
         *
         * @param pin ADC pin to read.
         * @return Simulated raw ADC value.
         */
        std::uint16_t readRaw(std::uint8_t pin) noexcept override
        {
            // Pin number is not applicable for stubs, ignore.
            (void)(pin);
            return myRawValue;
        }

        /**
         * @brief Convert the simulated raw ADC value to volts.
         *
         * @param pin ADC pin to read.
         * @return Simulated voltage in volts.
         */
        float readVoltage(std::uint8_t pin) noexcept override
        {
            // Pin number is not applicable for stubs, ignore.
            (void)(pin);

            // Calulate raw value ration (0.0-1.0).
            const float ratio{static_cast<float>(myRawValue) / myAdcMax};

            // Voltage = raw value / ADC max * supply voltage, ensure floating-point division.
            return ratio * mySupplyVoltage;
        }

        // voltage = (raw / ADC_MAX) * supply voltage

        /**
         * @brief Set the simulated raw ADC value.
         *
         * Values above the configured ADC maximum are ignored.
         *
         * @param rawValue Simulated raw ADC value.
         */
        void simulateRaw(const std::uint16_t rawValue) noexcept
        {
            // Check given raw value, ignore if above the max limit.
            if (myAdcMax < rawValue)
            {
                return;
            }
            myRawValue = rawValue;
        }

        /**Esp32s3 construct Forbidden moves/copy. **/
        Stub(const Stub &) = delete;
        Stub(Stub &&) = delete;
        Stub &operator=(const Stub &) = delete;
        Stub &operator=(Stub &&) = delete;

    private:
        static constexpr float DefaultVoltage{3.3};
        static constexpr std::uint16_t DefaultAdcMax{4095U};

        /** Supply voltage. */
        const float mySupplyVoltage;

        /** Max ADC value (used for voltage calculations). */
        const std::uint16_t myAdcMax;

        /** Simulated raw value. */
        std::uint16_t myRawValue;

        const bool myInitialized;
    };
} // namespace driver::adc
