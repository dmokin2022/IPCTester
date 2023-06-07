#pragma once

//#include <arch/IAudioInput.hpp>
//#include <tools/Common.hpp>

#include <mi_ai.h>

#include "Drivers/IAudioInput.hpp"

enum class ACodec : int8_t { UNKNOWN = -1, G711A = 0, G711U, G726, AAC, OPUS, COUNT };

class AudioInput2 : public IAudioInput {
public:
  AudioInput2()  = default;
  ~AudioInput2() = default;

  void setACodec(ACodec _codec);
  void setChannel(uint8_t _chn);
  void initialize() override;
  void terminate() override;
  void start() override;
  bool setVolume(uint8_t _db) override { return true; }
  bool setSampleRate(SampleRate _sr) override { return true; }

  int getSamplesSum(MI_AUDIO_Frame_t frame);

protected:
  uint8_t dev_ {0};
  uint8_t chn_ {0};
  uint8_t port_ {0};
  ACodec codec_ {ACodec::UNKNOWN};
  uint8_t frameDepth {0};
  uint8_t queueDepth {0};
};
