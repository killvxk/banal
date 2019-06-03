///
/// \file
/// \brief Binary section specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "banal/binary/component/symbol.hpp"

namespace banal {
namespace binary {
namespace component {

// Forward declaration
class Segment;

/// \brief A section
class Section {
private:
  /// \brief The index of the section
  ::std::uint16_t _index;

public:
  /// \brief Constructor
  ///
  /// \param index Index of the section
  Section(::std::uint16_t index) : _index(index) {}

  /// \brief Copy constructor
  Section(const Section&) = delete;

  /// \brief Move constructor
  Section(Section&&) = default;

  /// \brief Destructor
  virtual ~Section(void) {}

public:
  /// \brief Get the name of the section
  ///
  /// \return Name of the section
  virtual ::std::string_view name(void) const = 0;

  /// \brief Get the type of the section
  ///
  /// \return Type of the section
  virtual ::std::uint32_t type(void) const = 0;

  /// \brief Get the flags of the section
  ///
  /// \return Flags of the section
  virtual ::std::uint64_t flags(void) const = 0;

  /// \brief Get the info of the section
  ///
  /// \return Info of the section
  virtual ::std::uint32_t info(void) const = 0;

  /// \brief Get the link of the section
  ///
  /// \return Link of the section
  virtual ::std::uint32_t link(void) const = 0;

  /// \brief Get the address alignment of the section
  ///
  /// \return The address alignment of the section
  virtual ::std::uint64_t align(void) const = 0;

  /// \brief Get the entry size of the section
  ///
  /// \return Entry size of the section
  virtual ::std::size_t entry_size(void) const = 0;

  /// \brief Get the address of the section
  ///
  /// \return Address of the section
  virtual ::std::uint64_t address(void) const = 0;

  /// \brief Get the size of the section
  ///
  /// \return Size of the section
  virtual ::std::size_t size(void) const = 0;

  /// \brief Get the data of the section
  ///
  /// \return Pointer to the data of the section
  virtual const ::std::uint8_t* data(void) const = 0;

public:
  /// \brief Get index of the section
  ///
  /// \return Index of the section
  inline auto index(void) const { return _index; }

public:
  /// \brief Iterator to the first symbol
  ///
  /// \return Iterator to the first symbol
  virtual ::std::vector< Symbol >::const_iterator symbols_cbegin(void) = 0;

  /// \brief Iterator end of the symbols
  ///
  /// \return Iterator end of the symbols
  virtual ::std::vector< Symbol >::const_iterator symbols_cend(void) = 0;
};

} // end namespace component
} // end namespace binary
} // end namespace banal
