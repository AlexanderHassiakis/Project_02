//! @note File header missing.
#pragma once

//! @note Sort headers.
#include "driver/mqtt/interface.h" 
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>


//! @note Bracket on the next line.
namespace driver::mqtt {

//! @note Final missing.
//! @note Class header missing.
class Stub : public Interface
{
public:
    //! @note Added alias for the callback (since the line is so long). :)
    using Callback = std::function<void(const std::string& topic, const std::string& data)>;
    
    /** Global viewable pointer to find the stub during wsl. */
    //! @note Removed inline, since that's only valid for methods.
    static Stub *latestInstance{nullptr};

    /**
     * @brief Construct a new Stub object
     */
    Stub() noexcept
        : myData{} 
        , myStoredTopic{""}
        , myConnected{false}
        , myDataCallback{nullptr}
    {
        latestInstance = this;
    }

    /**
     * @brief Destroy the Stub object
     */
    ~Stub() noexcept override 
    { 
        if (latestInstance == this)
        {
            latestInstance = nullptr;
        }
        
    }

    /**
     * @brief Start initializing the MQTT stub.
     */
    void mqttInit() noexcept override 
    {
        std::printf("[MQTT STUB] Initializing MQTT...\n");
        myConnected = true;
        std::printf("[MQTT STUB] Connected to simulated broker.\n");
    }

    /**
     * @brief Connection status to MQTT SERVER
     */
    bool isConnected() noexcept override { return myConnected; }

    /**
     * @brief Send data through MQTT SIMULATED!
     */
    void send(const std::string& topic, const std::uint8_t* bytes,
                std::uint16_t byteLen) noexcept override 
    {
        //! @note Fixed formatting, initialized i with {}.
        if (isConnected()) 
        {
            std::printf("[MQTT STUB] PUBLISH on topic '%s': ", topic.c_str());
            for (std::uint16_t i{}; i < byteLen; ++i)
            {
                std::putchar(static_cast<char>(bytes[i]));
            }
            std::printf("\n");
        } 
        else { std::printf("[MQTT STUB] SEND ERROR\n"); }
    }

    /**
     * @brief Register callbackto receive incomming messages from the broker.
     * 
     * @param cb MQTT callback.
     */
    //! @note Replaced long callback definition with its alias.
    void registerCallback(Callback cb) noexcept override 
    {
        myDataCallback = cb;
        std::printf("[MQTT STUB] Callback registered successfully.\n");
    }

    /**
     * @brief Simulate incoming message for MQTT STUB!
     */
    //! @note Added noexcept.
    void simulateIncomingMessage(const std::string &topic, const std::string &data) noexcept
    {
        //! @note Write inline comments in English, please. :)
        // Vi kollar efter latestInstance för att skicka till den stubbe som faktiskt har callbacken registrerad!
        if (latestInstance != nullptr && latestInstance->myDataCallback != nullptr)
        {
            std::printf("[MQTT STUB] Simulating incoming message on topic '%s'...\n", topic.c_str());
            latestInstance->myDataCallback(topic, data);
        } 
        // Fallback on latestInstance 
        else if (myDataCallback != nullptr)
        {
            std::printf("[MQTT STUB] Simulating incoming message on topic '%s'...\n", topic.c_str());
            myDataCallback(topic, data);
        } 
        else 
        {
            std::printf("[MQTT STUB] Cannot simulate message (No callback registered yet).\n");
        }
    }

    //! @note Very nice, but please comment them as I've done in other files.
    Stub(const Stub &) = delete;
    Stub(Stub &&) = delete;
    Stub &operator=(const Stub &) = delete;
    Stub &operator=(Stub &&) = delete;

 private:
    //! @note Use PascalCase for class constants.
    static constexpr std::uint16_t DataLen{128U};

    //! @note Removed initialization of 'myData' here (should be done in the constructor).
    std::uint8_t myData[DataLen];
    std::string myStoredTopic;
    bool myConnected;

    //! @note Replaced long callback definition with its alias.
    Callback myDataCallback;
};
} // namespace driver::mqtt
