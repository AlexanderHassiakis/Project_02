//! @note File header missing.
#pragma once

//! @note Sort headers: standard library before project headers.
#include <cstdint>

#include "driver/serial/interface.h"

//! @note Bracket on the next line, please.
namespace driver::serial
{
//! @note Class header missing.
class Esp32s3 final : public Interface
{
public:
	/**
	 * @brief Constructor.
	 */
	Esp32s3() noexcept;

	/**
	 * @brief Destructor.
	 */
	//! @note Added override.
	~Esp32s3() noexcept override;

	/**
	 * @brief Initialize the Uart communication
	 *
	 * @return True on success, false on failure.
	 */
	bool init() noexcept override;

	/**
	 * @brief Construct a new send string.
	 * 
	 * @param msg ??
	 */
	void send(const char *msg) noexcept override;

	/**
	 * @brief Send bytes.
	 * 
	 * @param buf ??
	 * @param bufLen ??
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
	 * @return true ??
	 * @return false
	 */
	bool isConnected() const noexcept override;

	Esp32s3(const Esp32s3 &)            = delete; // No copy constructor.
	Esp32s3(Esp32s3 &&)                 = delete; // No move constructor.
	Esp32s3 &operator=(const Esp32s3 &) = delete; // No copy assignment.
	Esp32s3 &operator=(Esp32s3 &&)      = delete; // No move assignment.

private:
	/** Buffer size. */
	static constexpr std::uint16_t BufSize{200U};

	//! @note Don't initialize members here, please do it in the constructor.
	//! All of them I mean. I fixed it for you this time. :)

	//! @note Also try to comment the member variables.
	std::uint8_t myDataMsg[BufSize];
	const char *myMsg;
	bool myDataAvailable;
	bool myConnectionStatus;
};
} // namespace driver::serial
