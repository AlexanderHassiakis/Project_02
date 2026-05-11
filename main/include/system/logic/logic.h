#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

#include <thread>
#include "esp_log.h"           // För snygg debugging (ESP_LOGI)
#include "driver/factory/interface.h"
#include "driver/serial/interface.h"
#include "driver/gpio/interface.h"
#include "driver/timer/interface.h"
#include "driver/adc/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/watchdog/interface.h"

namespace app::logic
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
			,myLed{factory.gpio(4U)}
			,myTimer{factory.timer()}
			,myAdc{factory.adc(1)}
			,myWatch{factory.watchdog(10)} 
		{
			/*Initialize hardware*/
			if(mySerial){mySerial->init();}
			if(myLed){myLed->output(false);}
			if(myTimer){myTimer->setPeriod(500U);}
			if(myAdc){myTemp = factory.tempSensor(1,*myAdc);}
			if(myWatch){myWatch->delay_ms(0U);} // Default value.
		}

		/**
		 * @brief Run 
		 * 
		 */
		
		void run()
		{
			bool isBlinking{false}; 
			char rxBuffer[256];
			int rxInd = 0;
			char menu[128];
			/*Menu for terminal commands.*/
			snprintf(menu, sizeof(menu), 	"\n-----Commands-----\n"
										"Temp\n"
										"blink\n"
										"blink off\n"
										"on = LED ON\n"
										"off = LED OFF\n"
										"status\n"
										"period x \n");
			if(mySerial) {mySerial->send(menu);}

			/**Logic Loop */
			while (true)
			{
				
				if (mySerial)
				{
					uint8_t temporaryBuf[64];
					uint16_t bytes = mySerial->received(temporaryBuf, sizeof(temporaryBuf));

					for (size_t i = 0; i < bytes; i++)
					{
						char c = static_cast<char>(temporaryBuf[i]);
						
						if(c == '\n' || c == '\r')
						{
							if(rxInd > 0)
							{
								rxBuffer[rxInd] = '\0'; // Avlsuta strängen.
								processCommand(rxBuffer, isBlinking);
								rxInd = 0;
							}
						}
						else{
							if(rxInd < (sizeof(rxBuffer) -1))
							{
								rxBuffer[rxInd++] = c;
							}
						}
					}

				/** If isBlinking & myTimer & isTimeout is true, The led will toggle on or off**/
				if (isBlinking && myTimer && myTimer->hasExpired()){myLed->toggle(); myTimer->start();} 
				/**Watchdog for Logic.**/
				if(myWatch){myWatch->delay_ms(1U);} // Watchdog delay.
			}
		}
	}

		private:

		void processCommand (char* buffer ,bool& isBlinking)
		{
			//--------- LED ON ---------//
            if (strcmp(buffer, "on") == 0)
            {
                isBlinking = false;
                myLed->output(true);
                mySerial->send("LED is constant ON!\n");
            }
            //--------- LED OFF ---------//
            else if (strcmp(buffer, "off") == 0)
            {
                isBlinking = false;
                myLed->output(false);
                mySerial->send("LED is OFF!\n");
            }
            //--------- BLINK ON ---------//
            else if (strcmp(buffer, "blink") == 0)
            {
                isBlinking = true;
                myTimer->start();
                mySerial->send("Blinking Started\n");
            }
            //--------- BLINK OFF ---------//
            else if (strcmp(buffer, "blink off") == 0)
            {
                isBlinking = false;
                myTimer->stop();
                mySerial->send("Blinking stopped\n");
            }
            //--------- READ TEMP ---------//
            else if (strcmp(buffer, "temp") == 0)
            {
                if (myTemp) {
                    char msg[48];
                    int t = myTemp->readTemperature();
                    snprintf(msg, sizeof(msg), "Temperature: %d C\n", t);
                    mySerial->send(msg);
                }
            }
            //--------- TOTAL STATUS ---------//
            else if (strcmp(buffer, "status") == 0)
            {
                char msg[128];
                int t = (myTemp) ? myTemp->readTemperature() : 0;
                const char* blinkStr = isBlinking ? "ON" : "OFF";
                int ledLevel = myLed->input();
                
                snprintf(msg, sizeof(msg), "\n----- STATUS -----\n"
                                           "Temp: %d C\n"
                                           "Blink: %s\n"
                                           "LED: %s\n",
                                           t, blinkStr, (ledLevel == 1 ? "HIGH" : "LOW"));
                mySerial->send(msg);
            }
            //--------- SET PERIOD ---------//
            else if (strncmp(buffer, "period ", 7) == 0)
            {
                int newDelay{0};
                if (sscanf(buffer + 7, "%d", &newDelay) == 1)
                {
                    if (myTimer && newDelay >= 10)
                    {
						myTimer->stop();
                        myTimer->setPeriod(static_cast<uint32_t>(newDelay));
						myTimer->start();
						ESP_LOGI("TIMER","uint value set to: %lu",static_cast<uint32_t>(newDelay));
                        char msg[48];
                        snprintf(msg, sizeof(msg), "Period set to %d ms\n", newDelay);
                        mySerial->send(msg);
                    }
                    else { mySerial->send("ERROR: Period too low!\n"); }
                }
            }
            else
            {
                mySerial->send("Unknown command: ");
                mySerial->send(buffer);
                mySerial->send("\n");
            }

		}


		/**
		 * @brief Memory cleaner/ MALLOC AUTO Typ.
		 * 
		 */
		std::unique_ptr<driver::serial::Interface> mySerial;
		std::unique_ptr<driver::gpio::Interface> myLed;
		std::unique_ptr<driver::timer::Interface> myTimer;
		std::unique_ptr<driver::tempsensor::Interface> myTemp;
		std::unique_ptr<driver::adc::Interface> myAdc;
		std::unique_ptr<driver::watchdog::Interface> myWatch;

	};
} // namespace app::logic
