
/**
 * @brief Esp32s3 for GPIO
 *
 */
#pragma once

#include <cstdint>
#include <cstdio>
#include "driver/gpio.h"

#include "driver/gpio/direction.h"
#include "driver/gpio/interface.h"

namespace driver::gpio {
class Esp32s3 final : public Interface {
public:
  /**
   * @brief Destructor.
   */
  ~Esp32s3() noexcept = default;

  /**
   * @brief Construct a new Esp32s3 object
   *
   */
  Esp32s3() noexcept 
    :myState(false)
    ,myPullup(false)
    ,myDirection(0)
    ,myPin(0)
  {

  }

  /**
   * @brief Turns output the Gpio_pin.
   * @param [in] GPIO pinnumber.
   */
  void output(std::uint8_t pinNumber, bool state) noexcept override ;

  /**
   * @brief Turns input the Gpio_pin.
   * @param [in] GPIO pinnumber.
   */
  void input(std::uint8_t pinNumber, bool state) noexcept override;

  /**
   * @brief Turns on Toggle
   * @param [in] GPIO pinnumber.
   */
  void toggle(std::uint8_t pinNumber) noexcept override;


  /**
   * @brief Pullup resistor
   * @param [in] GPIO pinnumber.
   */
  void pullUpGpio(std::uint8_t pinNumber) noexcept override;

  /**
   * @brief Checks what status the GPIO port has.
   * 
   * @param pinNumber 
   */
  int checkGpio(std::uint8_t pinNumber) noexcept;

private:
  /** GPIO state (true = high, false = low). */

  bool myState;
  bool myPullup;
  std::uint8_t myDirection;
  std::uint8_t myPin;
};
} // namespace include::driver::gpio
