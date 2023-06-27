#include "I2C.hpp"

#include <fcntl.h>
#include <qdebug.h>
#include <stdio.h>
#include <sys/ioctl.h>

I2C::I2C() { init(); }

bool I2C::init() {
  i2c_fd = open(I2C_DEVICE, O_RDWR);
  if (!i2c_fd) {
    qDebug() << "Can not open i2c fd " << I2C_DEVICE;
    return false;
  } else {
    qDebug() << "i2c fd open successfully " << I2C_DEVICE << i2c_fd;
    return true;
  }
}

int I2C::read(unsigned char slave_addr, uint8_t *buf, uint16_t size) {
  unsigned char inbuf  = 0;
  unsigned char outbuf = 0;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[2];

  // Исходящий пакет с запросом
  messages[0].addr  = slave_addr;
  messages[0].flags = 0;
  messages[0].len   = sizeof(outbuf);
  messages[0].buf   = &outbuf;

  // Входящий пакет с ответом
  /* The data will get returned in this structure */
  messages[1].addr  = slave_addr;
  messages[1].flags = I2C_M_RD /* | I2C_M_NOSTART*/;
  messages[1].len   = sizeof(inbuf);
  messages[1].buf   = &inbuf;

  /* Send the request to the kernel and get the result back */
  packets.msgs  = messages;
  packets.nmsgs = 2;

  if (ioctl(i2c_fd, I2C_RDWR, &packets) < 0) {
    qDebug() << "Unable to send data I2C";
    return false;
  }
  return true;
}

//int I2C::i2c_write(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char value) {
int I2C::write(unsigned char slave_addr, const void *buf, uint16_t size) {
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[1];
  messages[0].addr  = slave_addr;
  messages[0].flags = 0;
  messages[0].len   = size;
  messages[0].buf   = (uint8_t *)buf;

  QString str((const char *)buf);

  qDebug() << "I2C:" << buf << str;
  qDebug() << "addr:" << QString::number(messages[0].addr);
  qDebug() << "flags:" << QString::number(messages[0].flags);
  qDebug() << "len:" << QString::number(messages[0].len);
  qDebug() << "buf:" << str;

  /* The first byte indicates which register we‘ll write
  The second byte indicates the value to write.
  Note that for many devices, we can write multiple, sequential registers at once by
  simply making outbuf bigger.
  Transfer the i2c packets to the kernel and verify it worked
    */
  packets.msgs  = messages;
  packets.nmsgs = 1;
  if (ioctl(i2c_fd, I2C_RDWR, &packets) < 0) {
    qDebug() << "Unable to send data via I2C" << i2c_fd;
    return false;
  }
  return true;
}
