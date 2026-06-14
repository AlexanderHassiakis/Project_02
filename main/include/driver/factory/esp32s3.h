/**
 * @brief ESP32-S3 factory.
 */
#pragma once

#include <cstdint>
#include <memory>

#include "driver/adc/esp32s3.h"
#include "driver/factory/interface.h"
#include "driver/gpio/esp32s3.h"
#include "driver/mqtt/esp32s3.h"
#include "driver/serial/esp32s3.h"
#include "driver/timer/esp32s3.h"
#include "driver/tempsensor/tmp36.h"
#include "driver/watchdog/esp32s3.h"

namespace driver::factory 
{
/**
 * @brief ESP32-S3 factory.
 * 
 *        This class should be non-copyable and non-movable.
 */
class Esp32s3 final : public Interface 
{
public:
    //! @note Doxygen comments missing!
    ~Esp32s3() noexcept override = default;

    std::unique_ptr<adc::Interface> adc(std::uint8_t pin) noexcept override;
   
    std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override;
    
    std::unique_ptr<serial::Interface> serial() noexcept override;

    std::unique_ptr<timer::Interface> timer() noexcept override;

    std::unique_ptr<tempsensor::Interface> tempSensor(std::uint8_t pin, adc::Interface& adc) noexcept override;
    
    std::unique_ptr<watchdog::Interface> watchdog() noexcept override;

    std::unique_ptr<mqtt::Interface> mqtt() noexcept override;

    //! @note Please prevent copy and move operations here!
};
} //namespace driver::factory
