
/**
 * @brief Stub for GPIO
 * 
 */
#pragma once

#include "driver/gpio/interface.h"

namespace driver::gpio
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
		Stub() noexcept
			: myState{false}
		{}

	/**
	 * @brief Set GPIO output.
	 *
	 * @param [in] state GPIO state (true = high, false = low).
	 */
	void output(bool state) noexcept override { myState = state; }
	/**
	 * @brief Read GPIO input.
	 *
	 * @return GPIO state (true = high, false = low).
	 */
	bool input() const noexcept { return myState; }

	/**
	 * @brief Toggle GPIO state.
	 */
	void toggle() noexcept { myState = !myState; }
		 
private:
	/** GPIO state (true = high, false = low). */
	bool myState;
};
} // namespace driver::gpio
