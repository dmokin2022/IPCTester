#include "Configs/CameraConfig.h"
#include "Connector.hpp"

Connector::Connector(QString ip, int port, QString terminator, QObject *parent) : QObject {parent} {
  m_ipAddress      = ip;
  m_ipPort         = port;
  stringTerminator = terminator;

  tcpServer = new QTcpServer(this);
  //tcpServer->listen(QHostAddress::Any, CAMERA_IP_PORT);
  connect(tcpServer, &QTcpServer::newConnection, this, &Connector::slotNewConnection);

  if (!tcpServer->listen(QHostAddress::Any, CAMERA_IP_PORT)) {
    qDebug() << "server is not started";
  } else {
    qDebug() << "server is started";
  }

  //  tcpSocket = new QTcpSocket(this);
  //  tcpSocket->bind(m_ipPort);
  //  connect(tcpSocket, &QTcpSocket::readyRead, this, &AbstractConnector::readStringFromBuffer);
  //  tcpSocket->connectToHost(m_ipAddress, m_ipPort);
  //in = new QDataStream(tcpSocket);
  //in->setVersion(QDataStream::Qt_4_0);
}

void Connector::setIp(const QString &ip) { m_ipAddress = ip; }

void Connector::setPort(int port) { m_ipPort = port; }

void Connector::send(QString string) {
  //QByteArray array = string.toUtf8() + STAND_COMMAND_TERMINATOR;
  QByteArray array = string.toUtf8() + stringTerminator.toUtf8();
  qDebug() << "Отправлено:" << array;
  tcpSocket->write(array);
  emit messageWasTransmitted(string);
}

void Connector::slotNewConnection() {
  tcpSocket = tcpServer->nextPendingConnection();

  tcpSocket->write("Hello, World!!! I am echo server!\r\n");

  connect(tcpSocket, &QTcpSocket::readyRead, this, &Connector::readStringFromBuffer);
  connect(tcpSocket, &QTcpSocket::disconnected, this, &Connector::slotClientDisconnected);
}

void Connector::slotClientDisconnected() {}

QString Connector::readStringFromBuffer() {
  QDataStream in(tcpSocket);

  QByteArray array = tcpSocket->readAll();
  QString string(array);

  QStringList stringList = string.split("\r\n");

  qDebug() << "Принято:" << array;
  //  qDebug() << "QString:" << string;
  for (auto string : stringList) {
    //    qDebug() << string;
    //emit readyToBeParsed(string);
  }

  string = stringList.first();
  emit readyToBeParsed(string);

  return string;
}
