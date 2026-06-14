//! @note File header missing.
//! @note #pragma once missing.

//! @note Sort headers.
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
// #include "esp_log.h" // if needed to debugg. Comment out of running stubs in WSL.

//! @note I strongly recommend that the method definitions are moved to a .cpp file.
//!       This class is impressive, but the header is very large and complex.
//!       An implementation file would ensure that this file could be "clean".
namespace app::logic
{
//! @note Class header missing.
class Logic final
{
public:
    /**
     * @brief Construct a new Logic object
     * * @param factory
     */
    explicit Logic(driver::factory::Interface &factory)
        //! @note Initialize with {} instead of ().
        : myRxBuffer{}, 
        , mySerial{factory.serial()}
        , myLed{factory.gpio(4U)}
        , myTimer{factory.timer()}
        , myAdc{factory.adc(1)},
        , myWatch{factory.watchdog()}
        , myMqtt(factory.mqtt()),
        , myInitialized{false}
        , myIsBlinking{false}
    {
        /*Initialize hardware at Boot*/
        // Indicate failure if any of the pointers are nullptr.
        if (mySerial && myLed && myTimer && myAdc && myWatch)
        {
            //! @note Avoid magic numbers if youo can.
            mySerial->init();
            myLed->output(false);
            myTimer->setPeriod(500U);
            myTemp = factory.tempSensor(1, *myAdc);
            myWatch->reset();
            myInitialized = true;

            if (myMqtt)
            {
                myMqtt->mqttInit();

                //! @note I love lambdas (not), haha! Good work. :)
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
    //! @note noexcept?
    void run()
    {
        if (!myInitialized) { return; }

        //! @note Use {} initialization instead of =.
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
                //! @note Use std::uintx and std::size_t.
                //! @note Initialize with {}.
                //! @note Also try to avoid magic numbers.
                std::uint8_t temporaryBuf[64U]{};
                const auto bytes{mySerial->received(temporaryBuf, sizeof(temporaryBuf))};

                for (std::size_t i{0U}; i < bytes; ++i)
                {
                    const auto c{static_cast<char>(temporaryBuf[i])};

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
        char mqttBuffer[RxLen]{};
        //! @note Use std::size_t and std::memcpy.
        const std::size_t copyLen{(data.length() < (RxLen - 1)) ? data.length() : (RxLen - 1)};
        memcpy(mqttBuffer, data.data(), copyLen);
        mqttBuffer[copyLen] = '\0';
        // ESP_LOGI("MQTT_MSG", "Recived MQTT-Message: %s", mqttBuffer);
        processCommand(mqttBuffer);
    }

    /**
     * @brief Send the read temp to the MQTT Broker.
     *
     */
    //! @note noexcept?
    void mqttTemp(bool force = false)
    {
        std::lock_guard<std::mutex> lock(myMqttMutex); // Locks the function so that we avoid Race Condition.

        if (force)
        {
            //! @note Use {} initialization instead of =. Initialize tempBuffer[].
            const int tHel{myTemp->readTemperature()};
            char tempBuffer[20]{};
            snprintf(tempBuffer, sizeof(tempBuffer), "%d.%d", tHel / 10, tHel % 10);
            myMqtt->send(topic, reinterpret_cast<const std::uint8_t *>(tempBuffer), strlen(tempBuffer));
            // ESP_LOGI("MQTT_MSG","Message sent to broker");
        }
    }

    /**
     * @brief All inputs and repsonses for Terminal.
     * * @param buffer
     */
    //! @note noexcept?
    void processCommand(const char *buffer)
    {

        std::lock_guard<std::mutex> serialLock(mySerialMutex);
        //--------- LED ON ---------//
        //! @note Yoda notation?
        //! @note Consider adding a helper method like this:
        //! bool strMatch(str1, str2) { return 0 == std::strcmp(str1, str2); } =>
        //! then you can write if (strMatch(buffer, "on")) instead of this.
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
                //! @note Use {} initialization instead of =.
                char msg[48]{};
                int tHel = myTemp->readTemperature();
                int t = tHel / 10;
                int tDeci = tHel % 10;
                std::snprintf(msg, sizeof(msg), "Temperature: %d.%d C\n", t, tDeci);
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
                    myTimer->setPeriod(static_cast<std::uint32_t>(newDelay));
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
    //! @note Added {} initialization.
    char myRxBuffer[RxLen]{};
    std::unique_ptr<driver::serial::Interface> mySerial;
    std::unique_ptr<driver::gpio::Interface> myLed;
    std::unique_ptr<driver::timer::Interface> myTimer;

    std::unique_ptr<driver::adc::Interface> myAdc;
    std::unique_ptr<driver::tempsensor::Interface> myTemp;

    std::unique_ptr<driver::watchdog::Interface> myWatch;
    std::unique_ptr<driver::mqtt::Interface> myMqtt;
    bool myInitialized;
    bool myIsBlinking;

    //! @note Can be made const.
    std::string topic = "sensor/temp";
    std::mutex myMqttMutex;   // Protects Mqtt for race condition.
    std::mutex mySerialMutex; //  Protects serial when more than one is writing to Uart/serial at once
};
} // namespace app::logic