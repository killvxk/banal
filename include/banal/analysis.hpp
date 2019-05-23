///
/// \file
/// \brief Analysis specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include "banal/arch.hpp"
#include "banal/extern/capstone.hpp"
#include "banal/extern/unicorn.hpp"
#include "banal/options.hpp"

namespace banal {

/// \brief Main analysis class
class Analysis {
private:
  /// \brief Options supplied by the user
  [[maybe_unused]] const Options& _options;

  /// \brief Architecture to analyze
  Architecture _arch;

  /// \brief Capstone handler
  ::csh _csh;

  /// \brief Unicorn handler
  ::uc_engine* _uc;

  /// \brief Tell if it is good
  bool _good;

public:
  /// \brief Constructor
  ///
  /// \param opt Options from the command line
  /// \param arch The targeted architecture
  Analysis(const Options& opt, Architecture arch);

  /// \brief Copy constructor
  Analysis(const Analysis&) = delete;

  /// \brief Copy operator=
  Analysis operator=(const Analysis&) = delete;

  /// \brief Move constructor
  Analysis(Analysis&&) = default;

  /// \brief Destructor
  ~Analysis(void);

public:
  /// \brief Get targeted architecture
  ///
  /// \return Targeted architecture
  inline auto arch(void) const { return _arch; }
};

} // end namespace banal
