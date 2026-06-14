//! @note File header missing.
#pragma once

//! @note Sort headers, please.
#include "mqtt_client.h"
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>

#include "driver/mqtt/interface.h"

//! @note Bracket on the next line, please.
namespace driver::mqtt
{

//! @note Class header missing.
class Esp32s3 final : public Interface 
{
	public:

	/**
	 * @brief Destroy the interface object
	 */
	~Esp32s3() noexcept override;

	/**
	 * @brief Construct a new Esp32s3 object
	 * 
	 */
	//! @note Should be placed above the constructor.
	Esp32s3() noexcept;

	/**
	 * @brief Send data though MQTT
	 *
	 * @param bytes
	 */
	//! @note Added override.
	//! @note Incorrect parameter list.
	void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept override;

	/**
	 * @brief Recieve data though MQTT
	 *
	 * @param bytes
	 */
	//! @note Added override.
	//! @note Incorrect parameter list.
	void registerCallback(std::function<void(const std::string& topic, const std::string& data)> cb) noexcept override;

	/**
	 * @brief Connection status to MQTT SERVER
	 *
	 * @return true
	 * @return false
	 */
	//! @note Return value not specified.
	bool isConnected() noexcept override;

	/**
	 * @brief Start initializing the MQTT.
	 *
	 */
	void mqttInit() noexcept override;

	Esp32s3(const Esp32s3&)            = delete; // No copy constructor.
	Esp32s3(Esp32s3&&)                 = delete; // No move constructor.
	Esp32s3& operator=(const Esp32s3&) = delete; // No copy assignment.
	Esp32s3& operator=(Esp32s3&&)      = delete; // No move assignment.


private:
	//! @note Added {} initialization.
	bool myConnectionStatus{false};
	esp_mqtt_client_handle_t myClient{nullptr};

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
	//! @note Only a short comment (as for the member variables) will do for private methods.
	//!       You can also skip comments if desired.
	static void mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data);


	/**WIFI functions*/
	void initWifi() noexcept;
	static void wifiEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
};
} // namespace driver::mqtt
