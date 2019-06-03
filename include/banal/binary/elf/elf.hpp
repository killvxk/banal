///
/// \file
/// \brief ELF parser specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <unordered_map>

#include <elfio/elfio.hpp>

#include "banal/binary/binary.hpp"
#include "banal/format.hpp"

namespace banal {
namespace binary {

class ELFBinary : public Binary {
private:
  /// \brief ELFIO reader
  ::ELFIO::elfio _reader;

  /// \brief Segments
  ::std::vector<::std::unique_ptr< component::Segment > > _segments;

  /// \brief Sections
  ::std::vector<::std::unique_ptr< component::Section > > _sections;

  /// \brief Symbols
  ::std::unordered_map< uintarch_t, const component::Symbol& > _symbols;

  /// \brief Entry
  uintarch_t _entry;

  /// \brief NX
  bool _nx;

  /// \brief PIE
  bool _pie;

public:
  /// \brief Constructor
  ELFBinary(const ::banal::Options& opt,
            int fd,
            void* addr,
            ::std::size_t file_len);

  /// \brief Copy constructor
  ELFBinary(const ELFBinary&) = delete;

  /// \brief Copy operator=
  ELFBinary operator=(const ELFBinary&) = delete;

  /// \brief Move constructor
  ELFBinary(ELFBinary&&) = delete;

  /// \brief Destructor
  ~ELFBinary(void) override;

public:
  ::banal::Format format(void) const override { return Format::ELF; }
  ::banal::Architecture architecture(void) const override;
  uintarch_t entry(void) const override;
  void set_entry(uintarch_t e) override;

protected:
  bool parse(void) override;

public:
  void dump(void) const override;
  ::std::vector<::std::unique_ptr< component::Segment > >::const_iterator
  segments_cbegin(void) const override;
  ::std::vector<::std::unique_ptr< component::Segment > >::const_iterator
  segments_cend(void) const override;
  ::std::vector<::std::unique_ptr< component::Section > >::const_iterator
  sections_cbegin(void) const override;
  ::std::vector<::std::unique_ptr< component::Section > >::const_iterator
  sections_cend(void) const override;

public:
  inline bool nx(void) const override { return _nx; }
  inline bool pie(void) const override { return _pie; }

public:
  ::std::optional< uintarch_t > get_address(
      const component::Symbol& sym) const override;
  ::std::optional< uintarch_t > get_address(uintarch_t address) const override;
  ::std::optional< std::reference_wrapper< const component::Symbol > >
  get_symbol(uintarch_t address) const override;
};
} // end namespace binary
} // end namespace banal
