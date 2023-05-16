#pragma once

#include <QDataStream>
#include <QObject>
#include <QTcpServer>
#include <QtNetwork/QTcpSocket>

#include "Configs/CameraConfig.h"

class Connector : public QObject {
  Q_OBJECT
public:
  explicit Connector(
      QString ip, int port = 1000, QString terminator = "", QObject *parent = nullptr);

  void setIp(const QString &ip);
  void setPort(int port);

  QString m_ipAddress;
  int m_ipPort;

signals:
  void readyToBeParsed(const QString string);
  void messageWasTransmitted(const QString string);

public slots:
  QString readStringFromBuffer();
  void send(QString string);
  void slotNewConnection();
  void slotClientDisconnected();

private:
  QTcpSocket *tcpSocket;
  QTcpServer *tcpServer;
  QString stringTerminator;
  //QDataStream *in;
};
