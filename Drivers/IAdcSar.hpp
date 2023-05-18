#pragma once

#include "AbstractSubHardware.hpp"
#include "Datatypes.hpp"

class IAdcSar : public AbstractSubHardware {
  public:
  IAdcSar()          = default;
  virtual ~IAdcSar() = default;

  virtual void Init()                         = 0;
  virtual uint32_t GetValue(uint32_t sar_num) = 0;
};
