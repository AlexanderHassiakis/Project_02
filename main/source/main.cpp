// Main file //
/**
 * @brief Stub example.
 */
#include <cstdint>
#include <chrono>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/serial/stub.h"
#include "esp_log.h"


extern "C" void app_main(){

while(1)
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  
}
 
}