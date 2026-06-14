/**
 * @brief Esp32s3 GPIO Implementation
 */

//! @note Include standard headers first.
#include "driver/gpio/esp32s3.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <cstdint>
#include <cstdio>

namespace driver::gpio
{
namespace
{
// -----------------------------------------------------------------------------
constexpr gpio_num_t gpioNum(const std::uint8_t pin) noexcept
{
    return static_cast<gpio_num_t>(pin);
}
} // namespace

// -----------------------------------------------------------------------------
Esp32s3::Esp32s3(std::uint8_t pin, Direction direction) noexcept
    : myDirection{direction}
    , myPin{pin}
{
    // Fixed: Bracket on next line with proper indentation
    const auto gpio = gpioNum(myPin);

    gpio_set_direction(gpio, GPIO_MODE_INPUT_OUTPUT);

    // Enable pull-up resistor if specified.
    if (Direction::InputPullup == myDirection)
    {
        gpio_set_pull_mode(gpio, GPIO_PULLUP_ONLY);
    }
}

// -----------------------------------------------------------------------------
Esp32s3::~Esp32s3() noexcept
{
    //! @remark GPIO cleanup missing! Please fix this during the autumn. :)
}

// -----------------------------------------------------------------------------
void Esp32s3::output(bool state) noexcept
{
    // Check data direction, terminate the function if configured as input.
    if (Direction::Output != myDirection) { return; }
    const auto gpio = gpioNum(myPin);
    gpio_set_level(gpio, state);
    std::printf("Pin %u is now set as output\n", myPin);
}

// -----------------------------------------------------------------------------
bool Esp32s3::input() const noexcept
{
    const auto gpio = gpioNum(myPin);
    return static_cast<bool>(gpio_get_level(gpio));
}

// -----------------------------------------------------------------------------
void Esp32s3::toggle() noexcept
{
    // Check data direction, terminate the function if configured as input.
    if (Direction::Output != myDirection) { return; }
    const auto gpio = gpioNum(myPin);
    const bool state{!input()};
    gpio_set_level(gpio, state);
}
} // namespace driver::gpio
