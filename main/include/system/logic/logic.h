#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

//För ESP32 Delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/factory/interface.h"
#include "driver/serial/interface.h"
#include "driver/gpio/interface.h"
#include "driver/timer/interface.h"
#include "driver/adc/interface.h"
#include "driver/tempsensor/interface.h"

namespace system::logic
{
	class Logic final{
		public:
		/**
		 * @brief Construct a new Logic object
		 * 
		 * @param factory 
		 */
		explicit Logic(driver::factory::Interface& factory)
		{
			/*skapar driver via factory*/
			mySerial = factory.serial();
			myLed = factory.gpio(4);
			myTimer = factory.timer();
			myAdc = factory.adc();

			/*Initialisera hårdvara*/
			if(mySerial){mySerial->init();}
			if(myLed){myLed->output(false);}
			if(myTimer){myTimer->setPeriod(500);}
			if(myAdc){myTemp = factory.tempSensor(1,*myAdc);}
		}

		/**
		 * @brief Run 
		 * 
		 */
		void run()
		{
			char rxBuffer[64];
			bool isBlinking{false};
			while (true)
			{
				if (mySerial)
				{
					uint16_t bytes = mySerial->received(reinterpret_cast<uint8_t*>(rxBuffer), sizeof(rxBuffer) -1);
					if (bytes > 0)
					{
						rxBuffer[bytes] = '\0';

						rxBuffer[strcspn(rxBuffer, "\r\n")] = 0;

						if(strcmp(rxBuffer, "on") == 0)
						{
							isBlinking = false;
							myLed->output(true);
							mySerial->("Led is constant ON!\n");
						}
						else if (strcmp(rxBuffer, "off") == 0)
						{
							isBlinking = false;
							myLed->output(false);
							mySerial->send("LEd is OFF!\n");
						}
						else if (strcmp(rxBuffer, "blink") == 0)
						{
							isBlinking = true;
							myTimer->start();
							mySerial->("Blinking Started\n");
						}
						else if (strcmp(rxBuffer, "Temp") == 0)
						{
							if(myTemp){
								char msg[32];
								float t = myTemp->readTemprature();
								snprintf(msg, sizeof(msg), "Temperature: %.2f C\n", t);
								mySerial->send(msg);
							}
						}
						
						
						
					}
					
				}
				
				if (isBlinking && myTimer && myTimer->isTimeout()){myLed->toggle();}

				// Detta är "Watchdog-matningen".
        		// pdMS_TO_TICKS(10) säger till systemet: 
        		// "Jag pausar i 10 millisekunder, låt andra processer köra."
				vTaskDelay(pdMS_TO_TICKS(10)); // Delay for loopen.
			}
			

		}

		private:
		std::unique_ptr<driver::serial::Interface> mySerial;
		std::unique_ptr<driver::gpio::Interface myLed;
		std::unique_ptr<driver::timer::Interface myTimer;
		std::unique_ptr<driver::tempsensor::Interface myTemp;
		std::unique_ptr<driver::adc::Interface myADC;

	};
} // namespace system::logic
