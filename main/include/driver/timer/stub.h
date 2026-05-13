#pragma once
#include "driver/timer/interface.h"
#include <cstdint>

namespace driver::timer 
{
class Stub final : public Interface 
{
public:
    Stub() noexcept 
        : isRunning{false}
        , currentPeriodMs{0}
        , isExpired{false}
        , initialized{true} {}

    void start() noexcept override 
    {
        isRunning = true;
        isExpired = false;
    }

    void stop() noexcept override 
    {
        isRunning = false;
    }

    void setPeriod(std::uint32_t periodMs) noexcept override 
    {
        currentPeriodMs = periodMs;
    }

    bool hasExpired() const noexcept override 
    {
        return isExpired;
    }

    bool isInitialized() const noexcept override 
    {
        return initialized;
    }
            
    void triggerTimeout() noexcept 
    {
        if (isRunning) 
        {
            isExpired = true;
        }
    }

    Stub(const Stub &)              = delete;
    Stub(Stub &&)                   = delete;
    Stub operator=(const Stub &)    = delete;
    Stub operator=(Stub &&)         = delete;

private:
    bool isRunning;
    std::uint32_t currentPeriodMs;
    bool isExpired;
    bool initialized;
};
} // namespace driver::timer
