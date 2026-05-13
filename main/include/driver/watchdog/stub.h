#pragma once 

#include <chrono>
#include <thread>
#include <cstdint>

#include "driver/watchdog/interface.h"

namespace driver::watchdog {
	class Stub final : public Interface {
	public:
  		Stub() noexcept = default;
  		~Stub() noexcept override =  default;

	void reset() noexcept override
	{
		// Doesn't have to do anything in this implementation.
	}

	private:
};
} // namespace driver::watchdog
