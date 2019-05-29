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

::std::uint64_t ELFBinary::entry(void) const {
  return static_cast<::std::uint64_t >(_reader.get_entry());
}

bool ELFBinary::parse(void) {
  ::banal::log::log("Start ELF parsing");

  _pie = _reader.get_type() == ET_DYN;

  auto seg_num = _reader.segments.size();
  ::banal::log::log("Segment number: ", seg_num);
  for (::std::uint32_t i = 0; i < seg_num; i++) {
    {
      const auto* seg = _reader.segments[i];
      _segments.emplace_back(
          ::std::make_unique< component::ELFSegment >(i, seg));
    }
    auto& seg = _segments.back();
    // check NX
    if (seg->type() == PT_GNU_STACK && (seg->flags() & PF_X)) {
      _nx = false;
    }
  }

  auto sec_num = _reader.sections.size();
  ::banal::log::log("Section number: ", sec_num);
  for (::std::uint32_t i = 0; i < sec_num; i++) {
    const auto* sec = _reader.sections[i];
    _sections.emplace_back(::std::make_unique< component::ELFSection >(i, sec));
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

} // end namespace binary
} // end namespace banal
