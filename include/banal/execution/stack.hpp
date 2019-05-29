///
/// \file
/// \brief Stack behavior specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <iomanip>
#include <type_traits>
#include <variant>

#include <unicorn/unicorn.h>

#include "banal/conf.hpp"
#include "banal/util/log.hpp"

namespace banal {
namespace execution {

/// \brief A Stack
class Stack {
protected:
  /// \brief Unicorn engine
  ::uc_engine* _uc;

  /// \brief Base address of the stack (top)
  uintarch_t _base;

  /// \brief Size of the stack
  ::std::size_t _size;

protected:
  /// \brief Current stack pointer
  uintarch_t _sp;

public:
  /// \brief Constructor
  ///
  /// \param base The base address of the stack (bottom)
  /// \param size Size of the stack (can be 0)
  Stack(::uc_engine* uc, uintarch_t base, ::std::size_t size)
      : _uc(uc), _base(base), _size(size), _sp(base) {}

  /// \brief Copy constructor
  Stack(const Stack&) = delete;

  /// \brief Move constructor
  Stack(Stack&&) = default;

  /// \brief Destructor
  ~Stack(void) = default;

public:
  /// \brief push a value on the stack
  ///
  /// \param value Value to push
  ///
  /// \return true if success, else false
  bool push(uintarch_t value);

  /// \brief Pop a value from the stack
  ///
  /// \param value Container for the value
  ///
  /// \return true if success, else false
  bool pop(uintarch_t& value);

  /// \brief Write arbitrary value on stack
  ///
  /// \param value Value to write
  /// \param size The size of the value to write
  ///
  /// \return true if success, else false
  template < typename T >
  bool write(T value, ::std::size_t size, uintarch_t at) {
    if (auto e = ::uc_mem_write(_uc,
                                static_cast<::std::uint64_t >(at),
                                reinterpret_cast< const void* >(value),
                                size);
        e != ::UC_ERR_OK) {
      ::banal::log::cerr() << "Unable to write data at 0x" << ::std::hex << at
                           << " (write size: " << size
                           << "): " << ::uc_strerror(e) << ::std::endl;
      return false;
    }
    return true;
  }

public:
  /// \brief Get the SP
  ///
  /// \return Stack pointer
  inline auto sp(void) const { return _sp; }

  /// \brief Set the sp
  ///
  /// \param value New sp value
  inline void sp(uintarch_t value) { _sp = value; }

  // \brief Get the base of the stack
  ///
  /// \return Base of the stack
  inline auto base(void) const { return _base; }

  /// \brief Get the size of the stack
  ///
  /// \return Size of the stack
  inline auto size(void) const { return _size; }
};

} // end namespace execution
} // end namespace banal
