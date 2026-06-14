/**
 * @brief Factory interface.
 */
#pragma once

#include <cstdint>
#include <memory>

#include "driver/adc/interface.h"
#include "driver/gpio/interface.h"
#include "driver/mqtt/interface.h"
#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"

namespace driver
{
namespace adc{class Interface; }
namespace gpio{class Interface; }
namespace mqtt{class Interface; }
namespace serial{class Interface; }
namespace tempsensor{class Interface; }
namespace timer{class Interface; }	
namespace watchdog{class Interface; }
} // namespace driver

namespace driver::factory
{
class Interface
{
public:
    /**
     * @brief Destructor
     * 
     */
    virtual ~Interface() noexcept = default;

    //! @note Comments are missing @param and @return, but OK.

    /**
     * @brief Create ADC driver.
     * 
     * @return ADC driver.
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
    virtual std::unique_ptr<watchdog::Interface> watchdog() noexcept = 0;

    /**
     * @brief Create temperature sensor.
     *
     * @param pin ADC pin
     * @param adc ADC reference
     */
    virtual std::unique_ptr<tempsensor::Interface>
    
    //! @note Doxygen comment missing!
    tempSensor(std::uint8_t pin, adc::Interface &adc) noexcept = 0;

    /**
     * @brief mqtt communication
     */
    virtual std::unique_ptr<mqtt::Interface> mqtt() noexcept = 0;
};
} // namespace driver::factory
