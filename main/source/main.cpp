// Main file //
/**
 * @brief Stub example.
 */
#include <cstdint>

#include "driver/serial/stub.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void runComUartStub(include::driver::Interface& uart) {
  std::printf("Startar Komtest\n");

  if(!uart.init()) {
    std::printf("Fel: kundet inte initiera! UART!\n");
    return;
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