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

            /**
             * @brief Start timer.
             * 
             * setPeriod use set period for timer length. 
             */
            void start() noexcept override;
            /**
             * @brief Stops the timer.
             */
            void stop() noexcept override;
            /**
             * @brief Sets the timeout period.
             *
             * @param period_ms Period in milliseconds.
             */
            void setPeriod(std::uint32_t period_ms) noexcept override;
            /**
             * @brief Checks if the timer has reached.
             *
             * @return true if timeout, false if not.
             */
            bool hasExpired() const noexcept override;
            /**
             * @brief Checks if the timer driver is initialized.
             *
             * @return true if initialized, false if not initialized.
             */
            bool isInitialized() const noexcept override;

            /**Esp32s3 construct Forbidden moves/copy. **/
            Esp32s3(const Esp32s3 &) = delete;
            Esp32s3(Esp32s3 &&) = delete;
            Esp32s3 &operator=(const Esp32s3 &) = delete;
            Esp32s3 &operator=(Esp32s3 &&) = delete;

        private:
            bool is_initialized_;
            bool is_running_;
            std::uint32_t period_ms_;
            int64_t start_time_us_; 
    };

} // namespace driver::timer

 // 64-bitars int används för att undvika overflow (32-bitar) Databladet använder 52-bitar hårdvaruregister,
 // programmet läser av som 64-bitar.