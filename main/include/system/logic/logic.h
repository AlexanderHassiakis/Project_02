
/**
 * @brief Logic driver 
 * 
 */

#pragma once
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <memory>

#include "driver/factory/interface.h"

namespace system::logic
{
	class logic final
	{
	
		public:
			explicit logic(driver::factory::Interface& factory,std::uint8_t ledpin, std::uint8_t buttonPin) noexcept
			:myLed{factory.gpio(ledPin)}
			,myButton{factory.gpio(buttonPin)}
			{}

			~logic() noexcept
			{
				myLed->write(false);
			}

			void run(const bool& stop)noexcept
			{
				bool buttonPrev{false};
				while (!stop)
				{
					const bool buttonCurrent{myButton->read()};

					if (buttonCurrent && !buttonPrev)
					{
						myLed->toggle();
					}
					buttonPrev = buttonCurrent;
				}
			}
		Logic()                        = delete;
		Logic(const Logic&)            = delete;
		Logic(Logic&&)                 = delete;
		Logic& operator=(const Logic&) = delete;
		Logic& operator=(Logic&&)      = delete;


		private:
		std::unique_ptr<driver::gpio::Interface> myLed;
		std::unique_ptr<driver::gpio::Interface> myButton;




	};
} // namespace system::logic

