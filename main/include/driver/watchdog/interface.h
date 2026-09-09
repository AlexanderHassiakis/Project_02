#pragma once

#include <cstdint>

namespace driver::watchdog
{
class Interface
{
public:
    /**
     * @brief Destroy the Interface object
     *
     */
    virtual ~Interface() noexcept = default;
    /**
     * @brief Watchdog delay.
     *
     */
    virtual void reset() noexcept = 0;
};
} // namespace driver::watchdog
