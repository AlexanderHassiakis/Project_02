#pragma once 

#include <cstdint>

#include "driver/adc/interface.h"

namespace driver::adc
{
class Esp32s3 : public Interface
{
public:
    Esp32s3() noexcept;
    ~Esp32s3() noexcept override;

    bool isInitialized() const noexcept override;
    std::uint16_t readRaw(std::uint8_t pin) noexcept override;
    float readVoltage(std::uint8_t pin) noexcept override;

private:
    bool myInitialized;
};
} // driver::adc