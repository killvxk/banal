///
/// \file
/// \brief Logging implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <bitset>
#include <iomanip>

#include "banal/util/log.hpp"

namespace banal {
namespace log {

::std::ostream& cerr(::std::ostream& out) {
  out << CODE_BRED << "[-] " << CODE_RESET << CODE_WHITE;
  return out;
}

::std::ostream& cwarn(::std::ostream& out) {
  out << CODE_BYELLOW << "[w] " << CODE_RESET << CODE_WHITE;
  return out;
}

::std::ostream& cgood(::std::ostream& out) {
  out << CODE_BGREEN << "[v] " << CODE_RESET << CODE_WHITE;
  return out;
}

::std::ostream& cinfo(::std::ostream& out) {
  out << CODE_BCYAN << "[!] " << CODE_RESET << CODE_WHITE;
  return out;
}

} // end namespace log
} // end namespace banal
