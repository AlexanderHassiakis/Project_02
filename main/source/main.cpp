// Use real drivers when STUB is not defined.
#ifndef STUB

#include "esp_log.h"
#include "driver/factory/esp32s3.h"
#include "freertos/task.h"
#include "system/logic/logic.h"

// extern "C" void app_main() Används för ESP IDF .
extern "C" void app_main()
{
    // Vi använder 'static' för att säkerställa att factoryn lever kvar i
    static driver::factory::Esp32s3 esp_factory;

    // Skapa applikationslogiken
    app::logic::Logic myApp(esp_factory);

    // Om din Logic::run() redan har en while(true)-loop:
    myApp.run();

    // Om run() mot förmodan skulle returnera, sätt en delay här för att hindra krasch.
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); // förhindar att system startar om.
        ESP_LOGI("ERROR", "LOGIC RUN HAS CRASHED!");
    }
}

// STUB CODE WSL no hardware needed
#else
//-------------------------------------------------------------------------------------------------------------------
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "driver/factory/stub.h"
#include "driver/mqtt/stub.h"
#include "system/logic/logic.h"

int main() {
  std::cout << "--- Startar Stub-version i WSL ---" << std::endl;
  std::cout << "Skriv kommandon (on, off, blink, status, temp) och tryck
  Enter:"
            << std::endl;
  std::cout << "--------------------------------------------------------"
            << std::endl;

  // 1. Skapa en fast, global instans av MQTT-stubben direkt i main-scopes
  // minne. Detta garanterar att latestInstance sätts DIREKT här och aldrig
  blir
  // nullptr!
  static driver::mqtt::Stub lokalMqttStub;

  // Skapa fabriken och applikationslogiken
  static driver::factory::Stub esp_factory;
  static app::logic::Logic myApp(esp_factory);

  // Starta bakgrundstråden.
  // kompilatorvarningen!
  std::thread appThread([]() { myApp.run(); });

  // Huvudloop för terminalinmatning
  std::string inputLine;
  while (std::getline(std::cin, inputLine)) {

    if (inputLine.empty()) {
      continue;
    }

    if (inputLine == "exit" || inputLine == "quit") {
      std::cout << "Avslutar WSL-simulering..." << std::endl;
      break;
    }

    // Vi använder vår garanterade lokala instans istället för att leta i
    minnet if (driver::mqtt::Stub::latestInstance != nullptr) {
      driver::mqtt::Stub::latestInstance->simulateIncomingMessage(
          "wsl/terminal", inputLine);
    } else {
      // fallback
      lokalMqttStub.simulateIncomingMessage("wsl/terminal", inputLine);
    }z
  }

  if (appThread.joinable()) {
    appThread.detach();
  }

  return 0;
}
#endif // STUB