#pragma once

#include "driver/timer/interface.h"

namespace driver::timer
{
    class Stub : public Interface
    {
        public:
            Stub() noexcept 
                : isRunning_(false), period_ms_(0), hasExpired_(false), isInitialized_(true) {}

            void start() noexcept override 
            {
                isRunning_ = true;
                hasExpired_ = false;
            }

            void stop() noexcept override 
            {
                isRunning_ = false;
            }

            void setPeriod(std::uint32_t period_ms) noexcept override 
            {
                period_ms_ = period_ms;
            }

            bool hasExpired() const noexcept override 
            {
                return hasExpired_;
            }

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

        private:
            bool isRunning_;
            std::uint32_t period_ms_;
            bool hasExpired_;
            bool isInitialized_;
    };

} // namespace driver::timer