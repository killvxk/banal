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
    ::banal::log::cerr() << "Unable to copy data " << ::std::dec
                         << seg.file_size()
                         << " bytes from binary (at offset 0x" << ::std::hex
                         << seg.offset() << ')' << " to memory map " << m
                         << " at 0x" << seg.virtual_address() << ": "
                         << ::uc_strerror(e) << ::std::endl;
  }
  ::banal::log::cinfo() << "Segment " << ::std::dec << seg.index() << " mapped."
                        << ::std::endl;
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
      _state{binary.entry(),
             0,
             {reinterpret_cast< const ::std::uint8_t* >(binary.entry()),
              0,
              binary.entry()}} {
  // Load loadable segments
  for (auto it = binary.segments_cbegin(); it != binary.segments_cend(); it++) {
    auto& seg = *it;
    if (seg->type() == PT_LOAD) {
      // loadable segment
      if (!this->load_segment(*seg)) {
        return;
      }
    }
  }

  // Find entry symbol
  // Find main
  bool f = false;
  for (auto it = binary.sections_cbegin(); it != binary.sections_cend(); it++) {
    const auto& sec = *it;
    for (auto jt = sec->symbols_cbegin(); jt != sec->symbols_cend(); jt++) {
      const auto& sym = *jt;
      if (sym.name() == "main") {
        auto file_offset = binary.get_address(sym);
        if (!file_offset) {
          ::banal::log::cwarn() << "Symbol cannot be located" << ::std::endl;
        } else {
          _state.begin = sym.value();
          _state.end = _state.begin + sym.size();
          _state.cs.address = sym.value();
          _state.cs.size = sym.size();
          _state.cs.cursor = binary.begin() + *file_offset;
          ::banal::log::log("Entry symbol: ",
                            sym.name(),
                            ", size=0x",
                            ::std::hex,
                            sym.size(),
                            ", virtual address = 0x",
                            ::std::hex,
                            sym.value(),
                            ", offset file = 0x",
                            *file_offset);
          _binary.set_entry(sym.value());
          f = true;
          break;
        }
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
}

bool Engine::step(void) {
  if (auto b = ::cs_disasm_iter(_csh,
                                &_state.cs.cursor,
                                &_state.cs.size,
                                &_state.cs.address,
                                _insn);
      !b) {
    ::banal::log::cerr() << "Unable to disassemble instruction at "
                         << static_cast< const void* >(_state.cs.cursor) << ": "
                         << ::cs_strerror(::cs_errno(_csh)) << ::std::endl;
    return false;
  }
  ::banal::log::cinfo() << "Emulating instruction [0x" << ::std::hex
                        << _insn->address << "]> `" << _insn->mnemonic << '\t'
                        << _insn->op_str << '`' << ::std::endl;
  auto e = ::uc_emu_start(_uc, _state.begin, _state.end, 5000, 1);
  if (e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to emulate insn at 0x" << ::std::hex
                         << _state.begin << ": " << ::uc_strerror(e)
                         << ::std::endl;
    return false;
  }
  uintarch_t ip = 0;
  if (e = ::uc_reg_read(_uc,
                        ::banal::get_ip(_binary.architecture()).first,
                        &ip);
      e != ::UC_ERR_OK) {
    ::banal::log::cerr() << "Unable to read IP: " << ::uc_strerror(e)
                         << ::std::endl;
    return false;
  }
  _state.begin = ip;
  _state.cs.address = ip;

  auto new_addr = _binary.get_address(ip);
  if (!new_addr) {
    ::banal::log::cerr() << "Address 0x" << ::std::hex << ip
                         << " is not mapped.in the file." << ::std::endl;
    return false;
  }
  _state.cs.cursor = _binary.begin() + *_binary.get_address(ip);
  _state.begin = _state.cs.address;
  return true;
}

Engine::~Engine(void) {
  if (_insn) {
    ::cs_free(_insn, 1);
  }
}

} // end namespace execution
} // end namespace banal
