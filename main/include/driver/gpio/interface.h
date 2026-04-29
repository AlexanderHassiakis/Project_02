
#pragma once

#include <cstdint>


namespace driver::gpio
{
	class Interface
	{
		public:
			/**
			* @brief Destructor.
			*/
			virtual ~Interface() noexcept = default;

			/**
			 * @brief Turns output the Gpio_pin.
			 * @param [in] GPIO pinnumber.
			 */
			virtual void output(std::uint8_t pinNumber,bool state) noexcept = 0;

			/**
			 * @brief Turns input the Gpio_pin.
			 * @param [in] GPIO pinnumber.
			 */
			virtual void input(std::uint8_t pinNumber, bool state) noexcept = 0;

			/**
			 * @brief Turns on Toggle
			 * @param [in] GPIO pinnumber.
			 */
			virtual void toggle(std::uint8_t pinNumber) noexcept = 0;

			/**
			 * @brief Pullup resistor
			 * @param [in] GPIO pinnumber.
			 */
			virtual void pullUpGpio(std::uint8_t pinNumber) noexcept = 0;


	};


} // include::driver::gpio




