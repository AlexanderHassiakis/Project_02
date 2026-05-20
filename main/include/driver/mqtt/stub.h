#pragma once

#include "driver/mqtt/interface.h" // Antar att ditt interface heter Interface med stort I
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>


namespace driver::mqtt {

// Klassen måste ärva från ditt Interface (t.ex. public Interface)
class Stub : public Interface {
public:
  /**
   * @brief Construct a new Stub object
   */
  Stub() noexcept
      : myConnected{false}, myData{}, myStoredTopic{""},
        myDataCallback{nullptr} {}

  /**
   * @brief Destroy the Stub object
   */
  ~Stub() noexcept override = default;

  /**
   * @brief Start initializing the MQTT stub.
   */
  void mqttInit() noexcept override {
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
  void send(const std::string &topic, const std::uint8_t *bytes,
            std::uint16_t byteLen) noexcept override {
    if (isConnected()) {
      std::printf("[MQTT STUB] PUBLISH on topic '%s': ", topic.c_str());
      for (std::uint16_t i = 0; i < byteLen; ++i) {
        std::putchar(static_cast<char>(bytes[i]));
    }
    std::printf("\n");} 
    else {std::printf("[MQTT STUB] SEND ERROR\n");}
  }

  /**
   * @brief RegisterCallback functions as a recive function 
   *        for incomming messages from the broker.
   */
  void registerCallback(std::function<void(const std::string &topic, const std::string &data)>cb) noexcept override {
    myDataCallback = cb;
    std::printf("[MQTT STUB] Callback registered successfully.\n");
  }


  /**
   * @brief Simualte incoming message for MQTT STUB!
   */
  void simulateIncomingMessage(const std::string &topic,
                               const std::string &data) {
    if (myConnected && myDataCallback != nullptr) {
      std::printf("[MQTT STUB] Simulating incoming message on topic '%s'...\n",
                  topic.c_str());
      myDataCallback(topic, data);
    } else {
      std::printf("[MQTT STUB] Cannot simulate message (disconnected).\n");
    }
  }


  Stub(const Stub &) = delete;
  Stub(Stub &&) = delete;
  Stub &operator=(const Stub &) = delete;
  Stub &operator=(Stub &&) = delete;

private:
  static constexpr std::uint16_t dataLen{128};
  std::uint8_t myData[dataLen]{0U};
  std::string myStoredTopic;
  bool myConnected;

  std::function<void(const std::string &topic, const std::string &data)>myDataCallback;
};

} // namespace driver::mqtt