
/**
 * @brief Stub for GPIO
 * 
 */
#pragma once

#include <cstdint>
#include <cstdio>

#include "driver/gpio/direction.h"
#include "driver/gpio/interface.h"

namespace include::driver::gpio
{
	class Stub final : public Interface
	{
	public:
		/**
		 * @brief Destructor.
		 */
		 ~Stub() noexcept = default;

		/**
		 * @brief Construct a new Stub object
		 * 
		 */
		 Stub() noexcept = default;

		/**
		 * @brief Turns output the Gpio_pin.
		 * @param [in] GPIO pinnumber.
		 */
		 void output(std::uint8_t pinNumber) noexcept override
		 {
			std::printf("%s",pinNumber,"Is now set as output");

		 }

		/**
		 * @brief Turns input the Gpio_pin.
		 * @param [in] GPIO pinnumber.
		 */
		 void input(std::uint8_t pinNumber) noexcept override
		 {
			std::printf("%s",pinNumber,"Is now set as input");
		 }

		/**
		 * @brief Turns on Toggle
		 * @param [in] GPIO pinnumber.
		 */
		 void toggle(std::uint8_t pinNumber) noexcept override
		 {
			std::printf("%s",pinNumber,"Toggels GPIO if false turns true and false if true before.");	
		 }

		/**
		 * @brief Pullup resistor
		 * @param [in] GPIO pinnumber.
		 */
		 void pullUpGpio(std::uint8_t pinNumber) noexcept override
		 {
			std::printf("%s",pinNumber,"Internal pull up resistor activated and set pin to input.");
		 }
		 
		private:
		    /** GPIO state (true = high, false = low). */
			bool myState;
			const Direction myDirection;
			const std::uint8_t myPin;

		
	};
}
