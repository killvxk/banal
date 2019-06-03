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

  /// \brief capstone state
  struct cs {
    /// \brief cursor to the code
    const ::std::uint8_t* cursor;

    /// \brief size of the code
    ::std::size_t size;

    /// \brief virtual address of the instruction
    ::std::uint64_t address;
  } cs;
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
  /// \brief Emulate one instruction
  ///
  /// \return true if success, else false
  bool step(void);
};

} // end namespace execution
} // end namespace banal
