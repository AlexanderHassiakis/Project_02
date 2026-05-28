#pragma once


#include <cstdint>
#include <functional>


namespace driver::mqtt
{
	class Interface
	{
	
	public:
		/**
		 * @brief Destroy the interface object
		 *
		 */
		virtual ~Interface() noexcept = default;

		/**
		 * @brief Send data though MQTT
		 *
		 * @param bytes
		 */
		virtual void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept = 0;

		/**
		 * @brief Recieve data though MQTT
		 *
		 * @param bytes
		 */
		virtual void registerCallback(std::function<void(const std::string& topic, const std::string& data)> cb) noexcept = 0;

		/**
		 * @brief Connection status to MQTT SERVER
		 *
		 * @return true
		 * @return false
		 */
		virtual bool isConnected() noexcept = 0;

		/**
		 * @brief Initializes the MQTT.
		 * 
		 */
		virtual void mqttInit() noexcept = 0;

	};
} // namespace driver::mqtt
