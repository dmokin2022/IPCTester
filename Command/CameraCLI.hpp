#pragma once

#include <QObject>

#include "Connector/CameraConnector.hpp"

class CameraCLI : public QObject {
  Q_OBJECT
public:
  explicit CameraCLI(int port, QObject *parent = nullptr);

  CameraConnector *connector;

public slots:
  void parse(const QString incomingString);

  void waitForResponse();

  // Команды управления платой/камерой
  void setGpio(int pin, int value);
  int getGpio(int pin, int &value);
  int getAdc(int channel, int &value);
  void setPwm(int pin, int value);
  void setDac(int pin, int value);
  void sendUart(int uartNum, QString string);
  QString receiveUart(int uartNum, int size);
  void sendI2c(int i2cNum, QString string);
  QString receiveI2c(int i2c, int size);

  void setGpioSync(int pin, int value);
  int getGpioSync(int pin);
  int getAdcSync(int channel);
  void setPwmSync(int pin, int value);
  void setDacSync(int pin, int value);
  void sendUartSync(int uartNum, QString string);
  QString receiveUartSync(int uartNum);
  void sendI2cSync(int i2cNum, QString string);
  QString receiveI2cSync(int i2c);

signals:
  void receivedInt(const int value);
  void receivedString(const QString string);

private:
  QString command;
  bool responseIsReady;
  QString responseString;
  int *intVal;
  QString *strVal;

  char inputBuffer[500];
};
