#pragma once

#include <QObject>

#include "Configs/CameraConfig.h"
#include "SSC337DE/include/mi_ao.h"

#define AUDIO_BUFFER_SIZE 2048

class AudioOutput : public QObject {
  Q_OBJECT
public:
  explicit AudioOutput(MI_AUDIO_DEV AoDevId, MI_AO_CHN AoChn, QObject *parent = nullptr);

  void playBuf(void *buf, size_t bufSize, int mode);
  void playSoundSync(double freq, double duration);
  void playSoundAsync(double freq);

  void playWavFile(const QString &WavFilePath);

signals:

private:
  void resetOutputFrame();
  void sourceOn();
  void sourceOff();
  void calcParams();

  MI_AUDIO_DEV AoDevId;
  MI_AO_CHN AoChn;
  MI_AUDIO_Attr_t pstAttr;

  double Ts;   // период одного семпла
  double Amp;  // амплитуда сэмпла

  MI_S32 ret;
  MI_AUDIO_Attr_t stAttr;
  MI_AUDIO_Attr_t stGetAttr;
  //MI_U8 u8Buf[AUDIO_BUFFER_SIZE];
  MI_U16 u16Buf[AUDIO_BUFFER_SIZE];
  MI_AUDIO_Frame_t stAoSendFrame;
};
