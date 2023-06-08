
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QtConcurrent>
#include <iostream>

#include "CameraCLI.hpp"
#include "Drivers/AudioInput.hpp"
#include "Drivers/AudioOutput.hpp"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  CameraCLI *cli = new CameraCLI(CAMERA_IP_PORT, nullptr);

  cli->sendUart(0, "Test string to UART0\n");

  //AudioInput2 *ai = new AudioInput2();
  //ai->startListening();

  //AudioOutput *ao = new AudioOutput();
  //ao->playSoundAsync(440, 100);

  std::cout << "Command Line Interpretator is running..." << std::endl;
  qDebug() << "Enter your command...";
  return a.exec();
}
