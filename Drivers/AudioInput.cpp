#include "AudioInput.hpp"

#include "SSC337DE/include/mi_sys.h"

AudioInput::AudioInput(MI_AUDIO_DEV AiDevId, MI_AI_CHN AiChn, QObject *parent) : QObject {parent} {
  /*
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

  // set ao public attr
  ret = MI_AO_SetPubAttr(AoDevId, &stAttr);
  if (MI_SUCCESS != ret) {
    printf("set ao %d attr err:0x%x\n", AoDevId, ret);
  }

  MI_SYS_Exit();
  */
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

  if (FD_ISSET(s32Fd, &readFdSet)) {
    ret = MI_AI_GetFrame(AiDevId, AiChn, &stAiChFrame, &stAecFrame, 0);
    if (MI_SUCCESS == ret) {
      // do something
      MI_AI_ReleaseFrame(AiDevId, AiChn, &stAiChFrame, &stAecFrame);
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
