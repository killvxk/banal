///
/// \file
/// \brief Binary segment specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <cstdint>
#include <optional>

#include "banal/conf.hpp"

namespace banal {
namespace binary {
namespace component {

/// \brief A segment
class Segment {
private:
  /// \brief Index of the segment
  ::std::uint16_t _index;

public:
  /// \brief Constructor
  ///
  /// \param index Index of the segment
  Segment(::std::uint16_t index) : _index(index) {}

  /// \brief Copy constructor
  Segment(const Segment&) = delete;

  /// \brief Move constructor
  Segment(Segment&&) = default;

  /// \brief Destructor
  virtual ~Segment(void) {}

public:
  /// \brief Get the offset of the segment
  ///
  /// \return Offset of the segment
  virtual ::std::size_t offset(void) const = 0;

  /// \brief Get the type of the segment
  ///
  /// \return Type of the segment
  virtual ::std::uint32_t type(void) const = 0;

  /// \brief Get the flags of the segment
  ///
  /// \return Flags of the segment
  virtual ::std::uint32_t flags(void) const = 0;

  /// \brief Get aligment on the segment
  ///
  /// \return Alignments of the segment
  virtual uintarch_t align(void) const = 0;

  /// \brief Get virtual address of the segment
  ///
  /// \return Virtual address of the segment
  virtual uintarch_t virtual_address(void) const = 0;

  /// \brief Get the physical address of the segment
  ///
  /// \return Physical address of the segment
  virtual uintarch_t physical_address(void) const = 0;

  /// \brief Get the file size of the segment
  ///
  /// \return File size of the segment
  virtual ::std::size_t file_size(void) const = 0;

  /// \brief Get the memory size of the segment
  ///
  /// \return Memory size of the segment
  virtual ::std::size_t memory_size(void) const = 0;

  /// \brief Get the data of the segment
  ///
  /// \return The data of the segment
  virtual const ::std::uint8_t* data(void) const = 0;

public:
  /// \brief Get the index of the segment
  ///
  /// \return The index of the segment
  inline auto index(void) const { return _index; }

  /// \brief Check if address is contained inside the segment
  ///
  /// \param address Address to check
  ///
  /// \return The offset of the virtual address in the file
  inline std::optional< uintarch_t > contains(uintarch_t address) {
    if (address >= this->virtual_address() &&
        address <= this->virtual_address() + this->memory_size()) {
      auto offset = address - this->virtual_address();
      return this->offset() + offset;
    }
    return ::std::nullopt;
  }
};

} // end namespace component
} // end namespace binary
} // end namespace banal
