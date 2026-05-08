#pragma once

#include <cstdint>

namespace driver::watchdog
{
	class Interface
	{
	public:

	Interface() noexcept = default;
	~Interface() noexcept  = default;

	virtual void delay_ms(const std::uint16_t ms) noexcept = 0;


	private:
		/* data */
	
	};	
} // namespace driver::watchdog

