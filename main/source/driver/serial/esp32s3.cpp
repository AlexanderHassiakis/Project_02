//! @note File header missing.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "driver/gpio.h"
#include "driver/serial/esp32s3.h"
#include "driver/uart.h"
#include "driver/usb_serial_jtag.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace driver::serial
{
namespace
{
// Delay for write/read operations.
constexpr std::uint16_t delay_ms{100U};
constexpr std::uint16_t delay_zero{0U};
} // namespace

// -----------------------------------------------------------------------------
Esp32s3::Esp32s3() noexcept
    : myDataMsg{}
    , myMsg{nullptr}
    , myDataAvailable{false}
    , myConnectionStatus{false}
{}

// -----------------------------------------------------------------------------
Esp32s3::~Esp32s3() noexcept
{
    //! @note Feel free to place write short lines like this (if you like it).
    if (myConnectionStatus) { usb_serial_jtag_driver_uninstall(); }
}

bool Esp32s3::init() noexcept
{
    // Skip if already initialized.
    if (myConnectionStatus) { return true; }
    constexpr int bufLen{1024};

    // Buffer for usb communication.
    usb_serial_jtag_driver_config_t cfg{};
    cfg.tx_buffer_size = bufLen;
    cfg.rx_buffer_size = bufLen;

    if (ESP_OK != usb_serial_jtag_driver_install(&cfg)) { return false; }
    myConnectionStatus = true;
    ESP_LOGI("init", "UART initialization suceeded!");
    return true;
}

// -----------------------------------------------------------------------------
void Esp32s3::send(const char *msg) noexcept
{
    //! @note Feel free to use auto + = here, since you specifically cast to uint8_t already:
    //! const auto buf = reinterpret_cast<const std::uint8_t*>(msg);
    const std::uint8_t* buf{reinterpret_cast<const std::uint8_t*>(msg)};
    const std::uint16_t bufLen{static_cast<std::uint16_t>(std::strlen(msg))};
    send(buf, bufLen);
}

// -----------------------------------------------------------------------------
void Esp32s3::send(const std::uint8_t *buf, std::uint16_t bufLen) noexcept
{
    if ((false == myConnectionStatus) || (nullptr == buf)) { return; }

    // Check that the file is not to large. if to large send only the bufsize to prevent crash.
    const std::uint16_t copyLen{(bufLen < BufSize) ? bufLen : BufSize};
    usb_serial_jtag_write_bytes(buf, copyLen, pdMS_TO_TICKS(delay_ms));
}

std::uint16_t Esp32s3::received(std::uint8_t *buf, std::uint16_t bufLen) noexcept
{
    if ((false == myConnectionStatus) || (nullptr == buf) || (0U == bufLen)) { return 0U; }
    const int bytesRead{usb_serial_jtag_read_bytes(buf, bufLen, delay_zero)}; // Delay set to zero
    return 0 < bytesRead ? static_cast<std::uint16_t>(bytesRead) : 0U;
}

// -----------------------------------------------------------------------------
std::uint16_t Esp32s3::received(const char *str, std::uint16_t strLen) noexcept
{
    // Cast to byte array to call the main received function.
    std::uint8_t *buf{reinterpret_cast<std::uint8_t *>(const_cast<char *>(str))};
    return received(buf, strLen);
}

// -----------------------------------------------------------------------------
bool Esp32s3::isConnected() const noexcept { return myConnectionStatus; }

} // namespace driver::serial
