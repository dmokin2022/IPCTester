#include "CameraConnector.hpp"

#include "Configs/CameraConfig.h"

CameraConnector::CameraConnector(int port, QString terminator, QObject *parent)
    : AbstractConnector("", port, terminator, parent) {}
