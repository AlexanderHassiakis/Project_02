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
            void setPeriod(std::uint32_t period_ms) noexcept override;
            bool hasExpired() const noexcept override;
            bool isInitialized() const noexcept override;

        private:
            bool is_initialized_;
            bool is_running_;
            std::uint32_t period_ms_;
            
            // 64-bitars int för att undvika overflow (32-bitar) Databladet använder 52-bitar hårdvaruregister,
            // mjukvaran läser av som 64-bitar.
            int64_t start_time_us_; 
    };

} // namespace driver::timer