// Main file //
/**
 * @brief Stub example.
 */
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>


#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "driver/serial/esp32s3.h"

#include "driver/gpio/direction.h"
#include "driver/gpio/esp32s3.h"

using namespace driver;

// Blink an LED every 1000 ms GPIO Output and Toggle test.

extern "C" void app_main(void) {

    constexpr std::uint8_t LED_PIN_1 = 46;
    constexpr std::uint8_t LED_PIN_2 = 45;
    constexpr std::uint8_t LED_PIN_3 = 0;

    constexpr std::uint32_t blinkSpeed_ms{1000U};

    gpio::Esp32s3 led1{LED_PIN_1, gpio::Direction::Output};
    gpio::Esp32s3 led2{LED_PIN_2, gpio::Direction::Output};
    gpio::Esp32s3 led3{LED_PIN_3, gpio::Direction::Output};

  // Blink LED every 500 ms.
  while (1) {

    led1.toggle();
    led2.toggle();
    led3.toggle();

    std::printf("Toggle leds!\n");
    vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));
  }
}
