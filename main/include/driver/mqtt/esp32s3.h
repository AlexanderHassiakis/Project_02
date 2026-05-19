#pragma once

#include "mqtt_client.h"
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>

#include "driver/mqtt/interface.h"

namespace driver::mqtt {

	class Esp32s3 final : public Interface 
	{

		public:
		/**
		 * @brief Destroy the interface object
		 *
		 */
		~Esp32s3() noexcept override;

		/**
		 * @brief Construct a new Esp32s3 object
		 * 
		 */
		Esp32s3() noexcept ;

		/**
		 * @brief Send data though MQTT
		 *
		 * @param bytes
		 */
		void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept;

		/**
		 * @brief Recieve data though MQTT
		 *
		 * @param bytes
		 */
		void registerCallback(std::function<void(const std::string& topic, const std::string& data)> cb) noexcept;

		/**
		 * @brief Connection status to MQTT SERVER
		 *
		 * @return true
		 * @return false
		 */
		bool isConnected() noexcept override;

		/**
		 * @brief Start initializing the MQTT.
		 *
		 */
		void mqttInit() noexcept override;

		/**Esp32s3 construct Forbidden moves/copy. **/
		Esp32s3(const Esp32s3 &)            = delete; // No copy constructor.
		Esp32s3(Esp32s3 &&)                 = delete; // No move constructor.
		Esp32s3 &operator=(const Esp32s3 &) = delete; // No copy assignment.
		Esp32s3 &operator=(Esp32s3 &&)      = delete; // No move assignment.


		private:
		bool myConnectionStatus;
		esp_mqtt_client_handle_t myClient;

		/**
		 * @brief Here we save the function that will sent back to use from logic.
		 * 
		 */
		std::function<void(const std::string&, const std::string&)> myDataCallback;

		/**
		 * @brief  Static Event Handler needed for the ESP-IDF
		 * 
		 * @param handler_args 
		 * @param base 
		 * @param event_id 
		 * @param event_data 
		 */
		static void mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data);
	};

  		

} // namespace driver::mqtt
