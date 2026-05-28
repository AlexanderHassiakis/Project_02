#pragma once

#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace driver::watchdog
{
	class Esp32s3 : public Interface
	{
	public:
		/**
		 * @brief Construct a new Esp 3 2s 3 object
		 * 
		 */
		Esp32s3() noexcept = default;

		/**
		 * @brief Destroy the Esp 3 2s 3 object
		 * 
		 */
		~Esp32s3() noexcept override = default;

		/**
		 * @brief Thread sleep to avoid esp watchdog reset.
		 *
		 */
		void reset() noexcept override
		{
			vTaskDelay(pdMS_TO_TICKS(sleepTime_ms)); // Delay for loopen.
		}

		/**Esp32s3 construct Forbidden moves/copy. **/
		Esp32s3(const Esp32s3 &) = delete;
		Esp32s3(Esp32s3 &&) = delete;
		Esp32s3 &operator=(const Esp32s3 &) = delete;
		Esp32s3 &operator=(Esp32s3 &&) = delete;

	private:
		/** Sleep time to reset the watchdog (we need to enter sleep mode for the RTOS to reset it). */
		static constexpr std::uint8_t sleepTime_ms{10U};
	};
} // namespace driver::watchdog