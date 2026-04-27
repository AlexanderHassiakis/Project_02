#pragma once

namespace include::driver::gpio
{
	class Interface
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
		 void output(std::uint8_t pinNumber) noexcept override;

		/**
		 * @brief Turns input the Gpio_pin.
		 * @param [in] GPIO pinnumber.
		 */
		 void input(std::uint8_t pinNumber) noexcept override;

		/**
		 * @brief Turns on Toggle
		 * @param [in] GPIO pinnumber.
		 */
		 void toggle(std::uint8_t pinNumber) noexcept override;

		/**
		 * @brief Pullup resistor
		 * @param [in] GPIO pinnumber.
		 */
		 void pullUpGpio(std::uint8_t pinNumber) noexcept override;
	}
}
