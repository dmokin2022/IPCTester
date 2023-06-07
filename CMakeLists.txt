cmake_minimum_required(VERSION 3.14)

project(IPCTesterCam LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(SHARED_LIB ON)
#set(STATIC_... ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

find_package(QT NAMES Qt6 QT5 REQUIRED COMPONENTS Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)


add_executable(IPCTesterCam
  main.cpp
  CameraCLI.cpp
  CameraCLI.hpp
  Connector.cpp
  Connector.hpp

  Drivers/AdcSar.hpp
  Drivers/AdcSar.cpp
  Drivers/AudioOutput.hpp
  Drivers/AudioOutput.cpp
  Drivers/AudioInput.hpp
  Drivers/AudioInput.cpp
  Drivers/AudioInput2.hpp
  Drivers/AudioInput2.cpp
  Drivers/IAudioInput.hpp
  Drivers/IAudioInput.cpp
  Drivers/PArch.hpp
#  Drivers/Arch.hpp
#  Drivers/Arch.cpp
  #Drivers/Threads.hpp
  #Drivers/Threads.cpp

  Configs/CameraConfig.h
  Configs/StandConfig.h
  Configs/TestNames.h
  Configs/Thresholds.h
)

target_link_libraries(IPCTesterCam Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(IPCTesterCam Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(IPCTesterCam Qt${QT_VERSION_MAJOR}::Concurrent)

set(SSTAR_337DE ON)

#-----------------------------------------------------------------
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "/opt/arm-toolchain/sigmastar/include")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "/opt/arm-toolchain/sigmastar/drivers-include")
if(SSTAR_337DE OR SSTAR_335DE)
  target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE "/opt/arm-toolchain/sigmastar/uclibc/i6b0/static")
elseif(SSTAR_339G)
  target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE "/opt/arm-toolchain/sigmastar/glibc/i6e/static")
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}
  mi_iqserver mi_sensor mi_vif mi_vpe mi_venc mi_rgn mi_divp mi_vdf mi_shadow
  OD_LINUX MD_LINUX VG_LINUX mi_ive mi_ai mi_ao mi_isp cus3a ispalgo
  mi_sys mi_common
  /opt/arm-toolchain/sigmastar/glibc/i6e/static/libcam_os_wrapper.a
  cam_fs_wrapper
  AEC_LINUX AED_LINUX BF_LINUX APC_LINUX SSL_LINUX SRC_LINUX g711 g726
  liveMedia UsageEnvironment BasicUsageEnvironment groupsock
  dl pthread)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wno-deprecated-enum-enum-conversion")
#-----------------------------------------------------------------


install(TARGETS IPCTesterCam DESTINATION /)
