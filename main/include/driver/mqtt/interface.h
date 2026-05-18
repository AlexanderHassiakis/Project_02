#pragma once


#include <cstdint>


namespace driver::mqtt
{
	class interface
	{
	
	public:
		/**
		 * @brief Destroy the interface object
		 *
		 */
		virtual ~Esp32s3() noexcept override;

		/**
		 * @brief Send data though MQTT
		 *
		 * @param bytes
		 */
		virtual void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept;

		/**
		 * @brief Recieve data though MQTT
		 *
		 * @param bytes
		 */
		virtual void registerCallback(std::functional<void(const std::string& topic, const std::string& data)> cb) noexcept;

		/**
		 * @brief Connection status to MQTT SERVER
		 *
		 * @return true
		 * @return false
		 */
		virtual bool isConnected() noexcept override;

	};
} // namespace driver::mqtt
