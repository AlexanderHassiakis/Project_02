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
#include "system/logic/logic.h"






extern "C" void app_main(void) {

  system::logic logic(esp32s3);
while (1)
{
  logic.run();
}

  
}
