//! @note File header missing.
#pragma once

namespace driver::tempsensor
{
//! @note Class header missing.
class Interface
{
public:
    /**
     * @brief Destroy the Interface object
     *
     */
    //! @note Added noexcept.
    virtual ~Interface() noexcept = default;

    /**
     * @brief Reads the value from the ADC and converts it to celcius.
     *
     * @return int ??
     */
    virtual int readTemperature() noexcept = 0;

    /**
     * @brief Construct a new init Temp object
     */
    virtual void initTemp() noexcept = 0;
};
} // namespace driver::tempsensor
