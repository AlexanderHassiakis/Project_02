/**
 * @brief ESP32S3 Factory driver
 * 
 */

#pragma once

#include <cstdint>
#include <memory>
#include "esp_log.h" // För snygg debugging (ESP_LOGI)

#include "driver/factory/interface.h"
#include "driver/gpio/esp32s3.h"
static const char*TAG = "FACTORY ESP32";

namespace driver::factory
{
	class Esp32s3 final : public interface
	{
		
		public:
			/**
			 * @brief Construct a new Esp32s3 object
			 * 
			 */
			Esp32s3() noexcept = default;
			/**
			 * @brief Destroy the Esp32s3 object
			 * 
			 */
			~Esp32s3 () noexcept override {
				ESP_LOGI(TAG,"Factory destroyed!\n")
			}

			std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override;


			Esp32s3(const Esp32s3&)            = delete;
			Esp32s3(Esp32s3&&)                 = delete;
			Esp32s3& operator=(const Esp32s3&) = delete;
			Esp32s3& operator=(Esp32s3&&)      = delete;

		private:
			/* data */



	};
} // namespace driver::factory


