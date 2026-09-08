#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

#include "driver/adc/interface.h"
#include "driver/factory/interface.h"
#include "driver/gpio/interface.h"
#include "driver/mqtt/interface.h"
#include "driver/serial/interface.h"
#include "driver/tempsensor/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"
#include "driver/ai_adapt/interface.h"
// #include "esp_log.h" // if needed to debugg. Comment out of running stubs in WSL.

namespace app::logic
{
    class Logic final
    {
    public:
        /**
         * @brief Construct a new Logic object
         * * @param factory
         */
        explicit Logic(driver::factory::Interface &factory, driver::ai_adapt::Interface &linReg)
            : myRxBuffer{}, mySerial{factory.serial()}, myLed{factory.gpio(4U)},
              myTimer{factory.timer()}, myAdc{factory.adc(1)},
              myWatch{factory.watchdog()}, myMqtt(factory.mqtt()),
              myLinReg{linReg}, myInitialized{false}, myIsBlinking{false}
        {
            /*Initialize hardware at Boot*/

            // Indicate failure if any of the pointers are nullptr.
            if (mySerial && myLed && myTimer && myAdc && myWatch)
            {
                mySerial->init();
                myLed->output(false);
                myTimer->setPeriod(500U);
                myTemp = factory.tempSensor(1, *myAdc, &linReg);
                myWatch->reset();
                myInitialized = true;
                
                
                

                if (myMqtt)
                {
                    myMqtt->mqttInit();

                    std::function<void(const std::string &topic, const std::string &data)>
                        callback{[this](const std::string &topic, const std::string &data)
                                 {
                                     this->mqttCallback(topic, data);
                                 }};
                    myMqtt->registerCallback(callback);
                }
            }
            // else {ESP_LOGI("Initialize hardware", "FAILURE TO Initialize hardware!\n");}
        }

        /**
         * @brief Run
         * */
        void run()
        {
            if (!myInitialized)
            {
                return;
            }

            int rxInd = 0;
            char menu[RxLen]{};
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
            {
                std::lock_guard<std::mutex> lock(mySerialMutex);
                mySerial->send(menu);
            }
            static bool hasSentInitialTemp = false;

            /**Logic Loop */
            while (true)
            {

                /**Watchdog for Logic.**/
                myWatch->reset();

                /** If myIsBlinking & myTimer & isTimeout is true, The led will toggle on
                 * or off**/
                if (myIsBlinking && myTimer->hasExpired())
                {
                    myLed->toggle();
                    myTimer->start();
                }

                if (!hasSentInitialTemp && myMqtt && myMqtt->isConnected())
                {
                    //   ESP_LOGI("MQTT_MSG", "TEMP SENT TO BROKER!");
                    mqttTemp(true);
                    hasSentInitialTemp = true;
                }

                if (mySerial)
                {

                    uint8_t temporaryBuf[64];
                    uint16_t bytes = mySerial->received(temporaryBuf, sizeof(temporaryBuf));

                    for (size_t i = 0; i < bytes; i++)
                    {
                        char c = static_cast<char>(temporaryBuf[i]);

                        if (c == '\n' || c == '\r')
                        {
                            if (rxInd > 0)
                            {
                                myRxBuffer[rxInd] = '\0'; // ends the string.
                                processCommand(myRxBuffer);
                                rxInd = 0;
                            }
                        }
                        else
                        {
                            if (rxInd < (sizeof(myRxBuffer) - 1))
                            {
                                myRxBuffer[rxInd++] = c;
                            }
                        }
                    }
                }
            }
        }

    private:
        /**
         * @brief reads and put the data in a buffer to be read.
         *
         * @param topic
         * @param data
         */
        void mqttCallback(const std::string &topic, const std::string &data) noexcept
        {
            char mqttBuffer[RxLen]{};                                                     // Buffer needed to have more than on letter/number saved.
            size_t copyLen = (data.length() < (RxLen - 1)) ? data.length() : (RxLen - 1); // Checks data lenght to be smaller than RxLen else it will be max size Rxlen.
            memcpy(mqttBuffer, data.data(), copyLen);                                     // memory copy (minneskopiering)
                                                                                          // Its sole purpose is to copy a specified number of bytes from one location in RAM to another.
            mqttBuffer[copyLen] = '\0';
            // ESP_LOGI("MQTT_MSG", "Recived MQTT-Message: %s", mqttBuffer);
            processCommand(mqttBuffer);
        }

        /**
         * @brief Send the read temp to the MQTT Broker.
         *
         */
        void mqttTemp(bool force = false)
        {
            std::lock_guard<std::mutex> lock(myMqttMutex); // Locks the function so that we avoid Race Condition.

            if (force)
            {
                int tHel = myTemp->readTemperature();
                char tempBuffer[20];
                snprintf(tempBuffer, sizeof(tempBuffer), "%d.%d", tHel / 10, tHel % 10);
                myMqtt->send(topic, reinterpret_cast<const std::uint8_t *>(tempBuffer), strlen(tempBuffer));
                // ESP_LOGI("MQTT_MSG","Message sent to broker");
            }
        }

        /**
         * @brief All inputs and repsonses for Terminal.
         * * @param buffer
         */
        void processCommand(const char *buffer)
        {

            std::lock_guard<std::mutex> serialLock(mySerialMutex);
            //--------- LED ON ---------//
            if (strcmp(buffer, "on") == 0)
            {
                myIsBlinking = false;
                myLed->output(true);
                mySerial->send("LED is constant ON!\n");
            }
            //--------- LED OFF ---------//
            else if (strcmp(buffer, "off") == 0)
            {
                myIsBlinking = false;
                myLed->output(false);
                mySerial->send("LED is OFF!\n");
            }
            //--------- BLINK ON ---------//
            else if (strcmp(buffer, "blink") == 0)
            {
                myIsBlinking = true;

                myTimer->start();
                mySerial->send("Blinking Started\n");
            }
            //--------- BLINK OFF ---------//
            else if (strcmp(buffer, "blink off") == 0)
            {
                myIsBlinking = false;
                myLed->output(false);
                myTimer->stop();
                mySerial->send("Blinking stopped\n");
            }
            //--------- READ TEMP ---------//
            else if (strcmp(buffer, "temp") == 0)
            {
                if (myTemp)
                {
                    char msg[48];
                    int tHel = myTemp->readTemperature();
                    int t = tHel / 10;
                    int tDeci = tHel % 10;
                    snprintf(msg, sizeof(msg), "Temperature: %d.%d C\n", t, tDeci);
                    mySerial->send(msg);

                    if (myMqtt && myMqtt->isConnected())
                    {
                        //   ESP_LOGI("MQTT_MSG", "Force temp update to MQTT");
                        mqttTemp(true);
                    }
                }
            }
            //--------- TOTAL STATUS ---------//
            else if (strcmp(buffer, "status") == 0)
            {
                char msg[128];
                int tRaw = (myTemp) ? (myTemp->readTemperature()) : 0;
                int t = tRaw / 10;
                const char *blinkStr = myIsBlinking ? "ON" : "OFF";
                int ledLevel = myLed->input();
                int tDeci = tRaw % 10;

                snprintf(msg, sizeof(msg),
                         "\n----- STATUS -----\n"
                         "Temp: %d.%d C\n"
                         "Blink: %s\n"
                         "LED: %s\n",
                         t, tDeci, blinkStr, (ledLevel == 1 ? "HIGH" : "LOW"));
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
                        char msg[48];
                        snprintf(msg, sizeof(msg), "Period set to %d ms\n", newDelay);
                        mySerial->send(msg);
                    }
                    else
                    {
                        mySerial->send("ERROR: Period too low!");
                    }
                }
            }
            else
            {
                mySerial->send("Unknown command: ");
                mySerial->send(buffer);
                mySerial->send("\n");
            }
        }

        /*Memory cleaner/ MALLOC AUTO Typ.*/

        static constexpr std::uint16_t RxLen{256U};
        char myRxBuffer[RxLen];
        std::unique_ptr<driver::serial::Interface> mySerial;
        std::unique_ptr<driver::gpio::Interface> myLed;
        std::unique_ptr<driver::timer::Interface> myTimer;

        std::unique_ptr<driver::adc::Interface> myAdc;
        std::unique_ptr<driver::tempsensor::Interface> myTemp;

        std::unique_ptr<driver::watchdog::Interface> myWatch;
        std::unique_ptr<driver::mqtt::Interface> myMqtt;

        ml::lin_reg::Interface& myLinReg;

        bool myInitialized;
        bool myIsBlinking;

        std::string topic = "sensor/temp";
        std::mutex myMqttMutex;   // Protects Mqtt for race condition.
        std::mutex mySerialMutex; //  Protects serial when more than one is writing to Uart/serial at once
    };
} // namespace app::logic