
/**
 * @brief  GPIO
 *
 */


#include <cstdint>
#include <cstdio>

#include "driver/gpio.h"

#include "driver/gpio/direction.h"
#include "driver/gpio/interface.h"
#include "driver/gpio/esp32s3.h"

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
  Esp32s3() noexcept = default;

  
  void output(std::uint8_t pinNumber, bool state) noexcept  {

    constexpr gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};
    gpio_set_direction(gpioNumber, GPIO_MODE_OUTPUT);
    gpio_set_level(gpioNumber, state);
    std::printf("%s", pinNumber, "Is now set as output");

    myDirection = Direction::output;
  }


  void input(std::uint8_t pinNumber, bool state) noexcept  {
    std::printf("%s", pinNumber, "Is now set as input");
    constexpr gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};
    gpio_set_direction(gpioNumber, GPIO_MODE_INPUT);
    gpio_set_level(gpioNumber, state);
	
    myDirection = Direction::input;
  }


  void toggle(std::uint8_t pinNumber) noexcept  {
    constexpr gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};
    gpio_set_direction(gpioNumber, GPIO_MODE_OUTPUT);
    gpio_set_level(gpioNumber,=!);

  }


  void pullUpGpio(std::uint8_t pinNumber) noexcept  {
    constexpr gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};
    gpio_set_direction(gpioNumber, GPIO_PULLUP_ENABLE);
    std::printf("%s", pinNumber, "Is now set as pullup");
    myDirection = Direction::input;
  }

private:
  /** GPIO state (true = high, false = low). */
  bool myState;
  bool myPullup;
  const myDirection;
  const std::uint8_t myPin;
};
} // namespace driver::gpio
