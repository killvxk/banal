///
/// \file
/// \brief Execution engine implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <elfio/elf_types.hpp>

#include "banal/execution/engine.hpp"

namespace banal {
namespace execution {

Engine::Engine(::uc_engine* uc, ::banal::binary::Binary& binary)
    : _uc(uc), _binary(binary), _mem(), _stacks() {
  for (auto it = binary.segments_cbegin(); it != binary.segments_cend(); it++) {
    auto& seg = *it;
    if (seg->type() == PT_LOAD) {
      // loadable segment
      const uint8_t* ptr = _binary.begin() + seg->offset();
      ::std::size_t size = seg->memory_size();
      if ((size % 4096) > 0) {
        size = (1 + (size / 4096)) * 4096;
      }
      ::std::uint32_t perms = 0;
      if (seg->flags() & PF_X) {
        perms |= ::UC_PROT_EXEC;
      }
      if (seg->flags() & PF_R) {
        perms |= ::UC_PROT_READ;
      }
      if (seg->flags() & PF_W) {
        perms |= ::UC_PROT_WRITE;
      }
      auto vaddr = seg->virtual_address() & 0xFFFFFFFFFFFFF000;
      //_mem.emplace_back(_uc, vaddr, size, perms);
      _mem.emplace_back(_uc, vaddr, size, UC_PROT_ALL);
      Map& m = _mem.back();
      if (!m.good()) {
        return;
      }
      if (auto e = ::uc_mem_write(_uc,
                                  seg->virtual_address(),
                                  reinterpret_cast< const void* >(ptr),
                                  seg->file_size());
          e != ::UC_ERR_OK) {
        ::banal::log::cerr()
            << "Unable to copy data " << ::std::dec << seg->file_size()
            << " bytes from binary (at offset 0x" << ::std::hex << seg->offset()
            << ')' << " to memory map " << m << " at 0x"
            << seg->virtual_address() << ": " << ::uc_strerror(e)
            << ::std::endl;
        return;
      }
      ::banal::log::log("ENGINE: copy ",
                        std::dec,
                        seg->file_size(),
                        " bytes from binary (",
                        ::std::hex,
                        reinterpret_cast< const void* >(ptr),
                        ") to Map ",
                        m,
                        " at 0x",
                        ::std::hex,
                        seg->virtual_address());
    }
  }
}

} // end namespace execution
} // end namespace banal
