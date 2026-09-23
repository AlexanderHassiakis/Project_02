/**
 * @brief Esp32s3 GPIO Implementation
 */
#include "arch/env/hw_platform.h"
#include "driver/gpio/esp32s3.h"
#include <cstdint>
#include <cstdio>

namespace driver::gpio
{

namespace
{
constexpr gpio_num_t gpioNum(const std::uint8_t pin) noexcept
{ return static_cast<gpio_num_t>(pin); }
} // namespace

Esp32s3::Esp32s3(std::uint8_t pin, Direction direction) noexcept
    : myDirection{direction}
    , myPin{pin}
{
    const auto gpio = gpioNum(myPin);

    gpio_set_direction(gpio, GPIO_MODE_INPUT_OUTPUT);

    // Enable pull-up resistor if specified.
    if (Direction::InputPullup == myDirection) { gpio_set_pull_mode(gpio, GPIO_PULLUP_ONLY); }
}

Esp32s3::~Esp32s3() noexcept
{
    // Gör cleanup, nollställ pinnen.
}

void Esp32s3::output(bool state) noexcept
{
    // Check data direction, terminate the function if configured as input.
    if (Direction::Output != myDirection) { return; }

    const auto gpio = gpioNum(myPin);
    gpio_set_level(gpio, state);
    std::printf("Pin %u is now set as output\n", myPin);
}

bool Esp32s3::input() const noexcept
{
    const auto gpio = gpioNum(myPin);
    return static_cast<bool>(gpio_get_level(gpio));
}

void Esp32s3::toggle() noexcept
{
    // Check data direction, terminate the function if configured as input.
    if (Direction::Output != myDirection) { return; }
    const auto gpio = gpioNum(myPin);
    const bool state{!input()};
    gpio_set_level(gpio, state);
}
} // namespace driver::gpio
