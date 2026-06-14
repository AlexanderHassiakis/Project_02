/**
 * @brief ESP32-S3 ADC driver declaration.
 */

#pragma once 

#include <cstdint>

#include "driver/adc/interface.h"

namespace driver::adc
{

/**
 * @brief ADC driver for ESP32-S3 using oneshot mode.
 *
 *        This class is non-copyable and non-movable.
 */
//! @note Added final.
class Esp32s3 final : public Interface
{
public:
    /**
     * @brief Constructor.
     */
    Esp32s3() noexcept;

    /**
     * @brief Destructor.
     */
    ~Esp32s3() noexcept override;

    /**
     * @brief Check if the ADC was initialized successfully.
     *
     * @return True if initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Read raw ADC value from a pin.
     *
     * @param pin ADC pin/channel.
     * 
     * @return Raw value (0–4095 for 12-bit ADC).
     */
    std::uint16_t readRaw(std::uint8_t pin) noexcept override;

    /**
     * @brief Read voltage from a pin.
     *
     *        Converts the raw ADC value to voltage based on ADC range and supply voltage.
     *
     * @param pin ADC pin/channel.
     * 
     * @return Voltage in volts.
     */
    float readVoltage(std::uint8_t pin) noexcept override;

    Esp32s3(const Esp32s3& )            = delete; // No copy constructor.
    Esp32s3(Esp32s3&&)                  = delete; // No move constructor.
    Esp32s3& operator=(const Esp32s3& ) = delete; // No copy assignment.
    Esp32s3& operator=(Esp32s3&&)       = delete; // No move assignment.

private:
    /** True if initialization succeeded, false otherwise. */
    bool myInitialized;
};
} // namespace driver::adc
