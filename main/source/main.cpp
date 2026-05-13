// Main file //
#include "driver/factory/esp32s3.h" 
#include "driver/factory/stub.h"
#include "freertos/task.h"
#include "system/logic/logic.h"


extern "C" void app_main() {
  // Vi använder 'static' för att säkerställa att factoryn lever kvar i minnet
  static driver::factory::Stub esp_factory;

  // Skapa applikationslogiken
  app::logic::Logic myApp(esp_factory);

  // Om din Logic::run() redan har en while(true)-loop:
  myApp.run();

  // Om run() mot förmodan skulle returnera, sätt en delay här för att hindra
  // krasch
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
