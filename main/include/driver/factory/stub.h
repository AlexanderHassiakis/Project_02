#pragma once

#include <cstdint>
#include <memory>

#include "driver/adc/stub.h"
#include "driver/gpio/stub.h"
#include "driver/serial/stub.h"
#include "driver/tempsensor/stub.h"
#include "driver/timer/stub.h"
#include "driver/watchdog/stub.h"

namespace driver::factory
{
	class Stub : public Interface
	{
	public:
		
		Stub() noexcept
		{
			std::printf("Constructor done!\n")
		}

		~Stub() noexcept override
		{
			std::printf("Destruktor finished!\n")
		}

		std::unique_ptr<adc::Interface> adc(std::uint8_t pin) noexcept override
		{
			(void)(pin);
			return std::make_unique<adc::Stub>();
		}

		std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override
		{
			(void)(pin); 
			return std::make_unique<gpio::Stub>();
		}

		std::unique_ptr<serial::Interface> serial() noexcept override
		{
			return std::make_unique<serial::Stub>();
		}

		std::unique_ptr<timer::Interface> timer() noexcept override
		{
			return std::make_unique<timer::Stub>();
		}

		std::unique_ptr<tempsensor::Interface> tempSensor(std::uint8_t pin, adc::Interface& adcRef) noexcept override
		{
			return std::make_unique<tempsensor::Stub>(pin, adcRef);
		}

        std::unique_ptr<watchdog::Interface> delay_ms(std::uint16_t ms) noexcept override
		{
			return std::make_unique<watchdog::Stub>(ms);
		}
	};

} // namespace include::driver::factory