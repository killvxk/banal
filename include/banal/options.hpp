///
/// \file
/// \brief Analysis options specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "banal/architecture.hpp"
#include "banal/format.hpp"

namespace banal {

/// \brief Options for analysis
class Options {
private:
  /// \brief The file to analyze
  ::std::string_view _filepath;

  /// \brief Arguments vector
  ::std::vector<::std::string > _argv;

  /// \brief Tell if everything is okay
  bool _status;

public:
  /// \brief Constructor
  ///
  /// \param argc Argument counter
  /// \param argv Argument vector
  Options(int argc, char** argv);

  /// \brief Copy constructor
  Options(const Options&) = delete;

  /// \brief Copy constructor=
  Options operator=(const Options&) = delete;

  /// \brief Move constructor
  Options(Options&&) = default;

public:
  /// \brief Get the file path to analyze
  ///
  /// \return The file path to analyze
  inline auto filepath(void) const { return _filepath; }

  /// \brief Get the program arguments
  ///
  /// \return The program arguments
  inline const auto& argv(void) const { return _argv; }

  /// \brief Tell if the parsing is okay or not
  ///
  /// \return true if no error has occured, else false
  inline bool good(void) const { return _status; }
};

} // end namespace banal
