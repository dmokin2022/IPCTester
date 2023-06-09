
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <iostream>

#include "CameraCLI.hpp"

char string[] = "TEST_STRING";

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  CameraCLI *cli = new CameraCLI(CAMERA_IP_PORT, nullptr);

  cli->sendUart(0, "Test string to UART0\n");

  std::cout << "Command Line Interpretator is running..." << std::endl;
  qDebug() << "Enter your command...";
  return a.exec();
}
