//! @note File header missing.
#pragma once

namespace driver::watchdog
{
//! @note Class header missing.
class Interface
{
public:
	/**
	 * @brief Destructor.
	 */
	virtual ~Interface() noexcept = default;

	/**
	 * @brief Reset the watchdog.
	 */
	virtual void reset() noexcept = 0;
};
} // namespace driver::watchdog
