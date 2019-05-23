///
/// \file
/// \brief Architecture specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <string_view>
#include <utility>

#include "banal/extern/capstone.hpp"
#include "banal/extern/unicorn.hpp"
#include "banal/util/log.hpp"

namespace banal {

/// \brief Supported architecture
enum Architecture {
  X86,    ///< Intel X86 (32 bits)
  X86_64, ///< Intel X86 (64 bits)
  ARM,    ///< ARM (32 bits)
  AArch64 ///< ARM (64 bits)
};

/// \brief Get the long name of the architecture
///
/// \param a The architecture
///
/// \return The name of the architecture
inline ::std::string_view get_architecture_long_name(Architecture a) {
  switch (a) {
    case Architecture::X86:
      return "Intel x86 (32 bits)";
    case Architecture::X86_64:
      return "Intel x86_64 (64 bits)";
    case Architecture::ARM:
      return "ARM (32 bits) - NOT SUPPORTED YET";
    case Architecture::AArch64:
      return "AArch64 (64 bits) - NOT SUPPORTED YET";
    default: {
      log::unreachable("unreachable");
    }
  }
}

/// \brief Get the short name of the architecture
///
/// \param a The architecture
///
/// \return The name of the architecture
inline ::std::string_view get_architecture_short_name(Architecture a) {
  switch (a) {
    case Architecture::X86:
      return "x86";
    case Architecture::X86_64:
      return "x86_64";
    case Architecture::ARM:
      return "arm";
    case Architecture::AArch64:
      return "aarch64";
    default: {
      log::unreachable("unreachable");
    }
  }
}

/// \brief Get the capstone equivalence
///
/// \param a The architecture
///
/// \return The capstone equivalence
inline ::std::pair< ::cs_arch, ::cs_mode > get_cs_architecture(Architecture a) {
  switch (a) {
    case Architecture::X86:
      return {::CS_ARCH_X86, ::CS_MODE_32};
    case Architecture::X86_64:
      return {::CS_ARCH_X86, ::CS_MODE_64};
    case Architecture::ARM:
      return {::CS_ARCH_ARM, ::CS_MODE_ARM};
    case Architecture::AArch64:
      return {::CS_ARCH_ARM64, ::CS_MODE_ARM};
    default: {
      log::unreachable("unreachable");
    }
  }
}

/// \brief Get the unicorn engine equivalence
///
/// \param a The architecture
///
/// \return The capstone equivalence
inline ::std::pair< ::uc_arch, ::uc_mode > get_uc_architecture(Architecture a) {
  switch (a) {
    case Architecture::X86:
      return {::UC_ARCH_X86, ::UC_MODE_32};
    case Architecture::X86_64:
      return {::UC_ARCH_X86, ::UC_MODE_64};
    case Architecture::ARM:
      return {::UC_ARCH_ARM, ::UC_MODE_ARM};
    case Architecture::AArch64:
      return {::UC_ARCH_ARM64, ::UC_MODE_ARM};
    default: {
      log::unreachable("unreachable");
    }
  }
}

} // end namespace banal

inline ::std::ostream& operator<<(::std::ostream& os, ::banal::Architecture a) {
  return os << get_architecture_long_name(a);
}
