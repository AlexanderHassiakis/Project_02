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
		virtual ~interface() noexcept = 0;

		/**
		 * @brief Send data though MQTT 
		 * 
		 * @param bytes 
		 */
		virtual void send(std::uint8_t* bytes, std::uint16_t byteLen) noexcept = 0;

		/**
		 * @brief Recieve data though MQTT
		 * 
		 * @param bytes 
		 */
		virtual void recieve(std::uint8_t* bytes,std::uint16_t byteLen ) noexcept = 0;

		/**
		 * @brief Connection status to MQTT SERVER
		 * 
		 * @return true 
		 * @return false 
		 */
		virtual bool isConnected() noexcept = 0;

		/**
		 * @brief Start initializing the MQTT.
		 * 
		 */
		virtual void mqttInit() noexcept = 0;









	};
} // namespace driver::mqtt
