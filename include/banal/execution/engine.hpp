///
/// \file
/// \brief Execution engine specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <vector>

#include <unicorn/unicorn.h>

#include "banal/binary/binary.hpp"
#include "banal/execution/map.hpp"
#include "banal/execution/stack.hpp"

namespace banal {
namespace execution {

class Engine {
private:
  /// \brief Unicorn handler
  ::uc_engine* _uc;

  /// \brief Binary
  ::banal::binary::Binary& _binary;

  /// \brief Mapped memory
  ::std::vector< Map > _mem;

  /// \brief Stacks
  ::std::vector< Stack > _stacks;

public:
  /// \brief Constructor
  ///
  /// \param engine Execution engine
  /// \param binary Binary
  Engine(::uc_engine* engine, ::banal::binary::Binary& binary);

  /// \brief Copy constructor
  Engine(const Engine&) = delete;

  /// \brief Copy operator=
  Engine operator=(const Engine&) = delete;

  /// \brief Move constructor
  Engine(Engine&&) = default;

  /// \brief Destructor
  ~Engine(void) = default;

public:
};

} // end namespace execution
} // end namespace banal
