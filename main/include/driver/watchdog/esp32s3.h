#pragma once

#include <cstdint>

// För ESP32 Delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace driver::watchdog {
class Esp32s3 : public Interface{
public:
	Esp32s3() noexcept = default;
	~Esp32s3()  noexcept override = default;

	void reset() noexcept override
	{	
		// This is the "Watchdog feeding".
		// pdMS_TO_TICKS(10) tells the system:
		// "I am pausing for 10 milliseconds, let other processes run."
		vTaskDelay(pdMS_TO_TICKS(sleepTime_ms)); // Delay for loopen.
	}

private:
    /** Sleep time to reset the watchdog (we need to enter sleep mode for the RTOS to reset it). */
    static constexpr std::uint8_t sleepTime_ms{1U};
};
} // namespace driver::watchdog