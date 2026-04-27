#pragma once
#include <cstdint.h>
#include <cstdbool>
#include <cstd
#include <chrono>

namespace include::driver
{
	class stub final : public Inteface
	{
	public:
		/**
		 * @brief Constructor.
		 */
		Stub() noexcept
            : myDataAvailable{false}
          	,myConnetionStatus{false}
			,
		{
            return 0U; // placeholder
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
			myConnetionStatus{true};
		}

		/**
		 * @brief sending commands/data
		 *
		 */
		void send(const char *msg) noexcept override
		{	//Check if there is a connection.
			if (!myConnectionStatus){return 0U;}
			 // Print message if valid.
        	if (nullptr != msg) { std::printf("%s", msg,"Will be sent!\n"); }
			myDataAvailable{true};
			}

		void send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept override
		{	//Check if there is a connection.
			if (!myConnectionStatus){return 0U;}
			myDataAvailable{true};
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
			if (!myDataAvailable) { return 0U};
			std::printf("Message recived:\n")
			std::printf("%s",msg);
			myDataAvailable{false};
		
		}

		/**
		 * @brief connected
		 *
		 * @return true
		 * @return false
		 */
		bool isConnected() const noexcept override
		{
			return myConnetionStatus;
		}

		/**Stub construct Forbidden moves/copy. **/
		Stub(const Stub &) = delete;			// No copy constructor.
		Stub(Stub &&) = delete;					// No move constructor.
		Stub &operator=(const Stub &) = delete; // No copy assignment.
		Stub &operator=(Stub &&) = delete;		// No move assignment.

	private:
		/** Buffer size. */
		static constexpr std::uint16_t BufSize{100U};
		std::uint8_t myDataMsg[BufSize]; // Data array
		std::char* msg;

		bool myDataAvailable;	// Status if there is a command.
		bool myConnetionStatus; // status off the UART connection
	}

} // namespace include::driver::serial
