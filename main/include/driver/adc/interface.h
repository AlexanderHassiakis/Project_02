#pragma once

#include <cstdint>

namespace driver::adc
{
class Interface 
{
public:
    virtual ~Interface() noexcept= default;

    virtual bool isInitialized() const noexcept = 0;

    virtual std::uint16_t readRaw(std::uint8_t pin) noexcept = 0;

    virtual float readVoltage(std::uint8_t pin) noexcept = 0;
};
} // namespace driver::adc
