#pragma once

#include "AbstractConnector.hpp"
#include "Configs/CameraConfig.h"

class CameraConnector : public AbstractConnector {
  Q_OBJECT
public:
  explicit CameraConnector(int port, QString terminator, QObject *parent = nullptr);
};
