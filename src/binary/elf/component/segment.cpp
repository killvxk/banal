///
/// \file
/// \brief ELF segment implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/binary/elf/component/segment.hpp"

namespace banal {
namespace binary {
namespace component {

ELFSegment::ELFSegment(::std::uint16_t index, const ::ELFIO::segment* seg)
    : Segment(index), _seg(*seg) {}

::std::size_t ELFSegment::offset(void) const {
  return _seg.get_offset();
}

::std::uint32_t ELFSegment::type(void) const {
  return _seg.get_type();
}

::std::uint32_t ELFSegment::flags(void) const {
  return _seg.get_flags();
}

uintarch_t ELFSegment::align(void) const {
  return _seg.get_align();
}

uintarch_t ELFSegment::virtual_address(void) const {
  return _seg.get_virtual_address();
}

uintarch_t ELFSegment::physical_address(void) const {
  return _seg.get_physical_address();
}

::std::size_t ELFSegment::file_size(void) const {
  return _seg.get_file_size();
}

::std::size_t ELFSegment::memory_size(void) const {
  return _seg.get_memory_size();
}

const ::std::uint8_t* ELFSegment::data(void) const {
  return reinterpret_cast< const ::std::uint8_t* >(_seg.get_data());
}

} // end namespace component
} // end namespace binary
} // end namespace banal
