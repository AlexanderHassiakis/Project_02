/**
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
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param adcMax Maximum raw ADC value used for conversions.
     * @param supplyVoltage Reference voltage used for conversions.
     */
    explicit Stub(const std::uint16_t adcMax = DefaultAdcMax,
                  const float supplyVoltage = DefaultVoltage) noexcept
        : mySupplyVoltage{0.0 < supplyVoltage ? supplyVoltage : DefaultVoltage}
        , myAdcMax{0U != adcMax ? adcMax : DefaultAdcMax}
        , myRawValue{}
    {}

    /**
     * @brief Check if the stub is initialized.
     *
     * @return Always `true` for the current stub implementation.
     */
    bool isInitialized() const noexcept override
    {
        //! @note Removed redundant member variable 'myInitialized'.
        return true;
    }

    /**
     * @brief Return the simulated raw ADC value.
     *
     * @param pin ADC pin to read.
     * 
     * @return Simulated raw ADC value.
     */
    std::uint16_t readRaw(const std::uint8_t pin) noexcept override
    {
        // Pin number is not applicable for stubs, ignore.
        (void) (pin);
        return myRawValue;
    }

    /**
     * @brief Convert the simulated raw ADC value to volts.
     *
     * @param pin ADC pin to read.
     * 
     * @return Simulated voltage in volts.
     */
    float readVoltage(const std::uint8_t pin) noexcept override
    {
        // Pin number is not applicable for stubs, ignore.
        (void)(pin);

        const float ratio{static_cast<float>(myRawValue) / myAdcMax};
        return ratio * mySupplyVoltage;
    }

    /**
     * @brief Set the simulated raw ADC value.
     *
     *        Values above the configured ADC maximum are ignored.
     *
     * @param rawValue Simulated raw ADC value.
     */
    void simulateRaw(const std::uint16_t rawValue) noexcept
    {
        // Check given raw value, ignore if above the max limit.
        if (myAdcMax < rawValue) { return; }
        myRawValue = rawValue;
    }

    Stub(const Stub&)             = delete; // No copy constructor.
    Stub(Stub&&)                  = delete; // No move constructor.
    Stub& operator=(const Stub&)  = delete; // No copy assignment.
    Stub& operator=(Stub&&)       = delete; // No move assignment.

private:
    /** Default voltage (3.3 V). */
    static constexpr float DefaultVoltage{3.3f};

    /** Default maximal raw ADC value. */
    static constexpr std::uint16_t DefaultAdcMax{4095U};

    /** Supply voltage in V. */
    const float mySupplyVoltage;

    /** Maximal raw ADC value (used for voltage calculations). */
    const std::uint16_t myAdcMax;

    /** Simulated raw value. */
    std::uint16_t myRawValue;
};
} // namespace driver::adc
