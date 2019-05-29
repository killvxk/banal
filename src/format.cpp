///
/// \file
/// \brief Executable format implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <iostream>

#include "banal/format.hpp"

::std::ostream& operator<<(::std::ostream& os, ::banal::Format f) {
  return os << ::banal::str(f);
}
