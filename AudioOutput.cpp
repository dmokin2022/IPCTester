#include "AudioOutput.hpp"

#include <math.h>

#include "SSC337DE/include/mi_sys.h"

AudioOutput::AudioOutput(MI_AUDIO_DEV AoDevId, MI_AO_CHN AoChn, QObject *parent)
    : QObject {parent} {
  this->AoDevId = AoDevId;
  this->AoChn   = AoChn;
  //AoDevId = 0;
  //AoChn = 0;

  stAttr.eBitwidth      = E_MI_AUDIO_BIT_WIDTH_16;
  stAttr.eSamplerate    = E_MI_AUDIO_SAMPLE_RATE_8000;
  stAttr.eSoundmode     = E_MI_AUDIO_SOUND_MODE_MONO;
  stAttr.eWorkmode      = E_MI_AUDIO_MODE_I2S_MASTER;
  stAttr.u32PtNumPerFrm = 2048;
  //AUDIO_BUFFER_SIZE;
  stAttr.u32ChnCnt = 1;

  Ts = 1.0 / (double)stAttr.eSamplerate;  // длительность экспозиции одного сэмпла

  int bitWidth = 16 + stAttr.eBitwidth * 8;
  Amp          = pow(2, bitWidth - 2);

  MI_SYS_Init();

  /* set ao public attr*/
  ret = MI_AO_SetPubAttr(AoDevId, &stAttr);
  if (MI_SUCCESS != ret) {
    printf("set ao %d attr err:0x%x\n", AoDevId, ret);
    //return ret;
  }

  MI_SYS_Exit();
}

void AudioOutput::playBuf(void *buf, size_t bufSize) {
  // Подготовка буфера воспроизведения
  resetOutputFrame();

  stAoSendFrame.eBitwidth  = E_MI_AUDIO_BIT_WIDTH_16;    /* audio frame bitwidth */
  stAoSendFrame.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO; /* audio frame momo or stereo mode */
  stAoSendFrame.u32Len     = bufSize;  // размер буфера звукозаписи
  stAoSendFrame.apVirAddr[0] = buf;  // указатель на нахождение буфера для канала 0
  stAoSendFrame.apVirAddr[1] = NULL;  // для канала 1 звук не выводится

  // Воспроизведение звука из буфера
  do {
    //
    ret = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, -1);
  } while (ret == MI_AO_ERR_NOBUF);
}

// Воспроизводит заданную частоту (нужно отстроить)
void AudioOutput::playSound(double freq, double duration) {
  // Расчёт параметров для генерации сэмплов волны

  // Расчёт количества сэмплов
  double Tf = 1 / freq;  // длительность одной волны (период для заданной частоты)

  double k = (Ts / Tf) * 2 * M_PI;

  int Ns = 2 * Tf / Ts;  // сколько сэмплов уйдёт на формирование 1-го периода волны

  if (Ns < AUDIO_BUFFER_SIZE) {
    for (int i = 0; i < Ns; ++i) {
      int sample = Amp * sin(k * i) + Amp;
      u16Buf[i]  = sample;
    }
  }

  int waves = freq * duration;  // количество целых волн в длительности звучания

  //----------------------------------------------------------------------------------
  MI_SYS_Init();

  /* enable ao device*/
  ret = MI_AO_Enable(AoDevId);
  if (MI_SUCCESS != ret) { printf("enable ao dev %d err:0x%x\n", AoDevId, ret); }

  ret = MI_AO_EnableChn(AoDevId, AoChn);
  if (MI_SUCCESS != ret) { printf("enable ao dev %d chn %d err:0x%x\n", AoDevId, AoChn, ret); }
  //----------------------------------------------------------------------------------

  // Вывод звука в течение заданного времени
  for (int i = 0; i < waves; ++i) {
    // Вывод одного периода волны
    playBuf(&u16Buf, Ns);
  }

  //-----------------------------------------------------------------------------------
  // Выключение канала
  ret = MI_AO_DisableChn(AoDevId, AoChn);
  if (MI_SUCCESS != ret) {
    printf("disable ao dev %d chn %d err:0x%x\n", AoDevId, AoChn, ret);
    //return ret;
  }

  // disable ao device
  ret = MI_AO_Disable(AoDevId);
  if (MI_SUCCESS != ret) {
    printf("disable ao dev %d err:0x%x\n", AoDevId, ret);
    //return ret;
  }

  MI_SYS_Exit();
  //-----------------------------------------------------------------------------------
}

void AudioOutput::playWavFile(const QString &WavFilePath) {
  // Считываем содержимое аудиофайла в буфер

  // Воспроизводим сэмплы из буфера
}

// обнуляем выводной буфер
void AudioOutput::resetOutputFrame() { memset(&stAoSendFrame, 0x0, sizeof(MI_AUDIO_Frame_t)); }
