#include "AudioInput.hpp"

#include "SSC337DE/include/mi_sys.h"

AudioInput::AudioInput(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, QObject *parent) : QObject {parent} {
  this->AiDevId = AiDevId;
  this->AiChn   = AiChn;
  //  this->AiDevId = 0;
  //  this->AiChn   = 0;

  MI_SYS_Init();

  stAttr.eBitwidth      = E_MI_AUDIO_BIT_WIDTH_16;
  stAttr.eSamplerate    = E_MI_AUDIO_SAMPLE_RATE_8000;
  stAttr.eSoundmode     = E_MI_AUDIO_SOUND_MODE_MONO;
  stAttr.eWorkmode      = E_MI_AUDIO_MODE_I2S_SLAVE;
  stAttr.u32PtNumPerFrm = 160;
  stAttr.u32ChnCnt      = 1;

  // set public attribute of AI device
  ret = MI_AI_SetPubAttr(AiDevId, &stAttr);

  if (MI_SUCCESS != ret) { printf("set ai %d attr err:0x%x\n", AiDevId, ret); }

  MI_SYS_Exit();
}

void AudioInput::writeFromMicTo(void *waveTable, size_t samplesInWave) {
  MI_SYS_Init();

  // enable AI device
  ret = MI_AI_Enable(AiDevId);
  if (MI_SUCCESS != ret) { printf("enable ai %d err:0x%x\n", AiDevId, ret); }

  // enable AI Channel
  ret = MI_AI_EnableChn(AiDevId, AiChn);
  if (MI_SUCCESS != ret) { printf("enable Dev%d Chn%d err:0x%x\n", AiDevId, AiChn, ret); }

  // set buffer depth
  stAiChnOutputPort.eModId    = E_MI_MODULE_ID_AI;
  stAiChnOutputPort.u32DevId  = AiDevId;
  stAiChnOutputPort.u32ChnId  = AiChn;
  stAiChnOutputPort.u32PortId = 0;
  MI_SYS_SetChnOutputPortDepth(&stAiChnOutputPort, 1, 8);

  // get port fd
  stChnPort.eModId    = E_MI_MODULE_ID_AI;
  stChnPort.u32DevId  = AiDevId;
  stChnPort.u32ChnId  = AiChn;
  stChnPort.u32PortId = 0;
  ret                 = MI_SYS_GetFd(&stChnPort, &s32Fd);

  if (MI_SUCCESS != ret) { printf("Dev%d Chn%d failed to call MI_SYS_GetFd!!!\n", AiDevId, AiChn); }

  // select 100ms
  FD_ZERO(&readFdSet);
  FD_SET(s32Fd, &readFdSet);
  stTimeOut.tv_sec  = 0;
  stTimeOut.tv_usec = 100 * 1000;
  ret               = select(s32Fd + 1, &readFdSet, NULL, NULL, &stTimeOut);

  // Подготовка буфера воспроизведения
  resetInputFrame();
  stAiChFrame.eBitwidth    = E_MI_AUDIO_BIT_WIDTH_16;    /* audio frame bitwidth */
  stAiChFrame.eSoundmode   = E_MI_AUDIO_SOUND_MODE_MONO; /* audio frame momo or stereo mode */
  stAiChFrame.u32SrcPcmLen = samplesInWave;      // размер буфера звукозаписи
  stAiChFrame.u32SrcPcmLen = AUDIO_BUFFER_SIZE;  // размер буфера звукозаписи
  //stAiChFrame.apSrcPcmVirAddr[0] = waveTable;  // указатель на нахождение буфера для канала 0
  stAiChFrame.apSrcPcmVirAddr[0] = u16Buf;  // указатель на нахождение буфера для канала 0
  stAiChFrame.apSrcPcmVirAddr[1] = NULL;  // для канала 1 звук не выводится

  if (FD_ISSET(s32Fd, &readFdSet)) {
    // stAecFrame - задаётся если нужно получить подавление обратной связи AEC
    // AEC - Acoustic Echo Cancellation
    // в нашем случае это не нужно  => =0
    //ret = MI_AI_GetFrame(AiDevId, AiChn, &stAiChFrame, &stAecFrame, 0);
    ret = MI_AI_GetFrame(AiDevId, AiChn, &stAiChFrame, NULL, AUDIO_BLOCKING_MODE);
    if (MI_SUCCESS == ret) {
      // do something
      //MI_AI_ReleaseFrame(AiDevId, AiChn, &stAiChFrame, &stAecFrame);
      MI_AI_ReleaseFrame(AiDevId, AiChn, &stAiChFrame, NULL);
    }
  }

  // disable AI Channel
  ret = MI_AI_DisableChn(AiDevId, AiChn);

  if (MI_SUCCESS != ret) { printf("disable Dev%d Chn%d err:0x%x\n", AiDevId, AiChn, ret); }

  // disable AI Device
  ret = MI_AI_Disable(AiDevId);
  if (MI_SUCCESS != ret) { printf("disable ai %d err:0x%x\n", AiDevId, ret); }

  MI_SYS_Exit();
}

// обнуляем выводной буфер
void AudioInput::resetInputFrame() {
  //
  memset(&u16Buf, 0, sizeof(u16Buf));
  memset(&stAiChFrame, 0x0, sizeof(MI_AUDIO_Frame_t));
}
