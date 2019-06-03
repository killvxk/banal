///
/// \file
/// \brief ELF parser implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <iomanip>

#include <elfio/elfio.hpp>

#include "banal/binary/elf/component/section.hpp"
#include "banal/binary/elf/component/segment.hpp"
#include "banal/binary/elf/elf.hpp"
#include "banal/util/log.hpp"

#define PT_GNU_STACK 0x6474e551

namespace banal {
namespace binary {

ELFBinary::ELFBinary(const ::banal::Options& opt,
                     int fd,
                     void* addr,
                     ::std::size_t file_len)
    : Binary(opt, fd, addr, file_len),
      _reader(),
      _segments(),
      _sections(),
      _entry(static_cast< uintarch_t >(_reader.get_entry())),
      _nx(true),
      _pie(true) {
  if (!_reader.load(_stream)) {
    ::banal::log::cerr() << "Unable to parse the ELF." << ::std::endl;
    return;
  }
  _good = true;
}

::banal::Architecture ELFBinary::architecture(void) const {
  auto machine = _reader.get_machine();
  Architecture arch;
  switch (machine) {
    case 0x03: {
      arch = ::banal::Architecture::X86;
    } break;
    case 0x28: {
      arch = ::banal::Architecture::ARM;
    } break;
    case 0x3E: {
      arch = ::banal::Architecture::X86_64;
    } break;
    case 0xB7: {
      arch = ::banal::Architecture::AArch64;
    } break;
    default: {
      ::banal::log::cerr() << "Architecture type " << _reader.get_machine()
                           << " is not supported yet." << ::std::endl;
      ::banal::log::unreachable("Not supported yet");
    }
  }
  if (!::banal::is_architecture_size_supported(arch)) {
    ::banal::log::cerr()
        << "This architecture (" << arch
        << ") size is not supported by this version of banal." << ::std::endl
        << "Please recompile banal with the appropriate architecture size "
           "(using cmake -DARCH_SIZE=(32|64))"
        << ::std::endl;
    ::banal::log::unreachable("Reachable");
  } else {
    return arch;
  }
}

uintarch_t ELFBinary::entry(void) const {
  return _entry;
}

void ELFBinary::set_entry(uintarch_t e) {
  _entry = e;
}

bool ELFBinary::parse(void) {
  ::banal::log::log("Start ELF parsing");

  _pie = _reader.get_type() == ET_DYN;

  auto seg_num = _reader.segments.size();
  ::banal::log::log("Segment number: ", seg_num);
  ::std::vector<::std::uint16_t > known_sections;
  known_sections.reserve(_reader.sections.size());
  for (::std::uint32_t i = 0; i < seg_num; i++) {
    const auto* seg = _reader.segments[i];
    _segments.emplace_back(::std::make_unique< component::ELFSegment >(i, seg));
    auto& segg = _segments.back();
    // check NX
    if (segg->type() == PT_GNU_STACK && (segg->flags() & PF_X)) {
      _nx = false;
    }
  }

  auto sec_num = _reader.sections.size();
  ::banal::log::log("Section number: ", sec_num);
  for (::std::uint32_t i = 0; i < sec_num; i++) {
    auto* sec = _reader.sections[i];
    _sections.emplace_back(
        ::std::make_unique< component::ELFSection >(_reader, i, sec));
    auto& secc = *_sections.back();
    for (auto it = secc.symbols_cbegin(); it != secc.symbols_cend(); it++) {
      auto& sym = *it;
      ::std::pair< uintarch_t, const component::Symbol& > value{sym.value(),
                                                                sym};
      _symbols.insert(value);
    }
  }
  return true;
}

::std::vector<::std::unique_ptr< component::Segment > >::const_iterator
ELFBinary::segments_cbegin(void) const {
  return _segments.cbegin();
}

::std::vector<::std::unique_ptr< component::Segment > >::const_iterator
ELFBinary::segments_cend(void) const {
  return _segments.cend();
}

::std::vector<::std::unique_ptr< component::Section > >::const_iterator
ELFBinary::sections_cbegin(void) const {
  return _sections.cbegin();
}

::std::vector<::std::unique_ptr< component::Section > >::const_iterator
ELFBinary::sections_cend(void) const {
  return _sections.cend();
}

ELFBinary::~ELFBinary(void) {}

void ELFBinary::dump(void) const {
  ::banal::log::cinfo() << _segments.size() << " segment(s): " << ::std::endl;
  for (auto it = this->segments_cbegin(); it != this->segments_cend(); it++) {
    auto& seg = *it;
    ::std::cout << '\t' << '[' << seg->index() << "], type=0x" << ::std::hex
                << seg->type() << ", flags=0x" << std::hex << seg->flags()
                << ", vaddr=0x" << ::std::setw(8) << ::std::setfill('0')
                << seg->virtual_address() << ", paddr=0x" << ::std::setfill('0')
                << seg->physical_address() << ::std::endl;
  }

  ::banal::log::cinfo() << _sections.size() << " section(s): " << ::std::endl;
  for (auto it = this->sections_cbegin(); it != this->sections_cend(); it++) {
    auto& sec = *it;
    ::std::cout << '\t' << '[' << sec->index() << "]: " << sec->name()
                << ", type=0x" << ::std::hex << sec->type() << ", flags=0x"
                << std::hex << sec->flags() << ", address=0x" << ::std::setw(8)
                << ::std::setfill('0') << sec->address() << ::std::endl;
  }
  ::banal::log::cinfo() << "NX: " << this->nx() << ::std::endl;
  ::banal::log::cinfo() << "PIE: " << this->pie() << ::std::endl;
}

::std::optional< uintarch_t > ELFBinary::get_address(
    const component::Symbol& sym) const {
  return this->get_address(sym.value());
}

::std::optional< uintarch_t > ELFBinary::get_address(uintarch_t address) const {
  for (auto& seg : _segments) {
    if (auto real = seg->contains(address)) {
      return *real;
    }
  }
  return ::std::nullopt;
}

::std::optional< std::reference_wrapper< const component::Symbol > > ELFBinary::
    get_symbol(uintarch_t address) const {
  if (const auto it = _symbols.find(address); it != _symbols.end()) {
    return it->second;
  }
  return ::std::nullopt;
}

} // end namespace binary
} // end namespace banal
