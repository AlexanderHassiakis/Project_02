#pragma once

#include <cstdint>
#include "esp_log.h"           // För snygg debugging (ESP_LOGI)


namespace driver::tempsensor {
class Stub final : public Interface {

public:
   ~Stub() = default;

   int readTemperature() noexcept override
   {
	return tempRead;
   }

   void setTemp(float temp) noexcept
   {
	return tempread = temp;
   }



private:

int tempRead = 25;


};
} // namespace driver::tempsensor