// Main file //
/**
 * @brief GPIO example.
 */
#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio/esp32s3.h"


driver::gpio::Esp32s3 myGpio;

constexpr std::uint8_t LED_PIN_1 = 1;
constexpr std::uint8_t LED_PIN_2 = 2;

constexpr std::uint32_t blinkSpeed_ms{500U};

// Blink an LED every 500 ms.

extern "C" void app_main(void) {

  // Configure LED 1 & 2 as output.
  myGpio.output(LED_PIN_1,true);
  myGpio.output(LED_PIN_2,false);

  

  

  // Blink LED every 500 ms.
  while (1) {
    
    myGpio.toggle(LED_PIN_1);
    myGpio.toggle(LED_PIN_2);
    std::printf("Toggel leds!\n");
    vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));

    myGpio.toggle(LED_PIN_1);
    myGpio.toggle(LED_PIN_2);
    std::printf("Toggel leds!\n")
    vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));
  }
}
