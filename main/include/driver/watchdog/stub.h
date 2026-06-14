//! @note File header missing.
#pragma once

//! @note Sort headers.
#include <chrono>
#include <thread>
#include <cstdint>

#include "driver/watchdog/interface.h"

namespace driver::watchdog
{
//! @note Class header missing.
class Stub final : public Interface
{
public:
	/**
	 * @brief Construct a new Stub object.
	 */
	Stub() noexcept = default;

	/**
	 * @brief Destroy the Stub object.
	 */
	~Stub() noexcept override = default;

	/**
	 * @brief Simulates watchdog reset with a short sleep.
	 */
	void reset() noexcept override
	{
		//! @note Consider using a constexpr for the delay, for instance 'Sleep_ms' I added below.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	/**Stub construct Forbidden moves/copy. **/
	Stub(const Stub &) = delete;
	Stub(Stub &&) = delete;
	Stub &operator=(const Stub &) = delete;
	Stub &operator=(Stub &&) = delete;

private:
    /** Watchdog sleep duration in ms. */
    static constexpr std::uint8_t Sleep_ms{100U};
};
} // namespace driver::watchdog
