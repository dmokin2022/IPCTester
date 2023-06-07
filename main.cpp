

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QtConcurrent>
#include <iostream>

#include "CameraCLI.hpp"
#include "Drivers/AudioInput2.hpp"
#include "Drivers/AudioOutput.hpp"

char string[] = "TEST_STRING";
void startMicListening() {}

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  CameraCLI *cli = new CameraCLI(CAMERA_IP_PORT, nullptr);

  cli->sendUart(0, "Test string to UART0\n");

  //ao->playSoundAsync(220);

  //AudioInput *ai = new AudioInput(0, 0);
  //ai->writeFromMicTo(0, 0);

  AudioInput2 *ai = new AudioInput2();
  ai->initialize();
  std::thread listeningMic {[ai]() { ai->start(); }};
  listeningMic.detach();

  // Внимание!!! Для использования QtConcurrent нужно скопировать библиотеку
  // из /opt/arm-sdk/qt6-arm-glibc/lib/libQt6Concurrent.so.6
  // (*.prl, *.so, *.so.3, *.so.6, *.so.6.4.1) на камеру в sh://root@10.30.7.16/lib
  //QtConcurrent::run([ai]() { ai->start(); });

  //  AudioOutput *ao = new AudioOutput(0, 0);
  //  ao->playSoundSync(110, 100);

  std::cout << "Command Line Interpretator is running..." << std::endl;
  qDebug() << "Enter your command...";
  return a.exec();
}
