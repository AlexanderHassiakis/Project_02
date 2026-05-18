#pragma once

#include <cstdint>
#include <cstdio>
#include "driver/mqtt/interface.h"

namespace driver::mqtt {
    class interface {

        public:
            /**
             * @brief Destroy the interface object
             *
             */
            ~Stub() noexcept override = default;

            /**
             * @brief Construct a new interface object
             * 
             */
            Stub() noexcept override
                :dataLen{}
                ,data{nullptr}
                ,connected{false}
                ,messageWaiting{false}
                ,topic{1U};
            {}


            /**
             * @brief Start initializing the MQTT.
             *
             */
            void mqttInit() noexcept 
            {
                std::printf("initializing the MQTT")
                connected = true;
            }

            /**
             * @brief Connection status to MQTT SERVER
             *
             * @return true
             * @return false
             */
            bool isConnected() noexcept override
            {
                return connected;
            }

            /**
             * @brief Send data though MQTT
             *
             * @param bytes
             */
            void send(const std::string& topic, const std::uint8_t *bytes, std::uint16_t byteLen) noexcept override
            {
                if (isConnected)
                {
                    std::uint16_t copyLen = (byteLen < dataLen) ? byteLen : dataLen; // MAX 16 bytes
                     for (size_t i = 0; i < copyLen; i++)
                    {
                        data[i] = bytes[i];
                    }
                }
                else{std::printf("No connection try initializing again!\n")}        
            }

            /**
             * @brief Recieve data though MQTT
             *
             * @param bytes
             */
            void recieve(std::uint8_t *bytes, sizeof(data)) noexcept
            {
                if (isConnected && messageWaiting)
                {
                    for (size_t i = 0; i < data ; i++)
                    {
                        bytes*[i] = data[i];

                    }
                    
                }
                else{std::printf("No connection or no , try initializing again!\n")}

            }

            // No necessary for Stub.
            void registerCallback(std::functional<void(const std::string& topic, const std::string& data)> cb) noexcept;

            /**Stub construct Forbidden moves/copy. **/
            Stub(const Stub &) = delete;            // No copy constructor.
            Stub(Stub &&) = delete;                 // No move constructor.
            Stub &operator=(const Stub &) = delete; // No copy assignment.
            Stub &operator=(Stub &&) = delete;      // No move assignment.

          private:
            static constexpr std::uint16_t dataLen{128};
            std::uint8_t data[dataLen]{0U};
            bool connected;
            bool messageWaiting;
            std::uint8_t topic{};



    };
} // namespace driver::mqtt
