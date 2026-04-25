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
		 * @brief sending commands
		 * 
		 */
		virtual void send() noexcept = 0;


		/**
		 * @brief recives commands
		 * 
		 */
		virtual void recive() noexcept = 0;






	}
	
} // namespace include::driver::interface
