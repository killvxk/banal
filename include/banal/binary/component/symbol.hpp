///
/// \file
/// \brief Symbol specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <string>

#include "banal/conf.hpp"

namespace banal {
namespace binary {
namespace component {

/// Forward declaration
class Section;

/// \brief A symbol
class Symbol {
private:
  /// \brief Index of the symbol
  ::std::size_t _index;

  /// \brief Name of the symbol
  ::std::string _name;

  /// \brief Value of the symbol
  uintarch_t _value;

  /// \brief Size of the symbol
  ::std::size_t _size;

  /// \brief Type
  ::std::uint8_t _type;

  /// \brief Section
  Section& _section;

public:
  /// \brief Constructor
  ///
  /// \param index The index of the symbol
  /// \param name The name of the symbol
  /// \param value The value of the symbol
  /// \param size The size of the symbol
  /// \param type The type of the symbol
  /// \param section The section which hold the symbols
  Symbol(::std::size_t index,
         ::std::string_view name,
         uintarch_t value,
         ::std::size_t size,
         ::std::uint8_t type,
         Section& section);

  /// \brief Copy constructor
  Symbol(const Symbol&) = delete;

  /// \brief Copy operator=
  Symbol operator=(const Symbol&) = delete;

  /// \brief Move constructor
  Symbol(Symbol&&) = default;

  /// \brief Destructor
  ~Symbol(void) = default;

public:
  /// \brief Get the index of the symbol
  ///
  /// \return Index of the symbol
  inline auto index(void) const { return _index; }

  /// \brief Get the name of the symbol
  ///
  /// \return Name of the symbol
  inline ::std::string_view name(void) const { return _name; }

  /// \brief Get the value of the symbol
  ///
  /// \return Value of the symbol
  inline auto value(void) const { return _value; }

  /// \brief Get the size of the symbol
  ///
  /// \return Size of the symbol
  inline auto size(void) const { return _size; }

  /// \brief Get the type of the symbol
  ///
  /// \return Type of the symbol
  inline auto type(void) const { return _type; }

  /// \brief Get the section which holds this symbol
  ///
  /// \return The section which holds this symbol
  inline auto& section(void) const { return _section; }
};

} // end namespace component
} // end namespace binary
} // end namespace banal
