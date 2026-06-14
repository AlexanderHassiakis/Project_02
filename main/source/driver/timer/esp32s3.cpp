//! @note File header missing.
#include <cstdint>

#include "driver/timer/esp32s3.h"
#include "esp_timer.h"

namespace driver::timer
{
// -----------------------------------------------------------------------------
Esp32s3::Esp32s3() noexcept
    //! @note Please use {} for initialization instead of ().
    //! Please place each , on the next line, in line with the :.
    : is_initialized_(true),
    is_running_(false),
    period_ms_(0),
    start_time_us_(0)
{}

// -----------------------------------------------------------------------------
Esp32s3::~Esp32s3() noexcept { stop(); }

// -----------------------------------------------------------------------------
void Esp32s3::start() noexcept
{
    is_running_ = true;
    start_time_us_ = esp_timer_get_time();
}

// -----------------------------------------------------------------------------
void Esp32s3::stop() noexcept
{
    is_running_ = false;
}

// -----------------------------------------------------------------------------
//! @note Consider adding check so that period_ms != 0U.
void Esp32s3::setPeriod(std::uint32_t period_ms) noexcept { period_ms_ = period_ms; }

// -----------------------------------------------------------------------------
bool Esp32s3::hasExpired() const noexcept
{
    if (!is_running_) { return false; }
    //! @note Feel free to use auto here (auto x = y()).
    int64_t current_time_us = esp_timer_get_time();
    int64_t time_ms = (current_time_us - start_time_us_) / 1000;
    return timeMs >= period_ms_;
}

// -----------------------------------------------------------------------------
bool Esp32s3::isInitialized() const noexcept { return is_initialized_; }

} // namespace driver::timer
