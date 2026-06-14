/**
 * @brief Stub GPIO driver.
 */
#pragma once

#include "driver/gpio/interface.h"

namespace driver::gpio
{
/**
 * @brief Stub GPIO driver.
 * 
 *        This class is non-copyable and non-movable.
 */
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
	//! @note Should be placed above the destructor.
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
	//! @note Added override.
	bool input() const noexcept override { return myState; }

	/**
	 * @brief Toggle GPIO state.
	 */
	//! @note Added override.
	void toggle() noexcept override { myState = !myState; }

	Stub(const Stub&)            = delete; // No copy constructor.
	Stub(Stub&&)                 = delete; // No move constructor.
	Stub& operator=(const Stub&) = delete; // No copy assignment.
	Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
	/** GPIO state (true = high, false = low). */
	bool myState;
};
} // namespace driver::gpio
