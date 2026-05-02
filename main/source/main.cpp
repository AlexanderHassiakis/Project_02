// Main file //
/**
 * @brief GPIO example.
 */
#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio/esp32s3.h"


driver::gpio::Esp32s3 myGpio;

constexpr std::uint8_t LED_PIN_1 = 46;
constexpr std::uint8_t LED_PIN_2 = 45;
constexpr std::uint8_t LED_PIN_3 = 0;

constexpr std::uint32_t blinkSpeed_ms{1000U};

// Blink an LED every 1000 ms GPIO Output and Toggle test.

extern "C" void app_main(void) {

  // Configure LED 1 & 2 as output.
  myGpio.output(LED_PIN_1,false);
  myGpio.output(LED_PIN_2, false);
  myGpio.output(LED_PIN_3, false);

  // Blink LED every 500 ms.
  while (1) {
    
    myGpio.toggle(LED_PIN_1);
    myGpio.toggle(LED_PIN_2);
    myGpio.toggle(LED_PIN_3);
    std::printf("Toggel leds!\n");
    vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));

    myGpio.toggle(LED_PIN_1);
    myGpio.toggle(LED_PIN_2);
    myGpio.toggle(LED_PIN_3);
    std::printf("Toggel leds!\n");
    vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));
  }
}
