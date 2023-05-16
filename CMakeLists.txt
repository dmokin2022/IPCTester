cmake_minimum_required(VERSION 3.14)

project(IPCTesterCam LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

add_executable(IPCTesterCam
  main.cpp
  CameraCLI.cpp
  CameraCLI.hpp
  Connector.cpp
  Connector.hpp
#  Connector/CameraConnector.cpp
#  Connector/CameraConnector.hpp

  Configs/CameraConfig.h
  Configs/StandConfig.h
  Configs/TestNames.h
  Configs/Thresholds.h
)

target_link_libraries(IPCTesterCam Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(IPCTesterCam Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS IPCTesterCam DESTINATION /)
