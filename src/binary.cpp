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

#include "banal/binary.hpp"
#include "banal/util/log.hpp"

namespace banal {

namespace {

void safe_close(int fd) {
  if (::close(fd) == -1) {
    log::cerr() << "Unable to safely close the file descriptor: "
                << ::std::strerror(errno) << ::std::endl;
  }
}

} // end anonymous namespace

Binary::Binary(int fd, void* addr, ::std::size_t file_len)
    : _begin(reinterpret_cast<::std::uint8_t* >(addr)),
      _end(reinterpret_cast<::std::uint8_t* >(addr) + file_len),
      _fd(fd),
      _stream(reinterpret_cast<::std::uint8_t* >(addr), file_len) {}

::std::unique_ptr< Binary > Binary::open(const ::std::string_view filepath) {
  auto fd = ::open(filepath.data(), O_RDONLY);
  if (fd == -1) {
    // Cannot open file. Abort.
    log::cerr() << "Unable to open " << filepath << ": "
                << ::std::strerror(errno) << ::std::endl;
    return nullptr;
  }

  struct stat file_stat;
  if (fstat(fd, &file_stat) != 0) {
    // Cannnot get stats about file. Abort.
    log::cerr() << "Cannot exec fstat on " << filepath << ::std::endl;
    safe_close(fd);
    return nullptr;
  }

  if (S_ISREG(file_stat.st_mode) == 0) {
    // Not a regular file. Abort.
    log::cerr() << "" << filepath << " is not a regular file." << ::std::endl;
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
    log::cerr() << "Unable to map file " << filepath << ": "
                << ::std::strerror(errno) << ::std::endl;
    safe_close(fd);
    return nullptr;
  }

  return ::std::make_unique< Binary >(fd,
                                      addr,
                                      static_cast<::std::size_t >(
                                          file_stat.st_size));
}

Binary::~Binary(void) {
  safe_close(_fd);
}

} // end namespace banal
