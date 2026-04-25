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
			 * @brief destructor
			 * 			Deletes to release memory
			 */
			 ~Interface() noexcept = default :

			/**
			 * @brief Initializes the Uart communication
			 *
			 */
			 void init_uart() noexcept override{

				std::printf("UART Initialized!\n")
			 }

			/**
			 * @brief sending commands/data
			 *
			 */
			 void send_data(std::uint8_t msg) noexcept override
			 {
				return 0U; //placeholder
			 }

			/**
			 * @brief recives commands/data
			 *
			 */
			 void recivedData() noexcept override
			 {
				return 0U; // placeholder
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


			std:uint8_t data[BufSize]; // Data 

			
			bool commad;
			bool connectionStatus; // status of UART connection 
        }

} // namespace include::driver::serial
	
