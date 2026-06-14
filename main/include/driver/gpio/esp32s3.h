/**
 * @brief Esp32-S3 GPIO driver.
 */
#pragma once

//! @note Removed unused <cstdio>.
#include <cstdint>

#include "driver/gpio.h"
#include "driver/gpio/direction.h"
#include "driver/gpio/interface.h"

//! @note Brackets on the next line, please.
namespace driver::gpio {

/**
 * @brief Esp32-S3 GPIO driver.
 */
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
    //! @note Added override.
    ~Esp32s3() noexcept override;

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

    Esp32s3(const Esp32s3&)            = delete; // No copy constructor.
    Esp32s3(Esp32s3&&)                 = delete; // No move constructor.
    Esp32s3& operator=(const Esp32s3&) = delete; // No copy assignment.
    Esp32s3& operator=(Esp32s3&&)      = delete; // No move assignment.

private:
    //! @note Should be commented.
    const Direction myDirection;
    const std::uint8_t myPin;
};
} // namespace driver::gpio
