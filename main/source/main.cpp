//ESP32 TEST code //
 #include "driver/factory/esp32s3.h"  
 #include "freertos/task.h"   
#include "system/logic/logic.h"

//extern "C" void app_main() Används för ESP IDF .
 extern "C" void app_main() {
  // Vi använder 'static' för att säkerställa att factoryn lever kvar i
  static driver::factory::Esp32s3 esp_factory;

  // Skapa applikationslogiken
  app::logic::Logic myApp(esp_factory);

  // Om din Logic::run() redan har en while(true)-loop:
  myApp.run();

  // Om run() mot förmodan skulle returnera, sätt en delay här för att hindra
  // krasch
  while (true) {
    // vTaskDelay(pdMS_TO_TICKS(1000));
  }
}


//STUB TEST CODE 
// Test WSL CODE : g++ -std=c++20 main/source/main.cpp -I main/include -o logic_test && ./logic_test
// #include <iostream>
// #include <memory>
// #include <chrono>

// #include "driver/factory/stub.h"
// #include "system/logic/logic.h"

//     int main() {
//         std::cout << "--- Startar Stub-version i WSL ---" << std::endl;


//     // Vi använder static för att säkerställa att factoryn lever kvar i minnet
//     static driver::factory::Stub esp_factory;

//     // Skapa applikationslogiken
//     app::logic::Logic myApp(esp_factory);

//     // Kör logiken
//     myApp.run();

//     while (true) {
//         // En liten paus för att inte grilla din PC-processor
//         // På PC kan vi använda std::this_thread::sleep_for om vi vill
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }


// }

