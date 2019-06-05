///
/// \file
/// \brief Source file specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <iostream>
#include <istream>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "banal/architecture.hpp"
#include "banal/binary/component/section.hpp"
#include "banal/binary/component/segment.hpp"
#include "banal/format.hpp"
#include "banal/options.hpp"
#include "banal/util/mem_based_stream.hpp"

namespace banal {
namespace binary {

/// \brief A binary to analyze
class Binary {
  friend ::std::unique_ptr< Binary > open(const ::banal::Options& opt);

private:
  /// \brief Address of the begin of the file
  ::std::uint8_t* _begin;

  /// \brief Address of the end of the file
  ::std::uint8_t* _end;

  /// \brief File descriptor
  int _fd;

protected:
  /// \brief Mem based stream
  util::MemBasedStream _stream;

  /// \brief Options
  const ::banal::Options& _opt;

  /// \brief Tell if this is good
  bool _good;

public:
  /// \brief Constructor
  ///
  /// \param fd File descriptor
  /// \param addr Address of the file
  /// \param file_len Length of the file
  Binary(const ::banal::Options& opt,
         int fd,
         void* addr,
         ::std::size_t file_len);

  /// \brief Copy constructor
  Binary(const Binary&) = delete;

  /// \brief Move constructor
  Binary(Binary&&) = delete;

  /// \brief Destructor
  virtual ~Binary(void);

public:
  /// \brief Get the size of the section to analyze
  ///
  /// \return Size of the section to analyze
  inline auto size(void) const {
    return static_cast<::std::size_t >(_end - _begin);
  }

  /// \brief Get the mapped address of the binary
  ///
  /// \return The mapped address of the binary
  inline auto* begin(void) const { return _begin; }

  /// \brief Get the end of the section to analyze
  ///
  /// \return The end of the section to analyze
  inline auto* end(void) const { return _end; }

  /// \brief Get the memory based stream
  ///
  /// \return The memory based stream
  inline auto& stream(void) { return _stream; }

  /// \brief Tell if this is good or not
  ///
  /// \return true if it is good, else false
  inline auto good(void) const { return _good; }

  inline ::std::uint8_t* operator[](::std::size_t off) {
    return (_begin + off);
  }

public:
  /// \brief Get the format of the current binary
  ///
  /// \return The format of the binary
  virtual ::banal::Format format(void) const = 0;

  /// \brief Get the targeted architecture of the current binary
  ///
  /// \return The targeted architecture of the current binary
  virtual ::banal::Architecture architecture(void) const = 0;

  /// \brief Get the entry point of the binary
  ///
  /// \return The entry point of the binary
  virtual uintarch_t entry(void) const = 0;

  /// \brief Set the entry point of the binary
  ///
  /// \param e Entry point of the binary
  virtual void set_entry(uintarch_t e) = 0;

protected:
  /// \brief Parse the binary, in order to get all the needed information
  ///
  /// \return true if everything is okay, else false
  virtual bool parse(void) = 0;

public:
  /// \brief Dump informations about the binary
  virtual void dump(void) const = 0;

  /// \brief Begin const iterator for segments
  ///
  /// \return Begin const iterator for segments
  virtual ::std::vector<
      ::std::unique_ptr< component::Segment > >::const_iterator
  segments_cbegin(void) const = 0;

  /// \brief End const iterator for segments
  ///
  /// \return End const iterator for segments
  virtual ::std::vector<
      ::std::unique_ptr< component::Segment > >::const_iterator
  segments_cend(void) const = 0;

  /// \brief Begin const iterator for sections
  ///
  /// \return Begin const iterator for sections
  virtual ::std::vector<
      ::std::unique_ptr< component::Section > >::const_iterator
  sections_cbegin(void) const = 0;

  /// \brief End const iterator for sections
  ///
  /// \return End const iterator for sections
  virtual ::std::vector<
      ::std::unique_ptr< component::Section > >::const_iterator
  sections_cend(void) const = 0;

  /// \brief Get symbols map
  ///
  /// \return Symbols map
  virtual const ::std::unordered_map< uintarch_t, const component::Symbol& >&
  symbols(void) const = 0;

public:
  /// \brief Is NX enabled
  ///
  /// \return true if NX is enabled, else false
  virtual bool nx(void) const = 0;

  /// \brief Is PIE
  ///
  /// \return true if PIE, else false
  virtual bool pie(void) const = 0;

public:
  /// \brief Get mapped address of a symbol
  ///
  /// \param sym The symbol
  /// \return The mapped address of the symbol if exists, else nothing
  virtual ::std::optional< uintarch_t > get_address(
      const component::Symbol& sym) const = 0;

  /// \brief Get mapped address of an address
  ///
  /// \param address The address
  ///
  /// \return The mapped address of the address
  virtual ::std::optional< uintarch_t > get_address(
      uintarch_t address) const = 0;

  /// \brief Get symbol by virtual address
  ///
  /// \param address Address of the symbol
  ///
  /// \return The symbol if exists, else nothing
  virtual ::std::optional< std::reference_wrapper< const component::Symbol > >
  get_symbol(uintarch_t address) const = 0;
};

/// \brief Open a binary
///
/// \param opt Options
///
/// \return A unique pointer to a Binary object, or nullptr if an error has
/// occured
::std::unique_ptr< Binary > open(const ::banal::Options& opt);

} // end namespace binary
} // end namespace banal
