#pragma once

//#include "IAdcSar.hpp"
#include <stdint.h>

//#include "Macro.hpp"

class AdcSar /* : public IAdcSar*/ {
public:
  AdcSar();
  ~AdcSar();

public:
  void Init() /*override*/;

public:
  uint32_t GetValue(uint32_t sar_num) /*override*/;

private:
  const char k_saradc_ioc_magic = 'a';
};
