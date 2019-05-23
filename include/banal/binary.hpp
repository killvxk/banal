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

#include "banal/util/mem_based_stream.hpp"

namespace banal {

/// \brief A binary to analyze
class Binary {
private:
  /// \brief Address of the begin of the file
  ::std::uint8_t* _begin;

  /// \brief Address of the end of the file
  ::std::uint8_t* _end;

  /// \brief File descriptor
  int _fd;

  /// \brief Mem based stream
  util::MemBasedStream _stream;

public:
  /// \brief Constructor
  ///
  /// \param fd File descriptor
  /// \param addr Address of the file
  /// \param file_len Length of the file
  Binary(int fd, void* addr, ::std::size_t file_len);

  /// \brief Copy constructor
  Binary(const Binary&) = delete;
  Binary operator=(const ::banal::Binary&) = delete;

  /// \brief Move constructor
  Binary(Binary&&) = delete;

  /// \brief Destructor
  ~Binary(void);

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
  inline const auto* begin(void) const { return _begin; }

  /// \brief Get the end of the section to analyze
  ///
  /// \return The end of the section to analyze
  inline const auto* end(void) const { return _end; }

  /// \brief Get the memory based stream
  ///
  /// \return The memory based stream
  inline auto& stream(void) { return _stream; }

  inline uint8_t operator[](::std::size_t off) { return *(_begin + off); }

public:
  /// \brief Open a binary
  ///
  /// \param filepath The path to the binary
  ///
  /// \return A unique pointer to a Binary object, or nullptr if an error has
  /// occured
  static ::std::unique_ptr< Binary > open(const ::std::string_view filepath);
};

} // end namespace banal
