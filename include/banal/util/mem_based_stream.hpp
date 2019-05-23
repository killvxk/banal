///
/// \file
/// \brief Memory based stream
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <istream>
#include <streambuf>

namespace banal {
namespace util {

/// \brief Memory based stream
class MemBasedStream : public ::std::streambuf, public ::std::istream {
private:
  /// \brief Begin
  ::std::uint8_t* _begin;

  /// \brief End
  ::std::uint8_t* _end;

  /// \brief Current position
  ::std::uint8_t* _current;

  /// \brief Size
  ::std::size_t _size;

public:
  /// \brief Constructor
  ///
  /// \param begin Begin of the memory
  /// \param size Size of the memory stream
  MemBasedStream(::std::uint8_t* begin, ::std::size_t size);

  /// \brief Copy constructor
  MemBasedStream(const MemBasedStream&) = delete;

  /// \brief Copy operator=
  MemBasedStream operator=(const MemBasedStream&) = delete;

  /// \brief Get the begin
  ///
  /// \return Get the begin address of the memory
  inline auto begin(void) const { return _begin; }

  /// \brief Get the size of the memory stream
  ///
  /// \return The size of the memory stream
  inline auto size(void) const { return _size; }

protected:
  ::std::streambuf::pos_type seekoff(
      ::std::streambuf::off_type off,
      ::std::ios_base::seekdir dir,
      ::std::ios_base::openmode which = ::std::ios_base::in |
                                        ::std::ios_base::out) override;
  ::std::streambuf::pos_type seekpos(
      ::std::streambuf::pos_type pos,
      ::std::ios_base::openmode which = ::std::ios_base::in |
                                        ::std::ios_base::out) override;
};

} // end namespace util
} // end namespace banal
