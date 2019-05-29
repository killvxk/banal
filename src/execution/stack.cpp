///
/// \file
/// \brief Stack behavior implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/execution/stack.hpp"
#include "banal/util/log.hpp"

#include "banal/conf.hpp"

namespace banal {
namespace execution {

bool Stack::push(uintarch_t value) {
  if (auto e = ::uc_mem_write(_uc,
                              static_cast<::std::uint64_t >(_sp),
                              reinterpret_cast< const void* >(&value),
                              sizeof(value));
      e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to push value \"" << value
                         << "\" on stack at 0x" << _sp
                         << " (write size: " << sizeof(value)
                         << "): " << ::uc_strerror(e) << ::std::endl;
    return false;
  }
  _sp -= sizeof(value);
  return true;
}

bool Stack::pop(uintarch_t& value) {
  if (auto e = ::uc_mem_read(_uc,
                             static_cast<::std::uint64_t >(_sp),
                             reinterpret_cast< void* >(&value),
                             sizeof(value));
      e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to read value on stack at 0x" << _sp
                         << " (read size: " << sizeof(value)
                         << "): " << ::uc_strerror(e) << ::std::endl;
    return false;
  }
  _sp += sizeof(value);
  return true;
}

} // end namespace execution
} // end namespace banal
