///
/// \file
/// \brief Analysis implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <iomanip>
#include <variant>

#include "banal/analysis.hpp"
#include "banal/execution/engine.hpp"

namespace banal {

namespace {

[[maybe_unused]] static void dump_registers(::uc_engine* uc, Architecture a) {
  ::std::int32_t* regs = nullptr;
  ::std::uint64_t values[17];
  void* rvalues[17] = {};
  for (int i = 0; i < 17; i++) {
    rvalues[i] = reinterpret_cast< void* >(&(values[i]));
  }
  int size;
  switch (a) {
    case Architecture::X86_64: {
      regs = new ::std::int32_t[17]{::UC_X86_REG_RAX,
                                    ::UC_X86_REG_RBX,
                                    ::UC_X86_REG_RCX,
                                    ::UC_X86_REG_RDX,
                                    ::UC_X86_REG_RSI,
                                    ::UC_X86_REG_RDI,
                                    ::UC_X86_REG_RBP,
                                    ::UC_X86_REG_RSP,
                                    ::UC_X86_REG_R8,
                                    ::UC_X86_REG_R9,
                                    ::UC_X86_REG_R10,
                                    ::UC_X86_REG_R11,
                                    ::UC_X86_REG_R12,
                                    ::UC_X86_REG_R13,
                                    ::UC_X86_REG_R14,
                                    ::UC_X86_REG_R15,
                                    ::UC_X86_REG_RIP};
      size = 17;
    } break;
    case Architecture::X86: {
      regs = new ::std::int32_t[9]{::UC_X86_REG_EAX,
                                   ::UC_X86_REG_EBX,
                                   ::UC_X86_REG_ECX,
                                   ::UC_X86_REG_EDX,
                                   ::UC_X86_REG_ESI,
                                   ::UC_X86_REG_EDI,
                                   ::UC_X86_REG_EBP,
                                   ::UC_X86_REG_ESP,
                                   ::UC_X86_REG_EIP};
      size = 9;
    } break;
    default: {
      log::unreachable("Unreachable");
    }
  }
  auto e =
      ::uc_reg_read_batch(uc, regs, reinterpret_cast< void** >(rvalues), size);
  if (e != ::UC_ERR_OK) {
    log::cerr() << "Unable to dump CPU registers for " << a << ": "
                << ::uc_strerror(e) << ::std::endl;
    return;
  }
  switch (a) {
    case Architecture::X86_64: {
      ::std::uint64_t* v = reinterpret_cast<::std::uint64_t* >(values);
      ::std::cout << "RAX=0x" << ::std::hex << v[0] << ::std::endl;
      ::std::cout << "RBX=0x" << ::std::hex << v[1] << ::std::endl;
      ::std::cout << "RCX=0x" << ::std::hex << v[2] << ::std::endl;
      ::std::cout << "RDX=0x" << ::std::hex << v[3] << ::std::endl;
      ::std::cout << "RSI=0x" << ::std::hex << v[4] << ::std::endl;
      ::std::cout << "RDI=0x" << ::std::hex << v[5] << ::std::endl;
      ::std::cout << "RBP=0x" << ::std::hex << v[6] << ::std::endl;
      ::std::cout << "RSP=0x" << ::std::hex << v[7] << ::std::endl;
      ::std::cout << "R8=0x" << ::std::hex << v[8] << ::std::endl;
      ::std::cout << "R9=0x" << ::std::hex << v[9] << ::std::endl;
      ::std::cout << "R10=0x" << ::std::hex << v[10] << ::std::endl;
      ::std::cout << "R11=0x" << ::std::hex << v[11] << ::std::endl;
      ::std::cout << "R12=0x" << ::std::hex << v[12] << ::std::endl;
      ::std::cout << "R13=0x" << ::std::hex << v[13] << ::std::endl;
      ::std::cout << "R14=0x" << ::std::hex << v[14] << ::std::endl;
      ::std::cout << "R15=0x" << ::std::hex << v[15] << ::std::endl;
      ::std::cout << "RIP=0x" << ::std::hex << v[16] << ::std::endl;
    } break;

    case Architecture::X86: {
      ::std::uint32_t* v = reinterpret_cast<::std::uint32_t* >(values);
      ::std::cout << "EAX=0x" << ::std::hex << v[0] << ::std::endl;
      ::std::cout << "EBX=0x" << ::std::hex << v[1] << ::std::endl;
      ::std::cout << "ECX=0x" << ::std::hex << v[2] << ::std::endl;
      ::std::cout << "EDX=0x" << ::std::hex << v[3] << ::std::endl;
      ::std::cout << "EDI=0x" << ::std::hex << v[4] << ::std::endl;
      ::std::cout << "ESI=0x" << ::std::hex << v[5] << ::std::endl;
      ::std::cout << "EBP=0x" << ::std::hex << v[6] << ::std::endl;
      ::std::cout << "ESP=0x" << ::std::hex << v[7] << ::std::endl;
      ::std::cout << "EIP=0x" << ::std::hex << v[8] << ::std::endl;
    } break;

    default: {
      log::unreachable("Unreachable");
    }
  }
  delete[] regs;
}

} // namespace

Analysis::Analysis(const Options& opt, binary::Binary& binary)
    : _options(opt),
      _csh(0),
      _uc(nullptr),
      _binary(binary),
      _virtual_binary_address(::std::nullopt),
      _good(false) {
  {
    auto capstone_value = get_cs_architecture(_binary.architecture());
    if (auto e = ::cs_open(capstone_value.first, capstone_value.second, &_csh);
        e != ::CS_ERR_OK) {
      log::cerr() << "Unable to initialize Capstone engine: "
                  << ::cs_strerror(e) << ::std::endl;
      _good = false;
      return;
    }
    log::cgood() << "Capstone loaded" << ::std::endl;
    log::log("Capstone handler = ", _csh);
  }

  {
    auto uc_value = get_uc_architecture(_binary.architecture());
    if (auto e = ::uc_open(uc_value.first, uc_value.second, &_uc);
        e != ::UC_ERR_OK) {
      log::cerr() << "Unable to initialize Unicorn engine: " << ::uc_strerror(e)
                  << ::std::endl;
      _good = false;
      return;
    }
    log::cgood() << "Unicorn engine loaded" << ::std::endl;
    log::log("Unicorn engine handler = ", _uc);
  }
}

Analysis::~Analysis(void) {
  if (auto e = ::cs_close(&_csh); e != ::CS_ERR_OK) {
    log::cerr() << "Unable to close capstone engine: " << ::cs_strerror(e)
                << ::std::endl;
  }
  if (auto e = ::uc_close(_uc); e != ::UC_ERR_OK) {
    log::cerr() << "Unable to close unicorn engine: " << ::uc_strerror(e)
                << ::std::endl;
  }
  _uc = nullptr;
}

void Analysis::start(::std::uint64_t entry) {
  (void)entry;
  // debug
  _binary.dump();

  execution::Engine engine(_uc, _csh, _binary);

  // set hooks
  ::uc_cb_hookcode_t basic_block_hook = Analysis::hook_basic_block;
  ::uc_hook hh;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
  if (auto e = ::uc_hook_add(_uc,
                             &hh,
                             ::UC_HOOK_BLOCK,
                             reinterpret_cast< void* >(basic_block_hook),
                             static_cast< void* >(this),
                             1,
                             0);
      e != ::UC_ERR_OK) {
#pragma clang diagnostic pop
    log::cerr() << "Unable to register hook: " << ::uc_strerror(e)
                << ::std::endl;
    return;
  }

  // prepare capstone
  ::cs_insn* insn = ::cs_malloc(_csh);
  ::cs_free(insn, 1);
  while (engine.step()) {
    // dump_registers(_uc, _binary.architecture());
  }
}

void Analysis::hook_basic_block(::uc_engine* uc,
                                ::std::uint64_t address,
                                ::std::uint32_t size,
                                void* user_data) {
  (void)uc;
  (void)size;
  (void)address;
  Analysis* self = static_cast< Analysis* >(user_data);
  return self->hook_basic_block(static_cast< uintarch_t >(address), size);
}

void Analysis::hook_basic_block(uintarch_t address, ::std::uint32_t size) {
  (void)size;
  log::cinfo() << "Basic block detected on 0x" << ::std::hex << address
               << ::std::endl;
}

} // end namespace banal
