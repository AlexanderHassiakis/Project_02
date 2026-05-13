#include "driver/timer/esp32s3.h"
#include <esp_timer.h>

namespace driver::timer
{
    Esp32s3::Esp32s3() noexcept
        : initialized{true}
        , isRunning{false}
        , periodMs{0}
        , startTimeUs{0} {}

    Esp32s3::~Esp32s3() noexcept
    {
        stop();
    }

    void Esp32s3::start() noexcept
    {
        isRunning = true;
        startTimeUs = esp_timer_get_time();
    }

    void Esp32s3::stop() noexcept
    {
        isRunning = false;
    }

    void Esp32s3::setPeriod(std::uint32_t periodMsValue) noexcept
    {
        periodMs = periodMsValue;
    }

    bool Esp32s3::hasExpired() const noexcept
    {
        if (!isRunning)
        {
            return false;
        }

        int64_t currentTimeUs = esp_timer_get_time();
        int64_t timeMs = (currentTimeUs - startTimeUs) / 1000;
        return timeMs >= static_cast<int64_t>(periodMs);
    }

    bool Esp32s3::isInitialized() const noexcept
    {
        return initialized;
    }

} // namespace driver::timer