/**
 * @brief Esp32s3 GPIO Implementation
 */

#include "driver/gpio/esp32s3.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <cstdint>
#include <cstdio>

    namespace driver::gpio {



void Esp32s3::output(std::uint8_t pinNumber, bool state) noexcept {
  const gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};

  gpio_set_direction(gpioNumber, GPIO_MODE_OUTPUT);
  gpio_set_level(gpioNumber, state);

  std::printf("Pin %u is now set as output\n", pinNumber);

  myState = state;
}

void Esp32s3::input(std::uint8_t pinNumber, bool state) noexcept {
  const gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};

  gpio_set_direction(gpioNumber, GPIO_MODE_INPUT);
  gpio_set_level(gpioNumber, state);

  std::printf("Pin %u is now set as input\n", pinNumber);
}

void Esp32s3::toggle(std::uint8_t pinNumber) noexcept {
  const gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};

  if (gpio_get_level(gpioNumber)== 1) { gpio_set_level(gpioNumber,false); }
  else{gpio_set_level(gpioNumber, true);}
}

void Esp32s3::pullUpGpio(std::uint8_t pinNumber) noexcept {
  const gpio_num_t gpioNumber{static_cast<gpio_num_t>(pinNumber)};

  // ESP-IDF funktion för pullup
  gpio_set_pull_mode(gpioNumber, GPIO_PULLUP_ONLY);

  std::printf("Pin %u is now set as pullup\n", pinNumber);
  ESP_LOGI("Pullup","Pullup %u\n",pinNumber);
}

} // namespace driver::gpio