#include "CameraCLI.hpp"

#include <fcntl.h>
#include <unistd.h>

#include "Configs/CameraConfig.h"

CameraCLI::CameraCLI(int port, QObject *parent) : QObject {parent} {
  //
  connector = new Connector("", port, QString(CAMERA_COMMAND_TERMINATOR), parent);
  connect(connector, &Connector::readyToBeParsed, this, &CameraCLI::parse);
  responseIsReady = false;
}

void CameraCLI::parse(const QString incomingString) {
  QStringList tokens = incomingString.split(" ");
  QString command    = tokens[0];
  QString module     = tokens[1];
  int channel        = tokens[2].toInt();
  int value          = tokens[3].toInt();
  QString string     = tokens[3];

  //QString responseString = QString(tokens);

  if (command == "get") {
    if (module == "gpio") {
      getGpio(channel, value);
    } else if (module == "adc") {
      getAdc(channel, value);
    }
    //connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));

  } else if (command == "set") {
    if (module == "gpio") {
      setGpio(channel, value);
    } else if (module == "pwm") {
      setPwm(channel, value);
    } else if (module == "dac") {
      setDac(channel, value);
    }
    //connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));

  } else if (command == "send") {
    if (module == "uart") {
      sendUart(0, string);
    } else if (module == "i2c") {
      sendI2c(0, string);
    }
    //connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));

  } else if (command == "receive") {
    if (module == "uart") {
      receiveUart(0, value);
    } else if (module == "i2c") {
      receiveI2c(0, value);
    }
    //connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));
  }

  connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));

  responseIsReady = true;
}

void CameraCLI::waitForResponse() {
  while (!responseIsReady) {}
  responseIsReady = false;
}

void CameraCLI::setGpio(int pin, int value) {
  command = QString("set gpio %1 %2").arg(pin).arg(value);
  connector->send(command);
}

int CameraCLI::getGpio(int pin, int &value) {
  command = QString("get gpio %1 %2").arg(pin);
  intVal  = &value;
  connector->send(command);
  return 0;
}

int CameraCLI::getAdc(int channel, int &value) {
  command = QString("get adc %1").arg(channel);
  intVal  = &value;
  connector->send(command);
  return 0;
}

void CameraCLI::setPwm(int pin, int value) {
  command = QString("set pwm %1 %2").arg(pin).arg(value);
  connector->send(command);
}

void CameraCLI::setDac(int pin, int value) {
  command = QString("get dac %1 %2").arg(pin).arg(value);
  connector->send(command);
}

void CameraCLI::sendUart(int uartNum, QString string) {
  int file = open("/dev/ttyS0", O_RDWR);
  write(file, string.toStdString().c_str(), string.size());
  close(file);  //закрыть файл
}

QString CameraCLI::receiveUart(int uartNum, int size) {
  int file = open("/dev/ttyS0", O_RDWR);
  read(file, &inputBuffer, size);
  close(file);  //закрыть файл
}

void CameraCLI::sendI2c(int i2cNum, QString string) {
  int file = open("/dev/i2c-0", O_RDWR);
  write(file, string.toStdString().c_str(), string.size());
  close(file);  //закрыть файл
}

QString CameraCLI::receiveI2c(int i2cNum, int size) {
  int file = open("/dev/i2c-0", O_RDWR);
  read(file, &inputBuffer, size);
  close(file);  //закрыть файл
}

// Синхронные команды
void CameraCLI::setGpioSync(int pin, int value) {
  command = QString("set gpio %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

int CameraCLI::getGpioSync(int pin) {
  command = QString("get gpio %1 %2").arg(pin);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

int CameraCLI::getAdcSync(int channel) {
  command = QString("get adc %1").arg(channel);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

void CameraCLI::setPwmSync(int pin, int value) {
  command = QString("set pwm %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCLI::setDacSync(int pin, int value) {
  command = QString("set dac %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCLI::sendUartSync(int uartNum, QString string) {
  command = QString("send uart %1 %2").arg(uartNum).arg(string);
  connector->send(command);
}

QString CameraCLI::receiveUartSync(int uartNum) {
  command = QString("receive uart %1").arg(uartNum);
  connector->send(command);
  waitForResponse();
  return responseString;
}

void CameraCLI::sendI2cSync(int i2cNum, QString string) {
  command = QString("send i2c %1 %2").arg(i2cNum).arg(string);
  connector->send(command);
  waitForResponse();
}

QString CameraCLI::receiveI2cSync(int i2cNum) {
  command = QString("send i2c %1").arg(i2cNum);
  connector->send(command);
  waitForResponse();
  return responseString;
}
