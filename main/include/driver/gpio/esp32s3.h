
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
     * @brief Construct a new Esp32s3 object
     *
     */
    Esp32s3(std::uint8_t pin, Direction direction) noexcept;
  
    /**
     * @brief Destructor.
     */
    ~Esp32s3() noexcept;

    /**
     * @brief Set GPIO output.
     *
     * @param [in] state GPIO state (true = high, false = low).
     */
    void output(bool state) noexcept override;

    /**
     * @brief Read GPIO input.
     *
     * @return GPIO state (true = high, false = low).
     */
    bool input() const noexcept override;

    /**
     * @brief Toggle GPIO state.
     */
    void toggle() noexcept override;

    /**Esp32s3 construct Forbidden moves/copy. **/
    Esp32s3(const Esp32s3 &) = delete;
    Esp32s3(Esp32s3 &&) = delete;
    Esp32s3 &operator=(const Esp32s3 &) = delete;
    Esp32s3 &operator=(Esp32s3 &&) = delete;

private:
    const Direction myDirection;
    const std::uint8_t myPin;
};
} // namespace include::driver::gpio
