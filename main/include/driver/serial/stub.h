#pragma once

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring> 
#include "driver/serial/interface.h"

namespace driver::serial
{
class Stub final : public Interface
{
public:
		/**
		 * @brief Constructor.
		 */
		Stub() noexcept
			:myDataMsg{}
			,myMsg{nullptr}
			,myDataAvailable{false}
			,myConnectionStatus{false}
		{}

		/**
		 * @brief destructor
		 * 			Deletes to release memory
		 */
		~Stub() noexcept = default;

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
		void send(const char *msg) noexcept override
		{	//Check if there is a connection.
			if (!myConnectionStatus){return;}
				// Print message if valid.
			if (nullptr != msg) { std::printf("%s Will be sent!\n", msg); }
			myMsg = msg;

		}

		void send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept override
		{	//Check if there is a connection.
			if (!myConnectionStatus){return;}
			//Check that the file is not to large. if to large send only the bufsize to prevent crash.
			std::uint16_t copyLen = (bufLen < BufSize) ? bufLen : BufSize;

			for (size_t i = 0; i < copyLen; i++)
			{
				myDataMsg[i] = buf[i];
			}
			std::printf("[UART Stub] sent %u bytes.\n",copyLen);

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
		std::uint16_t received(std::uint8_t *buf, std::uint16_t bufLen) noexcept override
		{
			(void)buf;    
            (void)bufLen; 
            return 0U;
		}
		
		/**
		 * @brief recives commands/
		 *
		 * @param[out] str Buffer holding received data.
		 * @param[in] bufLen Buffer length in bytes.
		 *
		 * @return Number of received bytes.
		 *
		 */
		std::uint16_t received(const char *str, std::uint16_t strLen) noexcept override
		{
			(void)str;
            (void)strLen;
            return 0U;
		}

		/**
		 * @brief connected
		 *
		 * @return true
		 * @return false
		 */
		bool isConnected() const noexcept override {
		return myConnectionStatus;
		}

		/**Stub construct Forbidden moves/copy. **/
		Stub(const Stub &) = delete;            // No copy constructor.
		Stub(Stub &&) = delete;                 // No move constructor.
		Stub &operator=(const Stub &) = delete; // No copy assignment.
		Stub &operator=(Stub &&) = delete;      // No move assignment.

        private:
          /** Buffer size. */
          static constexpr std::uint16_t BufSize{200U}; // Array size.
          std::uint8_t myDataMsg[BufSize]{0U};          // Data array
          const char *myMsg;

          bool myDataAvailable;    // Status if there is a command.
          bool myConnectionStatus; // status off the UART connection
        };
} // namespace driver::serial::serial
