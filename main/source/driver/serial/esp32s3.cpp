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
       {}

Esp32s3::~Esp32s3() noexcept {
  if (myConnectionStatus) {
    uart_driver_delete(UART_NUM_0);
  }

bool Esp32s3::init() noexcept {
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };

  // 1. Konfigurera parametrar först
  if (uart_param_config(UART_NUM_0, &uart_config) != ESP_OK) return false;

  // 2. Sätt pinnarna (TX=17, RX=16) och ifall den misslyckas så kastar den return false.
  if (uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK)
    return false;

  // 3. Installera drivern
  const int rx_buffer_size = 1024;
  const int tx_buffer_size = 1024;

  if (uart_driver_install(UART_NUM_0, rx_buffer_size, tx_buffer_size, 0, NULL, 0) != ESP_OK) return false;

  myConnectionStatus = true;
  return true;
}

void Esp32s3::send(const char *msg) noexcept { // Check if there is a connection.
  if (!myConnectionStatus || msg == nullptr) {
    return;
  }
    // Skickar datan till TX-bufferten/ UART_NUM_0
    uart_write_bytes(UART_NUM_0, msg, strlen(msg));
  
}

void Esp32s3::send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept { // Check if there is a connection.
  if (!myConnectionStatus || buf == nullptr) {return;}

  // Check that the file is not to large. if to large send only the bufsize to
  // prevent crash.
  std::uint16_t copyLen = (bufLen < BufSize) ? bufLen : BufSize;

  // Writes to UART TX-buffer / UART_NUM_0
  uart_write_bytes(UART_NUM_0, (const void *)buf, copyLen);
}

std::uint16_t Esp32s3::received(std::uint8_t *buf, std::uint16_t bufLen) noexcept {
  constexpr std::uint16_t delay_ms{20U};

  // Checks if there is a msg/data available.
  if (!myConnectionStatus || buf == nullptr || bufLen == 0) return 0U;


  size_t available_data = 0;
  uart_get_buffered_data_len(UART_NUM_0, &available_data);
  if (available_data == 0) {return 0U;}

  // Lokala variabeln bytesRead av typen int, eftersom ESP-IDF returnerar int
  int bytesRead = uart_read_bytes(UART_NUM_0, buf, bufLen, pdMS_TO_TICKS(20));

  if (buf != nullptr && bufLen > 0) {
    // PORT 17 GPIO UART_NUM_1 och en timeout på 20 millisekunder
    bytesRead = uart_read_bytes(UART_NUM_1, buf, bufLen, pdMS_TO_TICKS(delay_ms));
  }

  // Läs datan från UART_NUM_0
  int bytesRead = uart_read_bytes(UART_NUM_0, buf, bufLen, pdMS_TO_TICKS(20));
  // Check data if 0 returns 0U.
  if (bytesRead < 0){return 0U;}

  // Returnera säkert som uint16_t
  return static_cast<std::uint16_t>(bytesRead);
}

bool Esp32s3::isConnected() const noexcept
{
  return myConnectionStatus;
}

} // namespace driver::serial
