#pragma once

#include "driver/adc/stub.h"
#include "driver/gpio/stub.h"
#include "driver/serial/stub.h"
#include "driver/tempsensor/stub.h"
#include "driver/timer/stub.h"

namespace include::driver::factory
{
	class Stub : public Interface
	{
	public:

		std::unique_ptr<adc::Interface> adc() noexcept override
		{
			return std::make_unique<adc::Stub>();
		}

		std::unique_ptr<gpio::Interface> gpio() noexcept override
		{
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

		std::unique_ptr<tempsensor::Interface> tempSensor(
			std::uint8_t pin,
			adc::Interface& adcRef
		) noexcept override
		{
			return std::make_unique<tempsensor::Stub>(pin, adcRef);
		}
	};

} // namespace include::driver::factory