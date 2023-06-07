#pragma once

#include <QObject>

#include "Configs/CameraConfig.h"
#include "SSC337DE/include/mi_ai.h"

class AudioInput : public QObject {
  Q_OBJECT
public:
  explicit AudioInput(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, QObject *parent = nullptr);

  void writeFromMicTo(void *waveTable, size_t samplesInWave);

  MI_U16 u16Buf[AUDIO_BUFFER_SIZE];
signals:

private:
  // обнуляем выводной буфер
  void resetInputFrame();

  MI_S32 ret;
  MI_AUDIO_Attr_t stAttr;
  MI_AUDIO_DEV AiDevId = 0;
  MI_AI_CHN AiChn      = 0;
  MI_SYS_ChnPort_t stChnPort;
  MI_S32 s32Fd;

  fd_set readFdSet;

  struct timeval stTimeOut;

  MI_AUDIO_Frame_t stAiChFrame;
  MI_AUDIO_AecFrame_t stAecFrame;
  MI_SYS_ChnPort_t stAiChnOutputPort;
};
