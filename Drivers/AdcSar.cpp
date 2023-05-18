#include "AdcSar.hpp"

#include <fcntl.h>
#include <qdebug.h>
#include <sys/ioctl.h>
#include <unistd.h>

//#include "Debugger.hpp"

//#define Throw(err_string)                                                                        \
//  throw std::runtime_error {                                                                     \
//    std::string {err_string} + "\n\t" + __PRETTY_FUNCTION__ + "\n\tFILE: " + __FILE__ " LINE " + \
//        std::to_string(__LINE__).c_str()                                                         \
//  }

#define Throw(err_string) \
  { qDebug() << "Can't open file/device."; }

namespace {
struct SarAdcConfigRead {
  int channel_value;
  int adc_value;
};
};  // namespace

AdcSar::AdcSar() {};
AdcSar::~AdcSar() = default;

void AdcSar::Init() {
  try {
    int fd = open("/dev/sar", O_WRONLY);
    if (fd == -1) Throw();
    if (ioctl(fd, _IO(k_saradc_ioc_magic, 0 /*0 = INIT*/), NULL) < 0) Throw();

  } catch (std::exception &e) { std::string message = "INIT FAIL " + std::string(e.what()); }
}

uint32_t AdcSar::GetValue(uint32_t sar_num) {
  SarAdcConfigRead adc {};
  adc.channel_value = sar_num;  //PAD_SAR_GPIO0 = 0 PAD_SAR_GPIO1 = 1 PAD_SAR_GPIO2 = 2
  int fd            = open("/dev/sar", O_WRONLY);
  if (fd == -1) Throw();
  if (ioctl(fd, _IO(k_saradc_ioc_magic, 1 /*1 = SET_CHANNEL_READ_VALUE*/), &adc) < 0) Throw();
  close(fd);
  return adc.adc_value;
}
