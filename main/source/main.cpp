// // Main file //
// // #include "driver/factory/esp32s3.h"  Disable when running stub!!
// #include "driver/factory/stub.h"
// // #include "freertos/task.h"   Disable when running stub!!
// #include "system/logic/logic.h"


// extern "C" void app_main() {
//   // Vi använder 'static' för att säkerställa att factoryn lever kvar i minnet
//   static driver::factory::Stub esp_factory;

//   // Skapa applikationslogiken
//   app::logic::Logic myApp(esp_factory);

//   // Om din Logic::run() redan har en while(true)-loop:
//   myApp.run();

//   // Om run() mot förmodan skulle returnera, sätt en delay här för att hindra
//   // krasch
//   while (true) {
//     // vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }


#include <iostream>
#include <memory>
#include <chrono>

// #include "driver/factory/esp32s3.h" // Aktivera för ESP32
#include "driver/factory/stub.h"
#include "system/logic/logic.h"

// Om vi INTE är på ESP32, definiera app_main som main
#ifndef ESP_PLATFORM
    int main() {
        std::cout << "--- Startar Stub-version i WSL ---" << std::endl;
#else
    extern "C" void app_main() {
#endif

    // Vi använder static för att säkerställa att factoryn lever kvar i minnet
    static driver::factory::Stub esp_factory;

    // Skapa applikationslogiken
    app::logic::Logic myApp(esp_factory);

    // Kör logiken
    myApp.run();

    while (true) {
        // En liten paus för att inte grilla din PC-processor
        // På PC kan vi använda std::this_thread::sleep_for om vi vill
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

#ifndef ESP_PLATFORM
    return 0; // main() på PC måste returnera ett värde
}
#else
}
#endif