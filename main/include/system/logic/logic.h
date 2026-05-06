#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>



#include "driver/factory/interface.h"
#include "driver/serial/interface.h"
#include "driver/gpio/interface.h"
#include "driver/timer/interface.h"
#include "driver/adc/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/watchdog/interface.h"

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
			: mySerial{factory.serial()}
			,myLed{factory.gpio(4)}
			,myTimer{factory.timer()}
			,myAdc{factory.adc()}
			,myWatch{factory.watchdog()} 
		{
			/*Initialisera hårdvara*/
			if(mySerial){mySerial->init();}
			if(myLed){myLed->output(false);}
			if(myTimer){myTimer->setPeriod(500);}
			if(myAdc){myTemp = factory.tempSensor(1,*myAdc);}
			if(myWatch){myWatch->delay_ms(0);} // Default value.
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

						rxBuffer[strcspn(rxBuffer, "\r\n")] = 0; // Letar efter /r /n i buffer och ersätter med en nolla.
						//---------LED ON---------//
						if(strcmp(rxBuffer,"on") == 0)
						{
							isBlinking = false;
							myLed->output(true);
							mySerial->send("Led is constant ON!\n");
						}
						//---------LED OFF---------//
						else if (strcmp(rxBuffer,"off") == 0)
						{
							isBlinking = false;
							myLed->output(false);
							mySerial->send("LED is OFF!");
						}
						//---------LED BLINK ON---------//
						else if (strcmp(rxBuffer,"blink") == 0)
						{
							isBlinking = true;
							myTimer->start();
							mySerial->send("Blinking Started\n");
						}
						//---------READ TEMP---------//
						else if (strcmp(rxBuffer,"temp") == 0)
						{
							if(myTemp){
								char msg[32];
								int t = myTemp->readTemperature();
								snprintf(msg, sizeof(msg), "Temperature: %d C\n", t);
								mySerial->send(msg);
							}
						}
						//---------LED BLINK OFF---------//
						else if (strcmp(rxBuffer,"blink off") == 0)
						{
							isBlinking = false;
							myTimer->stop();
							mySerial->send("Blinking stopped\n");
						}
						//---------TOTAL STATUS---------//
						else if (strcmp(rxBuffer,"status") == 0)
						{
							char msg[128];
							int t = static_cast<int>(myTemp->readTemperature());
							const char* binkStr = isBlinking ? "ON" : "OFF";
							snprintf(msg, sizeof(msg), 	"\n-----STATUS-----\n "
														"Temperature: %d C\n"
														"Blink mode: %s\n",
														t,blinkStr);
							mySerial->send(msg);
							int ledLevel = myLed->input(); // When output is true, input will act as a gpio status provider.
							const char* ledStr =(ledLevel == 1) ? "High" : "Low";
							snprintf(msg,sizeof(msg), "LED Level: %s\n", ledStr);
							mySerial->send(msg);
						}
						//---------DELAY COMMAND PARSING OF VALUE---------//
						else if (strncmp(rxBuffer,"period", 7) == 0)
						{
							int newDelay{0};
							if (sscanf(rxBuffer + 7,"%d", &newDelay) == 1)
							{
								if(myTimer)
								{
									myTimer->setPeriod(newDelay);
									char msg[32];
									snprintf(msg,sizeof(msg),"Delay set to %d ms\n",newDelay);
									mySerial->send(msg);
								}
							}
						}
					}
					
				}
				
				if (isBlinking && myTimer && myTimer->isTimeout()){myLed->toggle();}
				if(myWatch){myWatch->delay_ms(10);} // Watchdog delay.
			}
		}

		private:
		std::unique_ptr<driver::serial::Interface> mySerial;
		std::unique_ptr<driver::gpio::Interface> myLed;
		std::unique_ptr<driver::timer::Interface> myTimer;
		std::unique_ptr<driver::tempsensor::Interface> myTemp;
		std::unique_ptr<driver::adc::Interface> myAdc;
		std::unique_ptr<driver::watchdog::Interface> myWatch;

	};
} // namespace system::logic
