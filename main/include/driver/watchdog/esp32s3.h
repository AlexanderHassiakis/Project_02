//! @note File header missing.
#pragma once

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace driver::watchdog
{
//! @note Class header missing.
//! @note Set to final.
class Esp32s3 : public Interface
{
public:
	/**
	 * @brief Construct a new Esp 3 2s 3 object
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
	//! @note Good work with this one. Tip: Use PascalCase for class-global constants.
	//! camelCase is fine for local constants, such as those defined in a function.
	static constexpr std::uint8_t sleepTime_ms{10U};
};
} // namespace driver::watchdog
