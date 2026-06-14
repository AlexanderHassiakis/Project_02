//! @note File header missing (I've added it to other files, now I'll just comment).
#pragma once

#include <cstdint>

//! @note Please place brackets on the next line instead. :)
namespace driver::gpio {
class Interface {
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Set GPIO output.
     *
     * @param [in] state GPIO state (true = high, false = low).
     */
    virtual void output(bool state) noexcept = 0;

    /**
     * @brief Read GPIO input.
     *
     * @return GPIO state (true = high, false = low).
     */
    virtual bool input() const noexcept = 0;

    /**
     * @brief Toggle GPIO state.
     */
    virtual void toggle() noexcept = 0;
};
} // namespace driver::gpio
