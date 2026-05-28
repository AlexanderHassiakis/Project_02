#pragma once

#include <cstdint>	// Int variavber

#include "driver/serial/interface.h"

namespace driver::serial
{
class Esp32s3 final : public Interface
{
public:
	/**
	 * @brief Constructor.
	 */
	Esp32s3() noexcept;

	/**
	 * @brief Destructor
	 * 			Deletes to release memory
	 */
	~Esp32s3() noexcept;

	/**
	 * @brief Initialize the Uart communication
	 *
	 * @return True on success, false on failure.
	 *
	 */
	bool init() noexcept override;

	/**
	 * @brief Construct a new send string.
	 * 
	 * @param msg 
	 */
	void send(const char *msg) noexcept override;

	/**
	 * @brief Send bytes.
	 * 
	 * @param buf 
	 * @param bufLen 
	 */
	void send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept override;

    /**
	 * @brief receive commands
	 *
	 * @param[out] buf Buffer holding received data.
	 * @param[in] bufLen Buffer length in bytes.
	 *
	 * @return Number of received bytes.
	 */
	std::uint16_t received(std::uint8_t *buf, std::uint16_t bufLen) noexcept override;

	/**
	 * @brief receive commands
	 *
	 * @param[out] buf Buffer holding received data.
	 * @param[in] bufLen String length in bytes.
	 *
	 * @return Number of received bytes.
	 */
    std::uint16_t received(const char *str, std::uint16_t strLen) noexcept override;

        /**
	 * @brief connected
	 *
	 * @return true
	 * @return false
	 */
	bool isConnected() const noexcept override;

	/**Esp32s3 construct Forbidden moves/copy. **/
	Esp32s3(const Esp32s3 &)            = delete; // No copy constructor.
	Esp32s3(Esp32s3 &&)                 = delete; // No move constructor.
	Esp32s3 &operator=(const Esp32s3 &) = delete; // No copy assignment.
	Esp32s3 &operator=(Esp32s3 &&)      = delete; // No move assignment.

private:
	/** Buffer size. */
	static constexpr std::uint16_t BufSize{200U}; // Array size.
	std::uint8_t myDataMsg[BufSize]{0U};		  // Data array
	const char *myMsg;

	bool myDataAvailable;	 // Status if there is a command.
	bool myConnectionStatus; // status off the UART connection
};
} // namespace driver::serial
