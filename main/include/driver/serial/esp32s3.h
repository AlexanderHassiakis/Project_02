#pragma once
#include <chrono>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "driver/serial/interface.h"
#include "esp_log.h"
#include "driver/uart.h"

namespace include::driver
{
	class eps32s3 final : public Inteface
	{
	public:
		/**
		 * @brief Constructor.
		 */
		esp32s3() noexcept
			: myDataAvailable{false}, myConnectionStatus{false}, myMsg{nullptr}, myDataMsg{}
		{
		}

		/**
		 * @brief destructor
		 * 			Deletes to release memory
		 */
		~esp32s3() noexcept {}

		/**
		 * @brief Initializes the Uart communication
		 *
		 * @return True on success, false on failure.
		 *
		 */
		bool init() noexcept override
		{
			
		
			std::printf("Starts initelizing UART!\n");
			std::printf("UART Connected!\n");
			
			myConnectionStatus = true;
			return true;
		}

		/**
		 * @brief sending commands/data
		 *
		 */
		void
		send(const char *msg) noexcept override
		{ // Check if there is a connection.
			if (!myConnectionStatus)
			{
				return 0U;
			}
			// Print message if valid.
			if (nullptr != msg)
			{
				std::printf("%s Will be sent!\n", msg);
			}
			myMsg = msg;
			myDataAvailable = true;
		}

		void send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept
			override
		{ // Check if there is a connection.
			if (!myConnectionStatus)
			{
				return 0U;
			}
			// Check that the file is not to large. if to large send only the bufsize to
			// prevent crash.
			std::uint16_t copyLen = (bufLen < BufSize) ? bufLen : BufSize;

			for (size_t i = 0; i < copyLen; i++)
			{
				myDataMsg[i] = buf[i];
			}

			myDataAvailable = true;
			std::printf("[UART esp32s3] sent %u bytes.\n", copyLen);
		}

		/**
		 * @brief recives commands/
		 *
		 * @param[out] buf Buffer holding received data.
		 * @param[in] bufLen Buffer length in bytes.
		 *
		 * @return Number of received bytes.
		 *
		 */
		std::uint16_t received(std::uint8_t *buf,
							   std::uint16_t bufLen) noexcept override
		{
			// Checks if there is a msg/data available.
			if (!myDataAvailable)
			{
				return 0U;
			};
			std::printf("Message recived:\n") std::printf("%s", myMsg);
			if (buf != nullptr && bufLen > 0)
			{
				for (size_t i = 0; i < bufLen; i++)
				{
					std::printf("%u", buf[i]);
				}
			}

			myDataAvailable = false;
			return 1U;
		}

		/**
		 * @brief connected
		 *
		 * @return true
		 * @return false
		 */
		bool isConnected() const noexcept override { return myConnectionStatus; }

		/**esp32s3 construct Forbidden moves/copy. **/
		esp32s3(const esp32s3 &) = delete;			  // No copy constructor.
		esp32s3(esp32s3 &&) = delete;				  // No move constructor.
		esp32s3 &operator=(const esp32s3 &) = delete; // No copy assignment.
		esp32s3 &operator=(esp32s3 &&) = delete;	  // No move assignment.

	private:
		/** Buffer size. */
		static constexpr std::uint16_t BufSize{200U}; // Array size.
		std::uint8_t myDataMsg[BufSize]{0U};		  // Data array
		const char *myMsg;

		bool myDataAvailable;	 // Status if there is a command.
		bool myConnectionStatus; // status off the UART connection
	};

} // namespace include::driver
