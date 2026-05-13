#pragma once

#include <cstdint>

namespace driver::timer
{
/**
 * @brief Interface for a timer driver.
 */
class Interface
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Starts the timer.
     */
    virtual void start() noexcept = 0;

    /**
     * @brief Stops the timer.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Sets the timeout period.
     * 
     * @param periodMs The desired period in milliseconds.
     */
    virtual void setPeriod(std::uint32_t periodMs) noexcept = 0;

    /**
     * @brief Checks if the timer has reached set period.
     * 
     * @return true if the time since start() is >= periodMs.
     * @return false if not running or period not reached.
     */
    virtual bool hasExpired() const noexcept = 0;

    /**
     * @brief Checks if the driver is correctly initialized.
     * * @return true if the stub is ready for use.
     */
    virtual bool isInitialized() const noexcept = 0;
};
} // namespace driver::timer
