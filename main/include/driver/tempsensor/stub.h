#pragma once

#include <cstdint>
#include "esp_log.h"  // Debugg

#include "driver/tempsensor/interface.h"

namespace driver::tempsensor {
class Stub final : public Interface {

public:
   ~Stub() = default;

   int readTemperature() noexcept override
   {
	return tempRead;
   ESP_LOGI("STUB","%i",tempRead);
   }

   void setTemp(float temp) noexcept
   {
	   tempRead = temp;

   }


private:

int tempRead = 25;


};
} // namespace driver::tempsensor