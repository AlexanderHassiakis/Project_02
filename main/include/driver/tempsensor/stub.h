#pragma once

#include <cstdint>
#include <cstdio>

#include "driver/tempsensor/interface.h"

namespace driver::tempsensor
{
class Stub final : public Interface
{
public:
    /**
     * @brief Construct a new Stub object
     *
     */
    Stub() { init = false; }

    /**
     * @brief Destroy the Stub object
     *
     */
    ~Stub() = default;

    /**
     * @brief Reads temperature.
     *
     * @return int
     */
    int readTemperature() noexcept override
    {
        return tempRead;
        std::printf("STUB,%i", tempRead);
    }

    /**
     * @brief Initilize the tempread
     *
     */
    void initTemp() noexcept override { init = true; }

    void setTemp(float temp) noexcept { tempRead = temp; }


    /**Esp32s3 construct Forbidden moves/copy. **/
    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;

private:
    int tempRead{250};
    bool init;
};
} // namespace driver::tempsensor