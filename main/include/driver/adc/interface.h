/**
 * @file interface.h
 * @brief Common ADC driver interface.
 */

#pragma once

#include <cstdint>

namespace driver::adc
{
/**
 * @brief Common interface for ADC drivers.
 */
class Interface
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the ADC driver is ready to use.
     *
     * @return true if the driver is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Read a raw ADC sample from the selected pin.
     *
     * @param pin ADC-capable pin to read from.
     * @return Raw ADC value.
     */
    virtual std::uint16_t readRaw(std::uint8_t pin) noexcept = 0;

    /**
     * @brief Read the input voltage on the selected pin.
     *
     * @param pin ADC-capable pin to read from.
     * @return Measured voltage in volts.
     */
    virtual float readVoltage(std::uint8_t pin) noexcept = 0;
};
} // namespace driver::adc
