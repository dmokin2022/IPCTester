#pragma once

class Arch;

class PArch {
public:
  void setRef(Arch *_arch);

protected:
  PArch();
  PArch(const PArch &)             = delete;
  PArch &operator=(const PArch &)  = delete;
  PArch(const PArch &&)            = delete;
  PArch &operator=(const PArch &&) = delete;

protected:
  Arch *arch_ = nullptr;
};

inline PArch::PArch() = default;

inline void PArch::setRef(Arch *_arch) { arch_ = _arch; }
