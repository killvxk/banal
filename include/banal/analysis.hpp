///
/// \file
/// \brief Analysis specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <optional>

#include "banal/binary/binary.hpp"
#include "banal/execution/stack.hpp"
#include "banal/extern/capstone.hpp"
#include "banal/extern/unicorn.hpp"
#include "banal/options.hpp"

namespace banal {

/// \brief Main analysis class
class Analysis {
private:
  /// \brief Options supplied by the user
  [[maybe_unused]] const Options& _options;

  /// \brief Capstone handler
  ::csh _csh;

  /// \brief Unicorn handler
  ::uc_engine* _uc;

  /// \brief Mapped binary, if any
  /// Optional reference IS ILLEGAL
  binary::Binary& _binary;

  /// \brief Virtal address of the binary
  ::std::optional<::std::uint64_t > _virtual_binary_address;

  /// \brief stack memory address
  ::std::optional<::std::uint64_t > _stack_address;

  /// \brief Size of stack
  ::std::optional<::std::size_t > _stack_size;

  /// \brief Stack Frame
  ::std::vector<::std::unique_ptr< execution::Stack > > _stacks;

  /// \brief Tell if it is good
  bool _good;

public:
  /// \brief Constructor
  ///
  /// \param opt Options from the command line
  /// \param binary The binary
  Analysis(const Options& opt, binary::Binary& binary);

  /// \brief Copy constructor
  Analysis(const Analysis&) = delete;

  /// \brief Copy operator=
  Analysis operator=(const Analysis&) = delete;

  /// \brief Move constructor
  Analysis(Analysis&&) = default;

  /// \brief Destructor
  ~Analysis(void);

public:
  /// \brief Get the mapped binary, if any
  ///
  /// \return The binary
  inline auto& binary(void) const { return _binary; }

  /// \brief Map the binary
  ///
  /// \param address Address for the mapping
  ///
  /// \return true if no error occured, else false
  bool map(::std::uint64_t address);

  /// \brief Set the stack
  ///
  /// \param address Address of the stack (bottom)
  /// \param size Size of the stack
  ///
  /// \return true if no error occured, else false
  bool set_stack(::std::uint64_t address, ::std::size_t size);

  /// \brief Start the analysis, beginning at the entry point given by the
  /// binary
  inline void start(void) { this->start(_binary.entry()); }

  /// \brief Start the analysis, beginning at an entry point
  ///
  /// \param entry the analysis, at entry given by entry
  void start(::std::uint64_t entry);

private:
  /// \brief Get a new stack
  ///
  /// \return New stack
  ::std::unique_ptr< execution::Stack > new_stack(::std::size_t size = 1024 *
                                                                       4);
};

} // end namespace banal
