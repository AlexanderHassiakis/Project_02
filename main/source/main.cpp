// Main file //
/**
 * @brief GPIO example.
 */
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/adc_types.h" // För att includera ESP egna enums för ADC portar, för att slippa magic numbers. Ex: ADC_CHANNEL_0 = 0-

#include "driver/tempsensor/tmp36.h"


static const char *TAG = "Test TMP36";

extern "C" void app_main(void) {

	ESP_LOGI(TAG, "Test program för tempsensor\n");

	driver::tempsensor::Tmp36 myTemp36(ADC_CHANNEL_0); //Ger oss GPIO 1

	while (true)
	{
		int celcius =  myTemp36.readTemperature();
        int helTal = (celcius/10);
        int deciTal = abs(celcius % 10); // absolutbelopp , matematisk funktion som gör en väldigt enkel sak: den tar bort minustecknet från ett tal.

        ESP_LOGI(TAG,"Avläst tempratur: %d.%d",helTal,deciTal);

        vTaskDelay(pdMS_TO_TICKS(2000)); //2 sekunder delay.

        }

}
