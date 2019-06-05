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

#include <capstone/capstone.h>
#include <unicorn/unicorn.h>

#include "banal/binary/binary.hpp"
#include "banal/execution/map.hpp"
#include "banal/execution/stack.hpp"

namespace banal {
namespace execution {

/// \brief Current state
struct State {
  /// \brief Begin
  ::std::uint64_t begin;

  /// \brief Until
  ::std::uint64_t end;
};

class Engine {
private:
  /// \brief Unicorn handler
  ::uc_engine* _uc;

  /// \brief Capstone handler
  ::csh _csh;

  /// \brief Capstone instruction
  ::cs_insn* _insn;

  /// \brief Binary
  ::banal::binary::Binary& _binary;

  /// \brief Mapped memory
  ::std::vector< Map > _mem;

  /// \brief Stacks
  ::std::vector< Stack > _stacks;

  /// \brief State
  struct State _state;

public:
  /// \brief Constructor
  ///
  /// \param engine Execution engine
  /// \param csh Capstone engine
  /// \param binary Binary
  Engine(::uc_engine* engine, ::csh csh, ::banal::binary::Binary& binary);

  /// \brief Copy constructor
  Engine(const Engine&) = delete;

  /// \brief Copy operator=
  Engine operator=(const Engine&) = delete;

  /// \brief Move constructor
  Engine(Engine&&) = default;

  /// \brief Destructor
  ~Engine(void);

private:
  /// \brief Load a segment
  ///
  /// \return true if success, else false
  bool load_segment(::banal::binary::component::Segment& segment);

public:
  /// \brief Emulate the code
  ///
  /// \return true if success, else false
  bool emulate(void);

  /// \brief Stop emulation
  void stop(void);

public:
  /// \brief Intercept each insn
  static void hook_insn(::uc_engine* uc,
                        ::std::uint64_t address,
                        ::std::uint32_t size,
                        void* user_data);

private:
  /// \brief Intercept insn
  void hook_insn(uintarch_t address, ::std::size_t size);
};

} // end namespace execution
} // end namespace banal
