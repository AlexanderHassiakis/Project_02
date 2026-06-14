//! @note File header missing.
#pragma once

#include <cstdint>

#include "driver/adc/interface.h"
#include "driver/tempsensor/interface.h"

namespace driver::tempsensor
{
//! @note Class header missing.
class Tmp36 final : public Interface
{
public:
    /**
     * @brief Construct a new Tmp36 object
     * @param channelAdc ADC channel number.
     */
    explicit Tmp36(driver::adc::Interface &adc, std::uint8_t channelAdc) noexcept;

    /**
     * @brief Destroy the Tmp36 object
     */
    ~Tmp36() noexcept override = default;

    /**
     * @brief Reads temperature in deci-Celsius (225 = 22.5C)
     * @return int ??
     */
    int readTemperature() noexcept override;

    /**
     * @brief Initilze the tempread.
     *
     */
    void initTemp() noexcept override;

    Tmp36(const Tmp36 &) = delete;            // No copy constructor.
    Tmp36(Tmp36 &&) = delete;                 // No move constructor.
    Tmp36 &operator=(const Tmp36 &) = delete; // No copy assignment.
    Tmp36 &operator=(Tmp36 &&) = delete;      // No move assignment.

private:
    //! @note Use 'my' prefix for all member variables.
    driver::adc::Interface &refBorrowAdc;
    //! @note Please use std::uint8_t instead of uint8_t.
    std::uint8_t channelAdc;
    bool init;
};
} // namespace driver::tempsensor
