///
/// \file
/// \brief Memory based stream implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/util/mem_based_stream.hpp"
#include "banal/util/log.hpp"

namespace banal {
namespace util {

MemBasedStream::MemBasedStream(::std::uint8_t* begin, ::std::size_t size)
    : ::std::istream(static_cast< ::std::streambuf* >(this)),
      _begin(begin),
      _end(begin + size),
      _current(begin),
      _size(size) {
  this->setg(reinterpret_cast< char* >(begin),
             reinterpret_cast< char* >(begin),
             reinterpret_cast< char* >(begin) + size);
}

::std::streambuf::pos_type MemBasedStream::seekoff(
    ::std::streambuf::off_type off,
    ::std::ios_base::seekdir dir,
    ::std::ios_base::openmode which) {
  (void)which;
  switch (dir) {
    case ::std::ios_base::beg: {
      _current = _begin + off;
    } break;
    case ::std::ios_base::cur: {
      _current += off;
    } break;
    case ::std::ios_base::end: {
      _current = _end + off;
    } break;
    default: {
      ::banal::log::unreachable("Unreachable");
    }
  }
  if (_current < _begin || _current > _end) {
    return -1;
  }
  this->setg(reinterpret_cast< char* >(_begin),
             reinterpret_cast< char* >(_current),
             reinterpret_cast< char* >(_begin) + _size);
  return _current - _begin;
}

::std::streambuf::pos_type MemBasedStream::seekpos(
    ::std::streambuf::pos_type pos, ::std::ios_base::openmode which) {
  (void)which;
  _current = _begin + pos;

  if (_current < _begin || _current > _end) {
    return -1;
  }
  this->setg(reinterpret_cast< char* >(_begin),
             reinterpret_cast< char* >(_current),
             reinterpret_cast< char* >(_begin) + _size);
  return _current - _begin;
}

} // end namespace util
} // end namespace banal
