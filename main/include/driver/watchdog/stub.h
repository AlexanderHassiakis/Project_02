#pragma once

#include <chrono>
#include <thread>
#include <cstdint>

#include "driver/watchdog/interface.h"

namespace driver::watchdog
{
	class Stub final : public Interface
	{
	public:
		/**
		 * @brief Construct a new Stub object
		 * 
		 */
		Stub() noexcept = default;
		/**
		 * @brief Destroy the Stub object
		 * 
		 */
		~Stub() noexcept override = default;

		/**
		 * @brief Thread sleep to avoid esp watchdog reset.
		 * 
		 */
		void reset() noexcept override
		{
			// Doesn't have to do anything in this implementation.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		/**Esp32s3 construct Forbidden moves/copy. **/
		Stub(const Stub &) = delete;
		Stub(Stub &&) = delete;
		Stub &operator=(const Stub &) = delete;
		Stub &operator=(Stub &&) = delete;

	private:
	};
} // namespace driver::watchdog
