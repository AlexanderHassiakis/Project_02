//! @note File header missing.
#pragma once

//! @note Sort headers: standard library before project headers.
#include <cstdint>
#include <cstdio>

#include "driver/tempsensor/interface.h"

//! @note Bracket on the next line, please.
namespace driver::tempsensor
{
//! @note Class header missing.
class Stub final : public Interface
{
public:
    /**
     * @brief Construct a new Stub object
     *
     */
    //! @note Added noexcept.
    //! @note Please initialize the members with an initializer list.
    Stub() noexcept { init = false; }

    /**
     * @brief Destroy the Stub object
     *
     */
    //! @note Added noexcept and override.
    ~Stub() noexcept override = default;

    /**
     * @brief Reads temperature.
     *
     * @return int ??
     */
    int readTemperature() noexcept override
    {
        //! @note Removed unreachable std::printf after return.
        return tempRead;
    }

    /**
     * @brief Initilize the tempread
     *
     */
    void initTemp() noexcept override { init = true; }

    //! @note Changed parameter from float to int, since you're using int in this implementation.
    void setTemp(int temp) noexcept { tempRead = temp; }

    /**Esp32s3 construct Forbidden moves/copy. **/
    Stub(const Stub &) = delete;
    Stub(Stub &&) = delete;
    Stub &operator=(const Stub &) = delete;
    Stub &operator=(Stub &&) = delete;

private:
    //! @note Please initialize in the constructor, not here. :)
    //! @note Please use prefix 'my' for member variables.
    int tempRead{250};
    bool init;
};
} // namespace driver::tempsensor