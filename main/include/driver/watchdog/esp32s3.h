#pragma once

#include <cstdint>

// För ESP32 Delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace driver::watchdog {
	class Esp32s3 {
	public:

		Esp32s3()  noexcept  = default;
		~Esp32s3()  noexcept = default;

		void delay_ms(const std::uint16_t ms) noexcept 
		{	
			// This is the "Watchdog feeding".
			// pdMS_TO_TICKS(10) tells the system:
			// "I am pausing for 10 milliseconds, let other processes run."
			vTaskDelay(pdMS_TO_TICKS(ms)); // Delay for loopen.
		}

	private:
	/* data */
	};
} // namespace driver::watchdog