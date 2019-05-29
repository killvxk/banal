///
/// \file
/// \brief Memory map specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>

#include <unicorn/unicorn.h>

namespace banal {
namespace execution {

class Map {
private:
  /// \brief unicorn engine
  ::uc_engine* _uc;

  /// \brief Address
  ::std::uint64_t _address;

  /// \brief Size
  ::std::size_t _size;

  /// \brief permissions (unicorn permissions)
  ::std::uint32_t _perms;

  /// \brief Is good
  bool _good;

  /// \brief Is mapped
  bool _mapped;

public:
  /// \brief Constructor
  ///
  /// \param uc Unicorn engine
  /// \param address Address
  /// \param size Size
  /// \param perms Permissions
  Map(::uc_engine* uc,
      ::std::uint64_t address,
      ::std::size_t size,
      ::std::uint32_t perms);

  /// \brief Copy constructor
  Map(const Map&) = delete;

  /// \brief Copy operator=
  Map operator=(const Map&) = delete;

  /// \brief Move constructor
  Map(Map&& m) { m._mapped = false; }

  /// \brief Destructor
  ~Map(void);

public:
  /// \brief Get the address
  ///
  /// \return Address
  inline auto address(void) const { return _address; }

  /// \brief Get the size
  ///
  /// \return Size
  inline auto size(void) const { return _size; }

  /// \brief Get the permissions
  ///
  /// \return Permissions
  inline auto perms(void) const { return _perms; }

  /// \brief Is it good
  ///
  /// \return true if it is good, else false
  inline auto good(void) const { return _good; }

  /// \brief Is it mapped
  ///
  /// \return true if it is mapped, else false
  inline auto mapped(void) const { return _mapped; }

public:
  /// \brief Unmap the address
  void unmap(void);

  /// \brief Map the address
  void map(void);

  /// \brief Protect
  ///
  /// \param perms new perms
  void protect(::std::uint32_t perms);
};
::std::ostream& operator<<(::std::ostream& os,
                           const ::banal::execution::Map& m);

} // end namespace execution
} // end namespace banal
