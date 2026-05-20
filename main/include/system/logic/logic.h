#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <thread>

#include "esp_log.h" // if needed to debugg.
#include "driver/adc/interface.h"
#include "driver/factory/interface.h"
#include "driver/gpio/interface.h"
#include "driver/mqtt/interface.h"
#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"

namespace app::logic {
class Logic final {
public:
  /**
   * @brief Construct a new Logic object
   * * @param factory
   */
  explicit Logic(driver::factory::Interface &factory)
      	: myRxBuffer{}
	  	, mySerial{factory.serial()}
	  	, myLed{factory.gpio(4U)}
	  	, myTimer{factory.timer()}
		, myAdc{factory.adc(1)}
		, myWatch{factory.watchdog()}
		, myMqtt(factory.mqtt())
		,myInitialized{false}
		,myIsBlinking{false} {
    /*Initialize hardware*/

    // Indicate failure if any of the pointers are nullptr.
    if (mySerial && myLed && myTimer && myAdc && myWatch) {
      mySerial->init();
      myLed->output(false);
      myTimer->setPeriod(500U);
      myTemp = factory.tempSensor(1, *myAdc);
      myWatch->reset();
      myInitialized = true;
    } else {
      //   ESP_LOGI("Initialize hardware", "FAILURE TO Initialize hardware!\n");
    }
  }

  /**
   * @brief Run
   * */
  void run() {
    if (!myInitialized) {
      return;
    }

    int rxInd = 0;
    char menu[RxLen]{};
    if (myMqtt) {
      	myMqtt->mqttInit();

	  	//Sending Temp read to MQTT
		int tHel = myTemp->readTemperature();
		char tempBuffer[20];
		//String Number Print Formatted gör om till string!.
		snprintf(tempBuffer, sizeof(tempBuffer), "%d.%d", tHel / 10, tHel % 10);
		myMqtt->send(topic,reinterpret_cast<const std::uint8_t*>(tempBuffer),strlen(tempBuffer));
		

		// Connect MQTT callback passing topic and data.
		std::function<void(const std::string &topic,const std::string &data)>callback{[this](const std::string &topic,const std::string &data) 
		{
			this->mqttCallback(topic, data);
		}};
		myMqtt->registerCallback(callback);
		ESP_LOGI("MQTT_TEST", "MQTT & WIFI WORKING!");
    }

  
    /*Menu for terminal commands.*/
    snprintf(menu, sizeof(menu),
             "\t\t\n-----Commands-----\n"
             "For temprature\t\t=\ttemp\n"
             "Start blink func\t=\tblink\n"
             "Stop blink func\t\t=\tblink off\n"
             "Turn on LED\t\t=\ton\n"
             "Turn off LED\t\t=\toff\n"
             "Status command\t\t=\tstatus\n"
             "Change blink period\t=\tperiod\n");
    mySerial->send(menu);


    /**Logic Loop */
    while (true) {
      if (mySerial) {
        uint8_t temporaryBuf[64];
        uint16_t bytes = mySerial->received(temporaryBuf, sizeof(temporaryBuf));

        for (size_t i = 0; i < bytes; i++) {
          char c = static_cast<char>(temporaryBuf[i]);

          if (c == '\n' || c == '\r') {
            if (rxInd > 0) {
              myRxBuffer[rxInd] = '\0'; // Avsluta strängen.
              processCommand(myRxBuffer);
              rxInd = 0;
            }
          } else {
            if (rxInd < (sizeof(myRxBuffer) - 1)) {
              myRxBuffer[rxInd++] = c;
            }
          }
        }
      }
      /** If myIsBlinking & myTimer & isTimeout is true, The led will toggle on or
       * off**/
      if (myIsBlinking && myTimer->hasExpired()) 
	  {myLed->toggle(); myTimer->start(); }

      /**Watchdog for Logic.**/
      myWatch->reset();
    }
  }

private:
  void mqttCallback(const std::string &topic, const std::string &data) noexcept
  {
      // Kalla på process command med mera, när du har tagit reda på vad du ska göra.
	  const char* buffer{data.c_str()};
	  processCommand(buffer);
  }

      /**
       * @brief All inputs and repsonses for Terminal.
       * * @param buffer
       */
      void processCommand(const char *buffer) {
    //--------- LED ON ---------//
    if (strcmp(buffer, "on") == 0) {
      myIsBlinking = false;
      myLed->output(true);
      mySerial->send("LED is constant ON!\n");
    }
    //--------- LED OFF ---------//
    else if (strcmp(buffer, "off") == 0) {
      myIsBlinking = false;
      myLed->output(false);
      mySerial->send("LED is OFF!\n");
    }
    //--------- BLINK ON ---------//
    else if (strcmp(buffer, "blink") == 0) {
      myIsBlinking = true;

      myTimer->start();
      mySerial->send("Blinking Started\n");
    }
    //--------- BLINK OFF ---------//
    else if (strcmp(buffer, "blink off") == 0) {
      myIsBlinking = false;
      myLed->output(false);
      myTimer->stop();
      mySerial->send("Blinking stopped\n");
    }
    //--------- READ TEMP ---------//
    else if (strcmp(buffer, "temp") == 0) {
      if (myTemp) {
        char msg[48];
        int tHel = myTemp->readTemperature();
        int t = tHel / 10;
        int tDeci = tHel % 10;
        snprintf(msg, sizeof(msg), "Temperature: %d.%d C\n", t, tDeci);
        mySerial->send(msg);
      }
    }
    //--------- TOTAL STATUS ---------//
    else if (strcmp(buffer, "status") == 0) {
      char msg[128];
      int t = (myTemp) ? (myTemp->readTemperature()) / 10 : 0;
      const char *blinkStr = myIsBlinking ? "ON" : "OFF";
      int ledLevel = myLed->input();
      int tDeci = t % 10;

      snprintf(msg, sizeof(msg),
               "\n----- STATUS -----\n"
               "Temp: %d.%d C\n"
               "Blink: %s\n"
               "LED: %s\n",
               t, tDeci, blinkStr, (ledLevel == 1 ? "HIGH" : "LOW"));
      mySerial->send(msg);
    }
    //--------- SET PERIOD ---------//
    else if (strncmp(buffer, "period ", 7) == 0) {
      int newDelay{0};
      if (sscanf(buffer + 7, "%d", &newDelay) == 1) {
        if (myTimer && newDelay >= 10) {
          myTimer->stop();
          myTimer->setPeriod(static_cast<uint32_t>(newDelay));
          myTimer->start();
          // ESP_LOGI("TIMER","uint value set to:
          // %lu",static_cast<uint32_t>(newDelay));
          char msg[48];
          snprintf(msg, sizeof(msg), "Period set to %d ms\n", newDelay);
          mySerial->send(msg);
        } else {
          mySerial->send("ERROR: Period too low!\n");
        }
      }
    } else {
      mySerial->send("Unknown command: ");
      mySerial->send(buffer);
      mySerial->send("\n");
    }
  }

  /**
   * @brief Memory cleaner/ MALLOC AUTO Typ.
   * */
  static constexpr std::uint16_t RxLen{256U};
  char myRxBuffer[RxLen];
  std::unique_ptr<driver::serial::Interface> mySerial;
  std::unique_ptr<driver::gpio::Interface> myLed;
  std::unique_ptr<driver::timer::Interface> myTimer;
  std::unique_ptr<driver::tempsensor::Interface> myTemp;
  std::unique_ptr<driver::adc::Interface> myAdc;
  std::unique_ptr<driver::watchdog::Interface> myWatch;
  std::unique_ptr<driver::mqtt::Interface> myMqtt;
  bool myInitialized;
  bool myIsBlinking;
  std::string topic = "sensor/temp";
};
} // namespace app::logic