///
/// \file
/// \brief Symbol implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/binary/component/symbol.hpp"

namespace banal {
namespace binary {
namespace component {

Symbol::Symbol(::std::size_t index,
               ::std::string_view name,
               uintarch_t value,
               ::std::size_t size,
               ::std::uint8_t type,
               Section& section)
    : _index(index),
      _name(name),
      _value(value),
      _size(size),
      _type(type),
      _section(section) {}

} // end namespace component
} // end namespace binary
} // end namespace banal
