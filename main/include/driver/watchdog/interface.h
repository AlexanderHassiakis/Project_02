#pragma once

#include <cstdint>

namespace driver::watchdog
{
class Interface
{
public:
	virtual ~Interface() noexcept  = default;
	virtual void reset() noexcept = 0;
};	
} // namespace driver::watchdog

