#pragma once
#include <stdint.h>

//#include <tools/Common.hpp>

#include "PArch.hpp"

class StreamSource;

enum class SampleRate : uint32_t {
  SR8000  = 8000,
  SR11025 = 11025,
  SR12000 = 12000,
  SR16000 = 16000,
  SR22050 = 22050,
  SR24000 = 24000,
  SR32000 = 32000,
  SR44100 = 44100,
  SR48000 = 48000,
  SR96000 = 96000,

  COUNT
};

class IAudioInput : public PArch {
public:
  IAudioInput() = default;
  virtual ~IAudioInput() {};

public:
  virtual void initialize()                  = 0;
  virtual void terminate()                   = 0;
  virtual void start()                       = 0;
  virtual bool setVolume(uint8_t _db)        = 0;
  virtual bool setSampleRate(SampleRate _sr) = 0;

  SampleRate getSampleRate() const noexcept;

protected:
  SampleRate sampleRate_ {SampleRate::SR8000};
};
