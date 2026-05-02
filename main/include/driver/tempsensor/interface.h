#pragma once

namespace driver::tempsensor {
class Interface {

public:
  virtual ~Interface() = default;

  virtual int readTemperature() noexcept = 0;

};
} // namespace driver::sensor