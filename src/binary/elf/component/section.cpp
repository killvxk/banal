///
/// \file
/// \brief ELF section implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/binary/elf/component/section.hpp"

namespace banal {
namespace binary {
namespace component {

ELFSection::ELFSection(::std::uint16_t index, const ::ELFIO::section* sec)
    : Section(index), _section(*sec), _name(sec->get_name()) {}

::std::string_view ELFSection::name(void) const {
  return _name;
}

::std::uint32_t ELFSection::type(void) const {
  return _section.get_type();
}

::std::uint64_t ELFSection::flags(void) const {
  return _section.get_flags();
}

::std::uint32_t ELFSection::info(void) const {
  return _section.get_info();
}

::std::uint32_t ELFSection::link(void) const {
  return _section.get_link();
}

::std::uint64_t ELFSection::align(void) const {
  return _section.get_addr_align();
}

::std::size_t ELFSection::entry_size(void) const {
  return _section.get_entry_size();
}

::std::uint64_t ELFSection::address(void) const {
  return _section.get_address();
}

::std::size_t ELFSection::size(void) const {
  return _section.get_size();
}

const ::std::uint8_t* ELFSection::data(void) const {
  return reinterpret_cast< const ::std::uint8_t* >(_section.get_data());
}

} // end namespace component
} // end namespace binary
} // end namespace banal
