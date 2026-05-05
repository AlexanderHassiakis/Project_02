/**
 * @brief Data directions.
 */
#pragma once

#include <cstdint>

namespace driver::gpio
{
/**
 * @brief Enumeration of data directions.
 */
enum class Direction : std::uint8_t
{
    Input,
	InputPullup,
	Output,
};
} // namespace driver::gpio
