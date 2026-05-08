#pragma once 

#include <chrono>
#include <thread>
#include <cstdint>

namespace driver::watchdog {
	class Stub {
	public:
  		Stub() noexcept = default;
  		~Stub() noexcept =  default;

		void delay_ms(const std::uint16_t ms) noexcept override {

			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}

	private:
  	/* data */
	};
} // namespace driver::watchdog
