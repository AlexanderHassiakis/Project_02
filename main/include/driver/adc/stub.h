#pragma once

#include <cstdint>

#include "driver/adc/interface.h"

namespace driver::adc
{
class Stub final : public Interface 
{
public:
    explicit Stub(const std::uint16_t adcMax = DefaultAdcMax,
                  const float supplyVoltage = DefaultVoltage) noexcept
        // Check if supply voltage is >= 0.0, if so, use default value 3.3
        : mySupplyVoltage{0.0 < supplyVoltage ? supplyVoltage : DefaultVoltage}
        // Check if adcMax is 0, if so, use default value 4095.
        , myAdcMax{0U != adcMax ? adcMax : DefaultAdcMax}
        , myRawValue{}
        , myInitialized{true}
    {}

     bool isInitialized() const noexcept override
     {
        return myInitialized;
     }

     std::uint16_t readRaw(std::uint8_t pin) noexcept override
     {
        // Pin number is not applicable for stubs, ignore.
        (void) (pin);
        return myRawValue;
     }

     float readVoltage(std::uint8_t pin) noexcept override
     {
        // Pin number is not applicable for stubs, ignore.
        (void) (pin);

        // Calulate raw value ration (0.0-1.0).
        const float ratio{static_cast<float>(myRawValue) / myAdcMax};

        // Voltage = raw value / ADC max * supply voltage, ensure floating-point division.
        return ratio * mySupplyVoltage;
     }

     // voltage = (raw / ADC_MAX) * supply voltage

    void simulateRaw(const std::uint16_t rawValue) noexcept 
    { 
        // Check given raw value, ignore if above the max limit.
        if (myAdcMax < rawValue) { return; }
        myRawValue = rawValue; 
    }

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


