#pragma once

#include <cstdint>
#include <cstdio>


#include "driver/tempsensor/interface.h"

namespace driver::tempsensor {
class Stub final : public Interface {

public:
   Stub()= default;
   ~Stub() = default;

   int readTemperature() noexcept override
   {
	return tempRead;
   std::printf("STUB,%i",tempRead);
   }

   void setTemp(float temp) noexcept
   {
	   tempRead = temp;

   }


private:

int tempRead{250};


};
} // namespace driver::tempsensor