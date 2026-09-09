#include <cstdint>
#include <memory>

#include "driver/factory/esp32s3.h"

#include "driver/adc/esp32s3.h"
#include "driver/gpio/esp32s3.h"
#include "driver/mqtt/esp32s3.h"
#include "driver/serial/esp32s3.h"
#include "driver/tempsensor/tmp36.h"
#include "driver/timer/esp32s3.h"
#include "driver/watchdog/esp32s3.h"

namespace driver::factory
{
// adc
std::unique_ptr<adc::Interface> Esp32s3::adc(std::uint8_t pin) noexcept
{ return std::make_unique<adc::Esp32s3>(); }

// gpio
std::unique_ptr<gpio::Interface> Esp32s3::gpio(std::uint8_t pin) noexcept
{
    // Du skickade bara 'pin', men den behöver 'Direction' också:
    return std::make_unique<gpio::Esp32s3>(pin, gpio::Direction::Output);
}
// serial
std::unique_ptr<serial::Interface> Esp32s3::serial() noexcept
{ return std::make_unique<serial::Esp32s3>(); }

// timer
std::unique_ptr<timer::Interface> Esp32s3::timer() noexcept
{ return std::make_unique<timer::Esp32s3>(); }

// temperatursensor
std::unique_ptr<tempsensor::Interface>
Esp32s3::tempSensor(std::uint8_t pin, adc::Interface& adc,
                    driver::ai_adapt::Interface* linReg) noexcept
{
    // Vi kollar om vi har en linReg-modell här. I så fall skapar vi en smart sensor.
    if (nullptr != linReg) { return std::make_unique<tempsensor::Smart>(adc, pin, linReg); }
    // Annars skapar vi en vanlig sensor (TMP36).
    return std::make_unique<tempsensor::Tmp36>(adc, pin);
};
// Watchdog
std::unique_ptr<watchdog::Interface> Esp32s3::watchdog() noexcept
{ return std::make_unique<driver::watchdog::Esp32s3>(); }

// MQTT
std::unique_ptr<mqtt::Interface> Esp32s3::mqtt() noexcept
{ return std::make_unique<driver::mqtt::Esp32s3>(); }

} // namespace driver::factory