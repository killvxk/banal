///
/// \file
/// \brief Executable format implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <iostream>

#include "banal/format/elf.hpp"
#include "banal/format/format.hpp"

namespace banal {
namespace format {

::std::unique_ptr< Parser > get_parser(Format f, ::std::istream& stream) {
  ::std::unique_ptr< Parser > p(nullptr);

  switch (f) {
    case Format::ELF: {
      p = ::std::make_unique< ELFParser >(stream);
    } break;

    case Format::PE: {
      p = nullptr;
    } break;

    default: {
      ::banal::log::unreachable("Unreachable");
    }
  }
  if (p && !p->good()) {
    p = nullptr;
  }
  return p;
}

} // end namespace format
} // end namespace banal

::std::ostream& operator<<(::std::ostream& os, ::banal::format::Format f) {
  return os << ::banal::format::str(f);
}
