/**
 * @brief ESP32S3 Factory driver
 *
 */


#include "esp_log.h" // För snygg debugging (ESP_LOGI)
#include <cstdint>
#include <memory>

#include "driver/factory/esp32s3.h"

static const char *TAG = "FACTORY ESP32";

namespace driver::factory {

  Esp32s3::Esp32s3() noexcept = default;

  Esp32s3::~Esp32s3() noexcept override{ESP_LOGI(TAG, "Factory destroyed!\n")}

  Esp32s3::std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override {
    return std::make_unique<gpio::Esp32s3>(pin);
    ESP_LOGI(TAG, "GPIO PIN WORKING!");
  }




} // namespace driver::factory
