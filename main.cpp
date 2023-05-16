
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

  int file = open("/dev/ttyS0", O_RDWR);

  write(file, string, sizeof(string));

  cli->sendUart(0, "QString string\n");

  //sprintf()

  std::cout << "Command Line Interpretator is running..." << std::endl;
  //std::cont
  //return 0;
  return a.exec();

  close(file);  //закрыть файл
}
