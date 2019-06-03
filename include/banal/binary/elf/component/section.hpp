///
/// \file
/// \brief Elf section specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <elfio/elfio.hpp>

#include "banal/binary/component/section.hpp"

namespace banal {
namespace binary {
namespace component {

/// \brief An ELF section
class ELFSection : public Section {
private:
  /// \brief The elfio section
  ::ELFIO::section& _section;

  /// \brief Name of the section
  ::std::string _name;

  /// \brief The symbols
  ::std::vector< Symbol > _symbols;

public:
  /// \brief Constructor
  ///
  /// \param index Index of the section
  /// \param sec Pointer to the elfio section object
  ELFSection(const ::ELFIO::elfio& file,
             ::std::uint16_t index,
             ::ELFIO::section* sec);

  /// \brief Copy constructor
  ELFSection(const ELFSection&) = delete;

  /// \brief Copy operator=
  ELFSection operator=(const ELFSection&) = delete;

  /// \brief Move constructor
  ELFSection(ELFSection&&) = default;

  /// \brief Desctructor
  ~ELFSection(void) override {}

public:
  ::std::string_view name(void) const override;
  ::std::uint32_t type(void) const override;
  ::std::uint64_t flags(void) const override;
  ::std::uint32_t info(void) const override;
  ::std::uint32_t link(void) const override;
  ::std::uint64_t align(void) const override;
  ::std::size_t entry_size(void) const override;
  ::std::uint64_t address(void) const override;
  ::std::size_t size(void) const override;
  const ::std::uint8_t* data(void) const override;

public:
  ::std::vector< Symbol >::const_iterator symbols_cbegin(void) override {
    return _symbols.cbegin();
  }
  ::std::vector< Symbol >::const_iterator symbols_cend(void) override {
    return _symbols.cend();
  }
};

} // end namespace component
} // end namespace binary
} // end namespace banal
