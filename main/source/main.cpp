// Main file //
/**
 * @brief Stub example.
 */
#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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


  const char* helloMsg = "HELLO_SYSTEM_READY";
  uart.send(helloMsg);

  std::uint8_t rawData[] {0xDE, 0xBE, 0xEE};
  uart.send(rawData, sizeof(rawData));
  if(uart.isConnected()){
    std::printf("Status : Connected is active UART\n");
  }

  std::uint8_t rxBuffer[10];
  std::uint8_t bytesReceived = uart.received(rxBuffer, sizeof(rxBuffer));

  std::printf("Resutlat tog emot %u bytes.\n",bytesReceived);
  std::printf("Test avslutat\n");
}


int main(){

  include::driver::Stub uartStub;

  runComUartStub(uartStub);
  return 0;

  


}