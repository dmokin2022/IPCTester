
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <iostream>

#include "CameraCLI.hpp"
#include "Drivers/AudioInput.hpp"
#include "Drivers/AudioOutput.hpp"

char string[] = "TEST_STRING";

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  CameraCLI *cli = new CameraCLI(CAMERA_IP_PORT, nullptr);

  cli->sendUart(0, "Test string to UART0\n");

  AudioOutput *ao = new AudioOutput(0, 0);
  ao->playSoundSync(220, 100);
  //ao->playSoundAsync(220);

  //  AudioInput *ai = new AudioInput(0, 0);
  //  ai->writeFromMicTo(0, 0);

  std::cout << "Command Line Interpretator is running..." << std::endl;
  qDebug() << "Enter your command...";
  return a.exec();
}
