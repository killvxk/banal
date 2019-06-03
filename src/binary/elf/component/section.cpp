///
/// \file
/// \brief ELF section implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/binary/elf/component/section.hpp"
#include "banal/util/log.hpp"

namespace banal {
namespace binary {
namespace component {

ELFSection::ELFSection(const ::ELFIO::elfio& file,
                       ::std::uint16_t index,
                       ::ELFIO::section* sec)
    : Section(index), _section(*sec), _name(sec->get_name()), _symbols() {
  if (this->type() == SHT_SYMTAB || this->type() == SHT_DYNSYM) {
    ::ELFIO::symbol_section_accessor sa(file, sec);
    ::std::size_t len_symbols = sa.get_symbols_num();
    ::banal::log::log("Symbols number: ",
                      len_symbols,
                      " for section `",
                      this->name(),
                      '`');
    for (::std::size_t i = 0; i < len_symbols; i++) {
      ::std::string name;
      uintarch_t value = 0;
      ::std::size_t size;
      uint8_t bind = 0;
      uint8_t type = 0;
      ::std::uint16_t j = 0;
      uint8_t other = 0;
      if (!sa.get_symbol(i, name, value, size, bind, type, j, other)) {
        ::banal::log::cerr()
            << "Unable to retrieve symbol n°" << ::std::dec << i
            << " of section `" << this->name() << "`." << ::std::endl;
        return;
      }
      ::banal::log::log("Retrieve symbol `",
                        name,
                        "` from section `",
                        this->name(),
                        "`.");
      _symbols.emplace_back(i, name, value, size, type, *this);
    }
  }
}

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
