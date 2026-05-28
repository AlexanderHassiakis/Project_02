#pragma once

namespace driver::tempsensor
{
    class Interface
    {

    public:
        /**
         * @brief Destroy the Interface object
         *
         */
        virtual ~Interface() = default;

        /**
         * @brief Reads the value from the ADC and converts it to celcius.
         *
         * @return int
         */
        virtual int readTemperature() noexcept = 0;

        /**
         * @brief Construct a new init Temp object
         *
         */
        virtual void initTemp() noexcept = 0;
    };
} // namespace driver::tempsensor