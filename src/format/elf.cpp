///
/// \file
/// \brief ELF parser implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <elfio/elfio.hpp>

#include "banal/format/elf.hpp"
#include "banal/util/log.hpp"

namespace banal {
namespace format {

ELFParser::ELFParser(::std::istream& stream) : Parser(stream), _reader() {
  if (!_reader.load(this->stream())) {
    ::banal::log::cerr() << "Unable to parse the ELF." << ::std::endl;
    this->set_good(false);
    return;
  }
  this->set_good(true);
}

::banal::Architecture ELFParser::architecture(void) const {
  auto machine = _reader.get_machine();
  switch (machine) {
    case 0x03: {
      return ::banal::Architecture::X86;
    }
    case 0x28: {
      return ::banal::Architecture::ARM;
    }
    case 0x3E: {
      return ::banal::Architecture::X86_64;
    }
    case 0xB7: {
      return ::banal::Architecture::AArch64;
    }
    default: {
      ::banal::log::cerr() << "Architecture type " << _reader.get_machine()
                           << " is not supported yet." << ::std::endl;
      ::banal::log::unreachable("Not supported yet");
    }
  }
}

::std::uintptr_t ELFParser::entry(void) const {
  return static_cast<::std::uintptr_t >(_reader.get_entry());
}

ELFParser::~ELFParser(void) {}

} // end namespace format
} // end namespace banal
