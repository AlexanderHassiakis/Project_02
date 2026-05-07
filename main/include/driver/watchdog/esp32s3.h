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
		// Detta är "Watchdog-matningen".
		// pdMS_TO_TICKS(10) säger till systemet:
		// "Jag pausar i 10 millisekunder, låt andra processer köra."
			vTaskDelay(pdMS_TO_TICKS(ms)); // Delay for loopen.
		}

	private:
	/* data */
	};
} // namespace driver::watchdog