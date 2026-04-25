#pragma once
#include <cstdint.h>
#include <cstdbool>
#include <cstd
#include <chrono>


namespace include::driver
{
	class stub final: public Inteface
	{
        public:
			/**
			 * @brief Constructor.
			 */
			Stub() noexcept;

			/**
			 * @brief destructor
			 * 			Deletes to release memory
			 */
			~Stub() noexcept = default :

			/**
			 * @brief Initializes the Uart communication
			 *
			 */
			void init_uart() noexcept override {

            std::printf("UART Initialized!\n")
          	}

			/**
			 * @brief sending commands/data
			 *
			 */
			 void send_data(std::uint8_t msg*) noexcept override
			{
				return 0U; //placeholder

			}

			/**
			 * @brief Sends commands
			 * 
			 * @param [in] command to be sent via uart.
			 */
			void sendCommand(std::uint8_t command) noexcept
			{
				if (command == 0u;){return 0U;} // checks the command to see that its something there.
				
				if (!connectionStatus) {return 0U;} // check to see if the connection is up. 

				return 0u; // placeholder
			}

			/**
			 * @brief recives commands/data
			 * 			
			 *
			 */
			void recivedData() noexcept override
			 {
				
				return 0U; // placeholder
			 }

			 /**
			  * @brief recivde command.
			  * 
			  * 	The device will responed on the command sent to it.
			  */
			 void reciveCommand() noexcept
			 {

				if (!availableCommand) {return 0U;} // Check if there is even a message/command sent.

				return 0U; // Placeholder
                             
                           
			}

			/**
			 * @brief connected
			 *
			 * @return true
			 * @return false
			 */
			 bool connected() noexcept override
			 {
				std::printf("Connected!\n")
			 }

			/**
			 * @brief disconnected
			 *
			 * @return true
			 * @return false
			 */
			bool disconnected() noexcept override
			{

				std::printf("Disconnected!\n")
			}

			/**Stub construct Forbidden moves/copy. **/
			Stub(const Stub&) 				= delete; // No copy constructor.
			Stub(Stub &&) 					= delete; // No move constructor.
			Stub &operator=(const Stub &)	= delete; // No copy assignment.
			Stub &operator=(Stub &&) 		= delete; // No move assignment.

			private:

			 /** Buffer size. */
    		static constexpr std::uint8_t BufSize{100U};


			std:uint8_t data[BufSize]; // Data array 


			bool availableCommand; // Status if there is a command.
			bool connectionStatus; // status off the UART connection 
        }

} // namespace include::driver::serial
	
