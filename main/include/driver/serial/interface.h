#pragma once

#include <cstdint.h>

namespace include::driver::interface
{
	class Interface
	{

		public:

		/**
		 * @brief destructor
		 * 			Deletes to release memory
		 */
		virtual ~Interface() noexcept = default:


		/**
		 * @brief Initializes the Uart communication
		 * 
		 */
		virtual void init_uart() noexcept = 0;

		/**
		 * @brief sending commands/data
		 * 
		 */
		virtual void send_data() noexcept = 0;


		/**
		 * @brief recives commands/data
		 * 
		 */
		virtual void recivedData() noexcept = 0;

		/**
		 * @brief connected
		 * 
		 * @return true 
		 * @return false 
		 */
		virtual bool connected() noexcept = 0;

		/**
		 * @brief disconnected
		 * 
		 * @return true 
		 * @return false 
		 */
		virtual bool disconnected() noexcept = 0;





	}
	
} // namespace include::driver::interface
