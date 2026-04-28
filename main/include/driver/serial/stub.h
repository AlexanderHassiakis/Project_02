#pragma once
#include <cstdint>
#include <cstdbool>
#include <cstdio>
#include <chrono>

namespace include::driver
{
	class Stub final : public Interface
	{
	public:
		/**
		 * @brief Constructor.
		 */
		Stub() noexcept
            : myDataAvailable{false}
          	,myConnectionStatus{false}
			,myMsg{nullptr}
			,myDataMsg{}
		{
            
		}

		/**
		 * @brief destructor
		 * 			Deletes to release memory
		 */
		~Stub() noexcept
		{	
		}

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
			if (!myConnectionStatus){return 0U;}
			 // Print message if valid.
        	if (nullptr != msg) { std::printf("%s Will be sent!\n", msg); }
			myMsg = msg;
			myDataAvailable = true;
		}

		void send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept override
		{	//Check if there is a connection.
			if (!myConnectionStatus){return 0U;}
			//Check that the file is not to large. if to large send only the bufsize to prevent crash.
			std::uint16_t copyLen = (bufLen < BufSize) ? bufLen : BufSize;

			for (size_t i = 0; i < copyLen; i++)
			{
				myDataMsg[i] = buf[i];
            }

			myDataAvailable = true;
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
			//Checks if there is a msg/data available.
			if (!myDataAvailable) { return 0U;};
			std::printf("Message recived:\n")
			std::printf("%s",myMsg);
			if ( buf != nullptr && bufLen > 0)
			{
				for (size_t i = 0; i < bufLen; i++)
				{
					std::printf("%u",buf[i]);
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
		bool isConnected() const noexcept override
		{
			return myConnectionStatus;
		}

		/**Stub construct Forbidden moves/copy. **/
		Stub(const Stub &) = delete;			// No copy constructor.
		Stub(Stub &&) = delete;					// No move constructor.
		Stub &operator=(const Stub &) = delete; // No copy assignment.
		Stub &operator=(Stub &&) = delete;		// No move assignment.

	private:
		/** Buffer size. */
		static constexpr std::uint16_t BufSize{200U}; // Array size.
		std::uint8_t myDataMsg[BufSize]{0U}; // Data array
		const char* myMsg;

		bool myDataAvailable;	// Status if there is a command.
		bool myConnectionStatus; // status off the UART connection
	};

} // namespace include::driver::serial
