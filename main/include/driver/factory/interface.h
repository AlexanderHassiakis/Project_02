
/**
 * @brief Factory Interface
 * 
 */
 #pragma once

 #include <cstdint>
 #include <memory>

namespace driver
{
	namespace gpio{class Interface;}
	namespace adc{class Interface;}
	namespace serial{class Interface;}
	namespace tempsensor{class Interface;}
	namespace timer{class Interface;}	
} // namespace driver

namespace driver::factory
{
	class interface
	{

        public:
        virtual ~interface() noexcept = default;
		virtual std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept = 0;

        private:
	};
} // namespace driver::factory
	


