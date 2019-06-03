///
/// \file
/// \brief ELF segment specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <elfio/elfio.hpp>

#include "banal/binary/component/segment.hpp"

namespace banal {
namespace binary {
namespace component {

/// \brief An ELF segment
class ELFSegment : public Segment {
private:
  /// \brief The elfio segment
  const ::ELFIO::segment& _seg;

public:
  /// \brief Constructor
  ///
  /// \param index Index of the segment
  /// \param seg Pointer to the elfio segment object
  ELFSegment(::std::uint16_t index, const ::ELFIO::segment* seg);

  /// \brief Copy constructor
  ELFSegment(const ELFSegment&) = delete;

  /// \brief Copy operator=
  ELFSegment operator=(const ELFSegment&) = delete;

  /// \brief Move constructor
  ELFSegment(ELFSegment&&) = default;

  /// \brief Destructor
  ~ELFSegment(void) override {}

public:
  ::std::size_t offset(void) const override;
  ::std::uint32_t type(void) const override;
  ::std::uint32_t flags(void) const override;
  uintarch_t align(void) const override;
  uintarch_t virtual_address(void) const override;
  uintarch_t physical_address(void) const override;
  ::std::size_t file_size(void) const override;
  ::std::size_t memory_size(void) const override;
  const ::std::uint8_t* data(void) const override;
};

} // end namespace component
} // end namespace binary
} // end namespace banal
