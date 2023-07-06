#pragma once

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>

#define I2C_DEVICE "/dev/i2c-0"
#define I2C_SLAVE_ADDRESS \
  0x10  // адрес, сдвинутый на 1 разряд вправо \
        // (в пакете будет 0x20)

class I2C {
public:
  explicit I2C();
  ~I2C();
  bool init();
  int read(unsigned char slave_addr, uint8_t *buf, uint16_t size);
  int write(unsigned char slave_addr, const void *buf, uint16_t size);

private:
  int i2c_fd;
};
