///
/// \file
/// \brief Configuration specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#ifndef ARCH_SIZE
#error "You must specify an architecture size, which is either 32 or 64"
#endif

#define ARCH_SIZE_32 32
#define ARCH_SIZE_64 64

#include <cstdint>
#if ARCH_SIZE == ARCH_SIZE_32
using uintarch_t = ::std::uint32_t;
namespace banal {
inline bool arch32(void) {
  return true;
}
inline bool arch64(void) {
  return false;
}
inline uintarch_t stack(void) {
  return 0xbffff000;
}
} // end namespace banal
#elif ARCH_SIZE == ARCH_SIZE_64
using uintarch_t = ::std::uint64_t;
namespace banal {
inline bool arch32(void) {
  return false;
}
inline bool arch64(void) {
  return true;
}
inline uintarch_t stack(void) {
  return 0x7ffffffffffff000;
}
} // end namespace banal
#else
#error "Architecture size must be either 32 or 64"
#endif
