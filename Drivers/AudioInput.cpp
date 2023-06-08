#include <mi_sys.h>

#include "AudioInput.hpp"

//#include "PArch.hpp"
//#include "Threads.hpp"

//#include "AudioEncoder.hpp"
#include <stdio.h>

#include <QDebug>
#include <stdexcept>
#include <thread>

#define MiTestEx(foo)                 \
  do {                                \
    auto errcode = foo;               \
    printf("errcode: %x\n", errcode); \
  } while (false)

#define ucDebug(debug)     qDebug() << debug
#define ucWarning(warning) qDebug() << warning

#define ucTrace(...)                                                                    \
  do {                                                                                  \
    if (!QString(__VA_ARGS__).isEmpty())                                                \
      fprintf(STDERR, "%s trace: %s -> %s\n", _CURRENT_DATE_TIME_, __PRETTY_FUNCTION__, \
          QString(__VA_ARGS__).toUtf8().data());                                        \
    else                                                                                \
      fprintf(STDERR, "%s trace: %s\n", _CURRENT_DATE_TIME_, __PRETTY_FUNCTION__);      \
    fflush(STDERR);                                                                     \
  } while (0)

#define FFMPEG_AUDIO_ENCODER_FRAME_SIZE 960

AudioInput::AudioInput() { initialize(); }

void AudioInput::setACodec(ACodec _codec) { codec_ = _codec; }

void AudioInput::setChannel(uint8_t _chn) { chn_ = _chn; }

void AudioInput::initialize() {
  //ucDebug();
  MI_AUDIO_Attr_t attr {};
  attr.eSamplerate    = E_MI_AUDIO_SAMPLE_RATE_48000;
  attr.eBitwidth      = E_MI_AUDIO_BIT_WIDTH_16;
  attr.eSoundmode     = E_MI_AUDIO_SOUND_MODE_MONO;
  attr.u32FrmNum      = 1;
  attr.u32PtNumPerFrm = FFMPEG_AUDIO_ENCODER_FRAME_SIZE;
  attr.u32ChnCnt      = 1;
  frameDepth          = 4;
  queueDepth          = 4;
  MiTestEx(MI_AI_SetPubAttr(dev_, &attr));
  MI_AI_Enable(dev_);
  MI_AI_EnableChn(dev_, chn_);
  MI_AI_EnableReSmp(dev_, chn_, E_MI_AUDIO_SAMPLE_RATE_32000);
  MiTestEx(MI_AI_SetVqeVolume(dev_, chn_, 4));  // TODO(44444): gro hardcode

  MI_SYS_ChnPort_t channel_port {
      .eModId = E_MI_MODULE_ID_AI, .u32DevId = dev_, .u32ChnId = chn_, .u32PortId = port_};
  MiTestEx(MI_SYS_SetChnOutputPortDepth(&channel_port, frameDepth, queueDepth));
}

void AudioInput::terminate() {
  MI_AI_DisableChn(dev_, chn_);
  MI_AI_Disable(dev_);
}

void AudioInput::startListening() {
  listeningIsOn = true;
  std::thread listeningMic {[this]() { start(); }};
  listeningMic.detach();
}

void AudioInput::stopListening() { listeningIsOn = false; }

void AudioInput::start() {
  MI_SYS_ChnPort_t chnPort {
      .eModId = E_MI_MODULE_ID_AI, .u32DevId = dev_, .u32ChnId = chn_, .u32PortId = port_};
  int32_t fd = 0;
  MiTestEx(MI_SYS_GetFd(&chnPort, &fd));
  if (fd < 0) throw std::runtime_error {"Invalid runtime descriptor!"};
  MI_AUDIO_Frame_t frame;

  fd_set set;
  // Бесконечный цикл, пока не будет остановлен флагом
  //while ( !static_cast<Threads::Thread *>(QThread::currentThread())->isMethodInterruptionRequested()) {
  while (listeningIsOn) {
    FD_ZERO(&set);
    FD_SET(fd, &set);
    timeval timeout {.tv_sec = 0, .tv_usec = 100000};  // 100 ms : 100 * 1000
    auto result = select(fd + 1, &set, nullptr, nullptr, &timeout);

    if (result < 0) throw std::runtime_error {"Result of \"select\" in audio is invalid"};
    if (!result) { continue; }

    result = MI_AI_GetFrame(dev_, chn_, &frame, nullptr, 100);
    if (result) {
      ucWarning("audio frame not received");
      continue;
    }

    // Подсчёт среднего значения уровня сигнала на микрофоне
    soundLevel = calcSoundLevel(frame);
    //qDebug() << soundLevel;

    MI_AI_ReleaseFrame(dev_, chn_, &frame, nullptr);
  }
}

int AudioInput::calcSoundLevel(MI_AUDIO_Frame_t &frame) {
  uint64_t dts     = frame.u64TimeStamp / 1000;
  void *table      = frame.apSrcPcmVirAddr[0];
  MI_U32 length    = frame.u32SrcPcmLen;
  int16_t *table16 = (int16_t *)table;

  // Подсчёт суммы выборок
  int S = 0;
  for (int i = 0; i < length; ++i) {
    //
    S = S + abs(table16[i]);
  }

  // Вычисление среднего значения
  if (length > 0) {
    return (int)(S / length);
  } else {
    return 0;
  }
}

int AudioInput::getSoundLevel() {
  //
  return soundLevel;
}
