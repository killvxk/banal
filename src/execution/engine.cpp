///
/// \file
/// \brief Execution engine implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <elfio/elf_types.hpp>

#include "banal/architecture.hpp"
#include "banal/execution/engine.hpp"

namespace banal {
namespace execution {

bool Engine::load_segment(::banal::binary::component::Segment& seg) {
  // Compute the size of the page
  ::std::size_t size = seg.memory_size();
  if ((size % 4096) > 0) {
    size = (1 + (size / 4096)) * 4096;
  }
  ::std::uint32_t perms = 0;
  if (seg.flags() & PF_X) {
    perms |= ::UC_PROT_EXEC;
  }
  if (seg.flags() & PF_R) {
    perms |= ::UC_PROT_READ;
  }
  if (seg.flags() & PF_W) {
    perms |= ::UC_PROT_WRITE;
  }
  auto vaddr = seg.virtual_address() & 0xFFFFFFFFFFFFF000;

  if ((seg.virtual_address() + seg.file_size()) > (vaddr + size)) {
    // 4KB is not enough
    size += 4096;
  }
  _mem.emplace_back(_uc, vaddr, size, perms);
  Map& m = _mem.back();
  if (!m.good()) {
    return false;
  }

  if (auto e = ::uc_mem_write(_uc,
                              seg.virtual_address(),
                              reinterpret_cast< const void* >(_binary.begin() +
                                                              seg.offset()),
                              seg.file_size());
      e != ::UC_ERR_OK) {
    // fail to copy data from file to mapped memory
    ::banal::log::cerr() << "Unable to copy data " << ::std::dec
                         << seg.file_size()
                         << " bytes from binary (at offset 0x" << ::std::hex
                         << seg.offset() << ')' << " to memory map " << m
                         << " at 0x" << seg.virtual_address() << ": "
                         << ::uc_strerror(e) << ::std::endl;
    return false;
  }
  ::banal::log::log("Segment ", ::std::dec, seg.index(), " mapped.");
  ::banal::log::log("ENGINE: copy ",
                    std::dec,
                    seg.file_size(),
                    " bytes from binary (",
                    ::std::hex,
                    reinterpret_cast< const void* >(_binary.begin() +
                                                    seg.offset()),
                    ") to Map ",
                    m,
                    " at 0x",
                    ::std::hex,
                    seg.virtual_address());
  return true;
}

Engine::Engine(::uc_engine* uc, ::csh csh, ::banal::binary::Binary& binary)
    : _uc(uc),
      _csh(csh),
      _insn(nullptr),
      _binary(binary),
      _mem(),
      _stacks(),
      _state{binary.entry(), 0} {
  for (auto it = binary.segments_cbegin(); it != binary.segments_cend(); it++) {
    // Load loadable segments
    auto& seg = *it;
    if (seg->type() == PT_LOAD) {
      // loadable segment
      if (!this->load_segment(*seg)) {
        return;
      }
    }
  }
  ::banal::log::cgood() << "Segments loaded successfully in RAM."
                        << ::std::endl;

  // Find entry symbol
  // Find main
  bool f = false;
  for (auto& [address, symbol] : binary.symbols()) {
    if (symbol.name() == "main") {
      auto file_offset = binary.get_address(symbol);
      if (!file_offset) {
        ::banal::log::cwarn() << "Symbol cannot be located" << ::std::endl;
      } else {
        _state.begin = symbol.value();
        _state.end = _state.begin + symbol.size();
        _binary.set_entry(symbol.value());
        ::banal::log::log("Entry symbol: ",
                          symbol.name(),
                          ", size=0x",
                          ::std::hex,
                          symbol.size(),
                          ", virtual address = 0x",
                          ::std::hex,
                          symbol.value(),
                          ", offset file = 0x",
                          *file_offset);
        f = true;
      }
    }
  }
  if (!f) {
    ::banal::log::cerr() << "Unable to find entry symbol." << ::std::endl;
    return;
  }

  // init capstone
  ::cs_option(_csh, ::CS_OPT_DETAIL, ::CS_OPT_ON);
  _insn = ::cs_malloc(_csh);
  if (!_insn) {
    ::banal::log::cerr() << "Unable to malloc a capstone instruction: "
                         << ::cs_strerror(::cs_errno(_csh)) << ::std::endl;
    return;
  }

  // init stack
  uintarch_t stack_addr = ::banal::stack();
  ::std::uint32_t perms = ::UC_PROT_READ | ::UC_PROT_WRITE;
  if (_binary.nx()) {
    perms |= ::UC_PROT_WRITE;
  }
  _mem.emplace_back(_uc, stack_addr, 4096, perms);
  stack_addr += 4096 - sizeof(uintarch_t) * 10;
  switch (binary.architecture()) {
    case ::banal::Architecture::X86: {
      ::uc_reg_write(_uc, ::UC_X86_REG_ESP, &stack_addr);
    } break;
    case ::banal::Architecture::X86_64: {
      ::uc_reg_write(_uc, ::UC_X86_REG_RSP, &stack_addr);
    } break;
    default: {
      ::banal::log::unreachable("Unreachable");
    }
  }

  // hook code
  ::uc_hook hh;
  ::uc_cb_hookcode_t code_hook = Engine::hook_insn;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
  if (auto e = ::uc_hook_add(_uc,
                             &hh,
                             ::UC_HOOK_CODE,
                             reinterpret_cast< void* >(code_hook),
                             static_cast< void* >(this),
                             1,
                             0);
      e != ::UC_ERR_OK) {
#pragma clang diagnostic pop
    ::banal::log::cerr() << "Unable to register code hook: " << ::uc_strerror(e)
                         << ::std::endl;
    return;
  }
}

bool Engine::emulate(void) {
  auto e = ::uc_emu_start(_uc, _state.begin, _state.end, 0, 0);
  if (e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable emulate code: " << ::uc_strerror(e)
                         << ::std::endl;
    return false;
  }
  return true;
  /*if (auto b = ::cs_disasm_iter(_csh,
                                &_state.cs.cursor,
                                &_state.cs.size,
                                &_state.cs.address,
                                _insn);
      !b) {
    ::banal::log::cerr() << "Unable to disassemble instruction at "
                         << static_cast< const void* >(_state.cs.cursor) << ": "
                         << ::cs_strerror(::cs_errno(_csh)) << ::std::endl;
    return false;
  }*/
}

Engine::~Engine(void) {
  if (_insn) {
    ::cs_free(_insn, 1);
  }
}

void Engine::stop(void) {
  if (auto e = ::uc_emu_stop(_uc); e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to stop emulation: " << ::uc_strerror(e)
                         << ::std::endl;
  }
}

void Engine::hook_insn(::uc_engine*,
                       ::std::uint64_t address,
                       ::std::uint32_t size,
                       void* user_data) {
  Engine* e = static_cast< Engine* >(user_data);
  e->hook_insn(static_cast< uintarch_t >(address),
               static_cast<::std::size_t >(size));
}

void Engine::hook_insn(uintarch_t address, ::std::size_t size) {
  ::std::vector<::std::uint8_t > insn_buffer;
  insn_buffer.reserve(size);
  if (auto e = ::uc_mem_read(_uc,
                             static_cast<::std::uint64_t >(address),
                             insn_buffer.data(),
                             size);
      e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to read instruction at 0x" << ::std::hex
                         << address << ": " << ::uc_strerror(e) << ::std::endl;
    this->stop();
    return;
  }
  ::std::uint64_t addr = static_cast<::std::uint64_t >(address);
  const ::std::uint8_t* data = insn_buffer.data();
  if (auto b = ::cs_disasm_iter(_csh, &data, &size, &addr, _insn); !b) {
    ::banal::log::cerr() << "Unable to disassemble instruction at 0x"
                         << ::std::hex << address << ": "
                         << ::cs_strerror(::cs_errno(_csh)) << ::std::endl;
    this->stop();
    return;
  }
  ::banal::log::cinfo() << CODE_YELLOW << "[0x" << ::std::hex << address
                        << "]> " << CODE_RESET << _insn->mnemonic << '\t'
                        << _insn->op_str << ::std::endl;
}

} // end namespace execution
} // end namespace banal
