//! @note File header missing.
#pragma once

//! @note Sort headers. Include standard library before project headers.
#include "driver/timer/interface.h"
#include <cstdint>

#include <esp_timer.h>

namespace driver::timer
{
//! @note Class header missing.
//! @note Add final.
class Esp32s3 : public Interface
{
public:
    //! @note Doxygen comments missing for these.
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
    //! @note Please use prefix 'my' for member variables instead of postfix _.
    bool is_initialized_;
    bool is_running_;
    std::uint32_t period_ms_;

    //! @note Please use std::int64_t instead of int64_t.
    int64_t start_time_us_; 
};
} // namespace driver::timer
