#pragma once
#include <QObject>

#include "Connector.hpp"
#include "Drivers/AdcSar.hpp"
#include "Drivers/AudioInput.hpp"
#include "Drivers/AudioOutput.hpp"

class CameraCLI : public QObject {
  Q_OBJECT
public:
  explicit CameraCLI(int port, QObject *parent = nullptr);
  Connector *connector;

public slots:
  void parse(const QString &incomingString);

public:
  void waitForResponse();
  void initPwm();
  void initGPIO();

  // Команды управления платой/камерой
  void setGpio(int pin, int value);
  int getGpio(int pin);
  int getAdc(int channel);
  int getSound();

  void setPwm(int channel, int value);
  void setSound(int duration, int freq);
  void sendUart(int uartNum, const QString &string);
  QString receiveUart(int uartNum, int size);
  void sendI2c(int i2cNum, const QString &string);
  QString receiveI2c(int i2c, int size);
  void setMotor(int motorNum, int value);
  void unknownCommandResponse();

private:
  int writeToDeviceFile(const QString &deviceFilePath, int value);
  void writeToDeviceFileAndClose(const QString &deviceFilePath, int value);
  void exec(const QString &);

  AdcSar adcSar;
  AudioOutput *ao;
  AudioInput *ai;

  QString command;
  bool responseIsReady;
  QString responseString;
  int *intVal;
  QString *strVal;
  char inputBuffer[500];
};
