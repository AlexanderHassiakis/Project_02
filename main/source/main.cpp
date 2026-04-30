// Main file //
/**
 * @brief Stub example.
 */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <chrono>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

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


extern "C" void app_main(){
  ESP_LOGI("Program start","Program started!\n");
  constexpr std::uint16_t delay_loop{20};
  constexpr std::uint16_t delay_start{1000};
  constexpr std::uint16_t bufSizer{100U};



  driver::serial::Esp32s3 usbSerial;
  usbSerial.init();

  vTaskDelay(pdMS_TO_TICKS(delay_start)); // Delay  1000ms.

  usbSerial.send("TEST\r\n");

  constexpr std::size_t bufLen{bufSizer};
  std::uint8_t dataRecived[bufLen]{};
  gpio_output_enable(GPIO_NUM_1);
  gpio_output_enable(GPIO_NUM_2);

  usbSerial.send("System startat! Skriv '1' för PÅ och '0' för AV.\r\n");

  while (1)
  {

    std::uint16_t bytesRead = usbSerial.received(dataRecived, sizeof(dataRecived));
    ESP_LOGI("main", "bytes read: %u", bytesRead);

    // Parse byte if received.
    if (1U <= bytesRead)
    {
      const char byte{static_cast<char>(dataRecived[0U])};
      ESP_LOGI("main", "Received byte: %u", byte);

        if ('1' == byte) 
        {  
          gpio_set_level(GPIO_NUM_1, 1);
          gpio_set_level(GPIO_NUM_2, 1);
          usbSerial.send("LED ON!n");
        } 
        else if ('0' == byte) 
        {
          gpio_set_level(GPIO_NUM_1, 0);
          gpio_set_level(GPIO_NUM_2, 0);
          usbSerial.send("LED OFF\n");
        }
      }
    vTaskDelay(pdMS_TO_TICKS(delay_loop)); // Delay 20ms.
  }
 
}