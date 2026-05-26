#pragma once

#include <cstdint>
#include <cstdio>


#include "driver/tempsensor/interface.h"

namespace driver::tempsensor {
class Stub final : public Interface {

public:
   Stub()
   {
      init = false;
   }
   ~Stub() = default;

   int readTemperature() noexcept override
   {
	return tempRead;
   std::printf("STUB,%i",tempRead);
   }

   void initTemp() noexcept override
   {
      init =  true;
   }

   void setTemp(float temp) noexcept
   {
	   tempRead = temp;

   }

 private:
   int tempRead{250};
   bool init;


};
} // namespace driver::tempsensor