#pragma once
#include "driver/timer/interface.h"
#include <cstdint>
#include <esp_timer.h>

namespace driver::timer 
{
class Esp32s3 : public Interface 
{
public:
    Esp32s3() noexcept;
    ~Esp32s3() noexcept override;

    void start() noexcept override;
    void stop() noexcept override;
    void setPeriod(std::uint32_t periodMs) noexcept override;
    bool hasExpired() const noexcept override;
    bool isInitialized() const noexcept override;

private:
    bool initialized;
    bool isRunning;
    std::uint32_t periodMs;
    int64_t startTimeUs; 
};
} // namespace driver::timer