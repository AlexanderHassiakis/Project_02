//! @note File header missing.
#pragma once

#include <cstdint>

namespace driver::serial
{
//! @note Class header missing.
class Interface
{
public:
	/**
	 * @brief destructor
	 * 			Deletes to release memory
	 */
	virtual ~Interface() noexcept = default;

	//! @note Write imperative comments, like "Initialize the ..." instead of "Initializes".
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
	//! @note Missing param documentation.
	virtual void send(const char* msg) noexcept = 0;

	//! @note Missing documentation.
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
	//! @note Good call to use the same comment for the overloaded methods here. But if you do,
	//! please ensure that the input arguments are called the same. How about buf and bufLen
	//! for both?
	virtual std::uint16_t received(std::uint8_t* buf, std::uint16_t bufLen) noexcept = 0;
	virtual std::uint16_t received(const char *str, std::uint16_t strLen) noexcept = 0;

	/**
	 * @brief connected
	 * 
	 * @return true 
	 * @return false 
	 */
	//! @note Specificy what is returned.
	virtual bool isConnected() const noexcept = 0;
};
} // namespace driver::serial
