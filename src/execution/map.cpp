///
/// \file
/// \brief Memory map implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <iomanip>

#include "banal/execution/map.hpp"
#include "banal/util/log.hpp"

namespace banal {
namespace execution {

Map::Map(::uc_engine* uc,
         ::std::uint64_t address,
         ::std::size_t size,
         ::std::uint32_t perms)
    : _uc(uc),
      _address(address),
      _size(size),
      _perms(perms),
      _good(false),
      _mapped(false) {
  this->map();
}

void Map::map(void) {
  if (_mapped) {
    ::banal::log::cwarn() << "Cannot map " << *this << ": already mapped."
                          << ::std::endl;
    return;
  }
  if (auto e = ::uc_mem_map(_uc, _address, _size, _perms); e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to map " << *this << ": "
                         << ::uc_strerror(e) << ::std::endl;
    _mapped = false;
    _good = false;
    return;
  }
  ::banal::log::log("MAP: ", *this, " is mapped.");
  _mapped = true;
  _good = true;
}

void Map::unmap(void) {
  if (!_mapped) {
    ::banal::log::cwarn() << "Cannot unmap " << *this << ": not mapped."
                          << ::std::endl;
  }
  if (auto e = ::uc_mem_unmap(_uc, _address, _size); e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to unmap " << *this << ": "
                         << ::uc_strerror(e) << ::std::endl;
    _good = false;
  } else {
    _mapped = false;
    _good = true;
    ::banal::log::log("MAP: ", *this, " is unmapped.");
  }
}

void Map::protect(::std::uint32_t perms) {
  if (!_mapped) {
    ::banal::log::cwarn() << "Cannot protect " << *this << ": not mapped."
                          << ::std::endl;
  }
  if (auto e = ::uc_mem_protect(_uc, _address, _size, perms);
      e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to protect " << *this << ": "
                         << ::uc_strerror(e) << ::std::endl;
    _good = false;
  } else {
    _good = true;
    _perms = perms;
  }
}

Map::~Map(void) {
  if (_mapped) {
    this->unmap();
  }
}

::std::ostream& operator<<(::std::ostream& os,
                           const ::banal::execution::Map& m) {
  os << "Map@" << &m << "(0x" << ::std::hex << m.address() << "->0x"
     << ::std::hex << m.address() + m.size() << ", " << ::std::dec << m.size()
     << ", ";
  if (m.perms() & ::UC_PROT_READ) {
    os << "READ, ";
  }
  if (m.perms() & ::UC_PROT_WRITE) {
    os << "WRITE, ";
  }
  if (m.perms() & ::UC_PROT_EXEC) {
    os << "EXEC";
  }
  os << ")";
  return os;
}

} // end namespace execution
} // end namespace banal
