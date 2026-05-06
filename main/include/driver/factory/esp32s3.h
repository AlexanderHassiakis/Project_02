#include "driver/factory/interface.h"

#include "driver/adc/esp32s3.h"
#include "driver/gpio/esp32s3.h"
#include "driver/serial/esp32s3.h"
#include "driver/timer/esp32s3.h"
#include "driver/tempsensor/tmp36.h"
#include "driver/watchdog/esp32s3.h"

namespace driver::factory 
{
class Esp32s3 : public Interface 
{
public:
    std::unique_ptr<adc::Interface> adc(std::uint8_t pin)) noexcept override 
    {
        return std::make_unique<adc::Esp32s3>(pin);
    }

    std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override 
    {
        return std::make_unique<gpio::Esp32s3>(pin);
    }

    std::unique_ptr<serial::Interface> serial() noexcept override 
    {
        return std::make_unique<serial::Esp32s3>();
    }

    std::unique_ptr<timer::Interface> timer() noexcept override 
    {
        return std::make_unique<timer::Esp32s3>();
    }

    std::unique_ptr<tempsensor::Interface> tempSensor(std::uint8_t pin, adc::Interface& adc) noexcept override 
    {
        return std::make_unique<tempsensor::Tmp36>(pin, adc);
    }

    std::unique_ptr<watchdog::Interface> delay_ms(std::uint16_t ms) noexcept override
    {
        return std::make_unique<watchdog::Esp32s3>(ms);
    }
};

} //namespace driver::factory