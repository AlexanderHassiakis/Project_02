//! @note Missing file header.
#pragma once

//! @note Added missing <string> include (std::string used in send/registerCallback).
#include <cstdint>
#include <functional>
#include <string>


//! @note I know that your editor messed up the indentation in the files, hence no comments
//! about that, haha! 
namespace driver::mqtt
{
//! @note Missing class header.
class Interface
{
public:
	/**
	 * @brief Destroy the interface object
	 *
	 */
	virtual ~Interface() noexcept = default;

	//! @note param list incorrect.
	/**
	 * @brief Send data though MQTT
	 *
	 * @param bytes
	 */
	virtual void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept = 0;

	//! @note param list incorrect.
	/**
	 * @brief Recieve data though MQTT
	 *
	 * @param bytes
	 */
	virtual void registerCallback(std::function<void(const std::string& topic, const std::string& data)> cb) noexcept = 0;

	//! @note return value not specified.
	/**
	 * @brief Check if the MQTT server is connected.
	 *
	 * @return true
	 * @return false
	 */
	virtual bool isConnected() noexcept = 0;

	/**
	 * @brief Initialize the MQTT.
	 * 
	 */
	virtual void mqttInit() noexcept = 0;

};
} // namespace driver::mqtt
