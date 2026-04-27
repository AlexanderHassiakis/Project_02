#pragma once

#include <cstdint.h>

namespace include::driver
{
class Interface
{
public:

	/**
	 * @brief destructor
	 * 			Deletes to release memory
	 */
	virtual ~Interface() noexcept = default:


	/**
	 * @brief Initializes the Uart communication
	 * 
	 * @return True on success, false on
	 * 
	 */
	virtual bool init() noexcept = 0;

	/**
	 * @brief sending commands/data
	 * 
	 */
	virtual void send(const char* msg) noexcept = 0;

	virtual void send(const std::uint8_t* buf, std::uint16_t bufLen) noexcept = 0;


	/**
	 * @brief recives commands/
	 * 
	 * @param[out] buf Buffer holding received data.
	 * @param[in] bufLen Buffer length in bytes.
	 * 
	 * @return Number of received bytes.
	 * 
	 */
	virtual std::uint16_t received(std::uint8_t* buf, std::uint16_t bufLen) noexcept = 0;

	/**
	 * @brief connected
	 * 
	 * @return true 
	 * @return false 
	 */
	virtual bool isConnected() const noexcept = 0;
};
} // namespace include::driver::interface
