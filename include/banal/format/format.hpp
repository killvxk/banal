///
/// \file
/// \brief Executable format specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <memory>
#include <ostream>
#include <string_view>

#include "banal/arch.hpp"
#include "banal/util/log.hpp"
#include "banal/util/mem_based_stream.hpp"

namespace banal {

/// \brief Namespace containing all format-related symbols
namespace format {

/// \brief Executable format specification
enum class Format {
  ELF, ///< ELF
  PE   ///< PE
};

/// \brief Get the string representation of a format
///
/// \param f The format
///
/// \return The string representation of the format f
inline ::std::string_view str(Format f) {
  switch (f) {
    case Format::ELF:
      return "ELF";
    case Format::PE:
      return "PE";
    default:
      log::unreachable("Unreachable");
  }
}

/// \brief An executable parser
class Parser {
  friend ::std::unique_ptr< Parser > get_parser(Format, ::std::istream&);

private:
  /// \brief Tell if parsing was okay
  bool _good;

  /// \brief The input stream
  ::std::istream& _stream;

public:
  /// \brief Constructor
  Parser(::std::istream& stream) : _good(false), _stream(stream) {}

  /// \brief Copy constructor
  Parser(const Parser&) = delete;

  /// \brief Move constructor
  Parser(Parser&&) = default;

  /// \brief Destructor
  virtual ~Parser(void) = default;

protected:
  /// \brief Tell if the parsing was okay
  ///
  /// \return true is no error occured, else false
  inline auto good(void) const { return _good; }

  /// \brief Set good
  ///
  /// \param v The value for good
  inline void set_good(bool v) { _good = v; }

  /// \brief Get the stream
  auto& stream(void) { return _stream; }

public:
  /// \brief Get the format handled by this parser
  ///
  /// \return The format currently handled by the parser
  virtual Format format(void) const = 0;

  /// \brief Get the architecture
  ///
  /// \return The architecture targeted by the executable
  virtual ::banal::Architecture architecture(void) const = 0;

  /// \brief Get the memory address of the entry point
  ///
  /// \return The memory address of the entry point
  virtual ::std::uintptr_t entry(void) const = 0;
};

/// \brief Get the right parser for a given format
///
/// \param f The executable format which is going to be parsed
///
/// \return The parser, if exists, else nullptr
::std::unique_ptr< Parser > get_parser(Format f, ::std::istream& stream);

} // end namespace format
} // end namespace banal

/// \brief Display a format
///
/// \param f The format to display
///
/// \return The output stream
::std::ostream& operator<<(::std::ostream&, ::banal::format::Format f);
