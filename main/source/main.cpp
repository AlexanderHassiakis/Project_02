// Main file //
/**
 * @brief GPIO example.
 */
#include <cstdint>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc/esp32s3.h"
/**
 * @brief Blink an LED every 500 ms.
 */
extern "C" void app_main(void) {
 
  

  // Configure LED as output.
driver::adc::Esp32s3 adc1;
adc1.isInitialized();

  // Blink LED every 500 ms.
  while (1)
  {
 
      ESP_LOGI("test" , "value adc %u", adc1.readRaw(3) );
      vTaskDelay(pdMS_TO_TICKS(500));
  }
}
