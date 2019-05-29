///
/// \file
/// \brief Executable format specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <memory>
#include <ostream>
#include <string_view>

#include "banal/util/log.hpp"

namespace banal {

/// \brief Executable format specification
enum class Format {
  ELF, ///< ELF
  PE   ///< PE
};

/// \brief Get the string representation of a format
///
/// \param f The format
///
/// \return The string representation of the format f
inline ::std::string_view str(Format f) {
  switch (f) {
    case Format::ELF:
      return "ELF";
    case Format::PE:
      return "PE";
    default:
      log::unreachable("Unreachable");
  }
}

} // end namespace banal

/// \brief Display a format
///
/// \param f The format to display
///
/// \return The output stream
::std::ostream& operator<<(::std::ostream&, ::banal::Format f);
