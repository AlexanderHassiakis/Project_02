
/**

 * @brief Factory Stub
 * 
 */
#pragma once
#include <cstdint>
#include <cstdio>
#include <memory>
#include "esp_log.h" // För snygg debugging (ESP_LOGI)

#include "driver/factory/interface.h"
#include "driver/gpio/stub.h"

namespace driver::factory
{
	class Stub final : public interface
	{
		Stub() noexcept
		{
			std::printf("Constructor on\n")
		}

		~Stub() noexcept override
		{
			std::printf("Destruktor finished\n")
		}

		std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override
		{
			(void)(pin);
			return std::make_unique<gpio::Stub>();
		}

		    Stub(const Stub&)            = delete;
    		Stub(Stub&&)                 = delete;
    		Stub& operator=(const Stub&) = delete;
    		Stub& operator=(Stub&&)      = delete;
	};
} // namespace driver::factory
