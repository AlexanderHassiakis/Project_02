#include <cstdint>
#include <cstdio>
#include <cstring>

#include "driver/gpio.h"       // För att styra pinnar
#include "driver/uart.h"       // Grundläggande UART-funktioner
#include "esp_log.h"           // För snygg debugging (ESP_LOGI)
#include "freertos/FreeRTOS.h" // Krävs för task-hantering och köer
#include "freertos/queue.h"    // För UART-events (avbrott)

#include "driver/serial/esp32s3.h"

namespace driver::serial {

Esp32s3::Esp32s3() noexcept
    : myDataAvailable{false}, myConnectionStatus{false}, myMsg{nullptr},
      myDataMsg{} {}

Esp32s3::~Esp32s3() noexcept {}

bool Esp32s3::init() noexcept {

  /**UART Config*/
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE.source_clk = UART_SCLK_DEFAULT,
  };
  uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  const int rx_buffer_size = 1024;
  const int tx_buffer_size = 1024; // Kan sättas till 0 om du vill att
                                   // sändning ska blockera tills klart
  QueueHandle_t uart_queue; // Används om du vill ha FreeRTOS-events (sätt
                            // till NULL om du pollar)
  uart_driver_install(UART_NUM_1, rx_buffer_size, tx_buffer_size, 10,
                      &uart_queue, 0);
  myConnectionStatus = true;
  return true;
}

void Esp32s3::send(const char *msg) noexcept { // Check if there is a connection.
  if (!myConnectionStatus) {
    return;
  }

  // Print message if valid.
  if (nullptr != msg) {
    std::printf("%s Will be sent!\n", msg);
    // Skickar datan till TX-bufferten
    uart_write_bytes(UART_NUM_1, msg, strlen(msg));
  }
}

void Esp32s3::send(const std::uint8_t *buf, std::uint16_t bufLen) noexceptoverride { // Check if there is a connection.
  if (!myConnectionStatus) {
    return;
  }

  // Check that the file is not to large. if to large send only the bufsize to
  // prevent crash.
  std::uint16_t copyLen = (bufLen < BufSize) ? bufLen : BufSize;

  // Writes to UART TX-buffer
  uart_write_bytes(UART_NUM_1, (const void *)buf, copyLen);
  // Prints how many bites is sent.
  std::printf("[UART Esp32s3] sent %u bytes.\n", copyLen);
}

std::uint16_t Esp32s3::received(std::uint8_t *buf,
                                     std::uint16_t bufLen) noexcept {
  constexpr std::uint16_t delay_ms{20U};

  // Checks if there is a msg/data available.
  if (!myDataAvailable) {
    return 0U;
  }

  // Lokala variabeln bytesRead av typen int, eftersom ESP-IDF returnerar int
  int bytesRead = 0;

  if (buf != nullptr && bufLen > 0) {
    // PORT 17 GPIO UART_NUM_1 och en timeout på 20 millisekunder
    bytesRead = uart_read_bytes(UART_NUM_1, buf, bufLen, pdMS_TO_TICKS(delay_ms));
  }

  // Hantera fel (om uart_read_bytes returnerar -1)
  if (bytesRead < 0) {
    bytesRead = 0;
  } else if (bytesRead > 0) {
    // Skriv bara ut att vi fått data om vi FAKTISKT fick in data.
    std::printf("Message received: %d bytes\n", bytesRead);
  }

  // Returnera säkert som uint16_t
  return static_cast<std::uint16_t>(bytesRead);
}

bool Esp32s3::isConnected() const noexcept
{

}

} // namespace driver::serial
