///
/// \file
/// \brief Source file implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "banal/binary/binary.hpp"
#include "banal/binary/elf/elf.hpp"
#include "banal/util/log.hpp"

namespace banal {

/// \brief Namespace containing all stuffs about binaries
namespace binary {

namespace {

void safe_close(int fd) {
  if (::close(fd) == -1) {
    log::cerr() << "Unable to safely close the file descriptor: "
                << ::std::strerror(errno) << ::std::endl;
  }
}

} // end anonymous namespace

Binary::Binary(const Options& opt, int fd, void* addr, ::std::size_t file_len)
    : _begin(reinterpret_cast<::std::uint8_t* >(addr)),
      _end(reinterpret_cast<::std::uint8_t* >(addr) + file_len),
      _fd(fd),
      _stream(reinterpret_cast<::std::uint8_t* >(addr), file_len),
      _opt(opt),
      _good(false) {}

::std::unique_ptr< Binary > open(const ::banal::Options& opt) {
  auto fd = ::open(opt.filepath().data(), O_RDONLY);
  if (fd == -1) {
    // Cannot open file. Abort.
    log::cerr() << "Unable to open " << opt.filepath() << ": "
                << ::std::strerror(errno) << ::std::endl;
    return nullptr;
  }

  struct stat file_stat;
  if (fstat(fd, &file_stat) != 0) {
    // Cannnot get stats about file. Abort.
    log::cerr() << "Cannot exec fstat on " << opt.filepath() << ::std::endl;
    safe_close(fd);
    return nullptr;
  }

  if (S_ISREG(file_stat.st_mode) == 0) {
    // Not a regular file. Abort.
    log::cerr() << "" << opt.filepath() << " is not a regular file."
                << ::std::endl;
    safe_close(fd);
    return nullptr;
  }

  // Map the address
  // We do not want a special virtual address
  // We are going to read it only
  // This is private, not shared
  void* addr = ::mmap(nullptr,
                      static_cast<::std::size_t >(file_stat.st_size),
                      PROT_READ,
                      MAP_PRIVATE,
                      fd,
                      0);
  if (addr == nullptr) {
    log::cerr() << "Unable to map file " << opt.filepath() << ": "
                << ::std::strerror(errno) << ::std::endl;
    safe_close(fd);
    return nullptr;
  }

  switch (opt.format()) {
    case Format::ELF: {
      ::std::unique_ptr< binary::Binary > bin =
          ::std::make_unique< binary::ELFBinary >(opt,
                                                  fd,
                                                  addr,
                                                  static_cast<::std::size_t >(
                                                      file_stat.st_size));
      if (bin->parse()) {
        return bin;
      } else {
        return nullptr;
      }
    }
    case Format::PE: {
      log::cerr() << "PE is not supported yet" << ::std::endl;
      return nullptr;
    }
    default: {
      log::unreachable("Unreachable");
    }
  }
}

Binary::~Binary(void) {
  safe_close(_fd);
}

} // namespace binary
} // end namespace banal
