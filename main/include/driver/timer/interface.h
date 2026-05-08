#pragma once

#include <cstdint>

namespace driver::timer
{
class Interface
{
public:
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
    * @param period_ms Period in milliseconds.
    */
    virtual void setPeriod(std::uint32_t period_ms) noexcept = 0;

    /**
    * @brief Checks if the timer has reached.
    * 
    * @return true if timeout, false if not.
    */
    virtual bool hasExpired() const noexcept = 0;

    /**
    * @brief Checks if the timer driver is initialized.
    * 
    * @return true if initialized, false if not initialized.
    */
    virtual bool isInitialized() const noexcept = 0;
};
} // namespace driver::timer
