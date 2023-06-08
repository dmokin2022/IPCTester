#include "CameraCLI.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>

#include "Configs/CameraConfig.h"

CameraCLI::CameraCLI(int port, QObject *parent) : QObject {parent} {
  //
  connector = new Connector("", port, QString(CAMERA_COMMAND_TERMINATOR), parent);
  connect(connector, &Connector::readyToBeParsed, this, &CameraCLI::parse);
  responseIsReady = false;

  initPwm();
  initGPIO();
  adcSar.Init();
  ao = new AudioOutput();
  ai = new AudioInput();
  ai->startListening();
}

void CameraCLI::parse(const QString &incomingString) {
  QStringList tokens = incomingString.split(" ");
  if (tokens.size() < CAMERA_WORDS_IN_RESPONSE) {
    unknownCommandResponse();
    return;
  }

  QString command = tokens[0];
  QString module  = tokens[1];
  int channel     = tokens[2].toInt();
  int value       = 0;
  QString string  = "";

  // ------------GET--------------
  if (command == "get") {
    if ((module == "gpio") || (module == "pin")) {
      value = getGpio(channel);
    } else if (module == "adc") {
      value = getAdc(channel);
    } else if (module == "sound") {
      value = getSound();
    }

    // ------------SET--------------
  } else if (command == "set") {
    // Проверка на коректность команды set (должно быть 4 слова)
    if (tokens.size() != CAMERA_WORDS_IN_COMMAND) {
      unknownCommandResponse();
      return;
    }

    value = tokens[3].toInt();

    if (module == "gpio" || module == "pin") {
      setGpio(channel, value);
    } else if (module == "pwm") {
      setPwm(channel, value);
    } else if (module == "motor") {
      setMotor(channel, value);
    } else if (module == "sound") {
      setSound(channel, value);
    }

    // ------------SEND-------------
  } else if (command == "send") {
    // Проверка на коректность команды send (должно быть 4 слова)
    if (tokens.size() != CAMERA_WORDS_IN_COMMAND) {
      unknownCommandResponse();
      return;
    }

    string = tokens[3];

    if (module == "uart") {
      sendUart(0, string);
    } else if (module == "i2c") {
      sendI2c(0, string);
    }
    // -----------RECEIVE-----------
  } else if (command == "receive") {
    if (module == "uart") {
      receiveUart(0, value);
    } else if (module == "i2c") {
      receiveI2c(0, value);
    }
    // ------ПРОЧИЕ КОМАНДЫ---------
  } else {
    //
    unknownCommandResponse();
  }

  // TODO: Внимание!!! Для разных команд должен быть разный формат ответа
  // set|get = int value  и send|receive = QString string
  connector->send(QString("%1 %2 %3").arg(module).arg(channel).arg(value));

  responseIsReady = true;
}

void CameraCLI::waitForResponse() {
  while (!responseIsReady) {}
  responseIsReady = false;
}

void CameraCLI::setGpio(int pin, int value) {
  //  command = QString("set gpio %1 %2").arg(pin).arg(value);
  exec(QString("echo %1 > /sys/class/gpio/gpio%2/value").arg(value).arg(pin));
}

int CameraCLI::getGpio(int pin) {
  FILE *FileOpen;
  char buffer[] = "################";

  QString fileName = QString("cat /sys/class/gpio/gpio%1/value").arg(pin);

  FileOpen = popen(fileName.toStdString().c_str(), "r");
  while (fgets(buffer, sizeof(buffer), FileOpen)) { puts(buffer); }

  qDebug() << buffer;

  if (buffer[0] == '0') return 0;
  if (buffer[0] == '1') return 1;
  return -1;
}

int CameraCLI::getAdc(int channel) {
  //
  return this->adcSar.GetValue(channel);
}

int CameraCLI::getSound() {
  //
  return ai->getSoundLevel();
}

int CameraCLI::writeToDeviceFile(const QString &deviceFilePath, int value) {
  QString valueStr = QString::number(value);

  int file = open(deviceFilePath.toStdString().c_str(), O_RDWR);
  write(file, valueStr.toStdString().c_str(), valueStr.size());
  return file;
}

void CameraCLI::writeToDeviceFileAndClose(const QString &deviceFilePath, int value) {
  int file = writeToDeviceFile(deviceFilePath, value);
  close(file);
}

void CameraCLI::initPwm() {
  //exec(QString("echo %1 > /sys/class/pwm/pwmchip0/export").arg(IRIS_PWM));
  exec(QString("echo %1 > /sys/class/pwm/pwmchip0/export").arg(IR_DIM));
}

void CameraCLI::initGPIO() {
  exec(QString("echo %1 > /sys/class/gpio/export").arg(IRC_MOTO_PLUS));
  exec(QString("echo %1 > /sys/class/gpio/export").arg(IRC_MOTO_MINUS));
  exec(QString("echo %1 > /sys/class/gpio/export").arg(ALARM_IN));
  exec(QString("echo %1 > /sys/class/gpio/export").arg(ALARM_OUT));
  exec(QString("echo %1 > /sys/class/gpio/export").arg(LED1));
}

void CameraCLI::exec(const QString &command) {
  //
  system(command.toStdString().c_str());
}

void CameraCLI::setPwm(int channel, int valuePercent) {
  /*
  // Как записывать PWM
  echo 8 > /sys/class/pwm/pwmchip0/export // номер канала
  echo 400000 > /sys/class/pwm/pwmchip0/pwm8/period // период шим
  echo 100000 > /sys/class/pwm/pwmchip0/pwm8/duty_cycle // длительность 1 (скважность)
  echo 1 > /sys/class/pwm/pwmchip0/pwm8/enable  // признак запуска
  */
  int value = PWM_PERIOD * valuePercent / 100;

  //exec(QString("echo %1 > /sys/class/pwm/pwmchip0/export").arg(channel));
  exec(QString("echo %1 > /sys/class/pwm/pwmchip0/pwm%2/period").arg(PWM_PERIOD).arg(channel));
  exec(QString("echo %1 > /sys/class/pwm/pwmchip0/pwm%2/duty_cycle").arg(value).arg(channel));
  exec(QString("echo %1 > /sys/class/pwm/pwmchip0/pwm%2/enable").arg(1).arg(channel));
}

void CameraCLI::setMotor(int motorNum, int value) {
  /*
   * Как управлять моторами
  Формат команды:
  echo "0 2 1 3 4000000 100" > /sys/nic-motor/m0
  Значения аргументов:
  1 = A+
  2 = A-
  3 = B+
  4 = B-

  5 = период (наносекунды)
  6 = количество периодов, в течение которых прокручивать

  /sys/nic-motor/m0 - фокус
  /sys/nic-motor/m1 - зум
*/
  /*
  QString deviceFileName = QString("/sys/nic-motor/m%1").arg(motorNum);
  QString parameter      = QString("0 2 1 3 4000000 %1").arg(value);

  int deviceFile = open(deviceFileName.toStdString().c_str(), O_RDWR);
  write(deviceFile, parameter.toStdString().c_str(), parameter.size());
  close(deviceFile);  //закрыть файл
  */
  exec(QString("echo \"0 2 1 3 4000000 %1\" > /sys/nic-motor/m%2").arg(value).arg(motorNum));
}

void CameraCLI::setSound(int duration, int freq) {
  ao->playSoundAsync(freq, duration);
  //  command = QString("get dac %1 %2").arg(pin).arg(value);
}

void CameraCLI::sendUart(int uartNum, const QString &string) {
  int file = open("/dev/ttyS0", O_RDWR);
  write(file, string.toStdString().c_str(), string.size());
  close(file);  //закрыть файл
}

QString CameraCLI::receiveUart(int uartNum, int size) {
  int file = open("/dev/ttyS0", O_RDWR);
  read(file, &inputBuffer, size);
  close(file);
  return "";
}

void CameraCLI::sendI2c(int i2cNum, const QString &string) {
  int file = open("/dev/i2c-0", O_RDWR);
  write(file, string.toStdString().c_str(), string.size());
  close(file);
}

QString CameraCLI::receiveI2c(int i2cNum, int size) {
  int file = open("/dev/i2c-0", O_RDWR);
  read(file, &inputBuffer, size);
  close(file);
  return "";
}

void CameraCLI::unknownCommandResponse() { connector->send("Unknown command"); }
