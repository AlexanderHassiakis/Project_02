#pragma once

#include "driver/timer/interface.h"

namespace driver::timer
{
class Stub final : public Interface
{
public:
    /**
     * @brief Construct a new Stub object
     * 
     */
    Stub() noexcept 
    : isRunning_{false}
    , period_ms_{0}
    , hasExpired_{false}
    , isInitialized_{true} {}

    /**
     * @brief Starts the timer.
     */
    void start() noexcept override 
    {
        isRunning_ = true;
        hasExpired_ = false;
    }

    /**
     * @brief Stops the timer.
     */
    void stop() noexcept override 
    {
        isRunning_ = false;
    }

    /**
     * @brief Sets the timeout period.
     *
     * @param period_ms Period in milliseconds.
     */
    void setPeriod(std::uint64_t period_ms) noexcept override 
    {
        period_ms_ = period_ms;
    }

    /**
     * @brief Checks if the timer has reached.
     *
     * @return true if timeout, false if not.
     */
    bool hasExpired() const noexcept override 
    {
        return hasExpired_;
    }

    /**
     * @brief Checks if the timer driver is initialized.
     *
     * @return true if initialized, false if not initialized.
     */
    bool isInitialized() const noexcept override 
    {
        return isInitialized_;
    }
            
    /**
    * @brief  Trigger a timeout manually.
    */
    void triggerTimeout() noexcept 
    {
        if (isRunning_) 
        {
            hasExpired_ = true;
        }
    }

    Stub(const Stub &)              = delete;
    Stub(Stub &&)                   = delete;
    Stub operator=(const Stub &)    = delete;
    Stub operator=(Stub &&)         = delete;

private:
    bool isRunning_;
    std::uint64_t period_ms_;
    bool hasExpired_;
    bool isInitialized_;
};
} // namespace driver::timer