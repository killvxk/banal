///
/// \file
/// \brief ELF parser specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include "banal/format/format.hpp"

#include <elfio/elfio.hpp>

namespace banal {
namespace format {

class ELFParser : public Parser {
private:
  /// \brief ELFIO reader
  ::ELFIO::elfio _reader;

public:
  /// \brief Constructor
  ELFParser(::std::istream& stream);

  /// \brief Copy constructor
  ELFParser(const ELFParser&) = delete;

  /// \brief Copy operator=
  ELFParser operator=(const ELFParser&) = delete;

  /// \brief Move constructor
  ELFParser(ELFParser&&) = default;

  /// \brief Destructor
  ~ELFParser(void) override;

public:
  Format format(void) const override { return Format::ELF; }
  ::banal::Architecture architecture(void) const override;
  ::std::uintptr_t entry(void) const override;
};
} // end namespace format
} // end namespace banal
