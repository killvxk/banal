///
/// \file
/// \brief Analysis implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include "banal/analysis.hpp"

namespace banal {

Analysis::Analysis(const Options& opt, Architecture arch)
    : _options(opt), _arch(arch), _csh(0), _uc(nullptr), _good(false) {
  {
    auto capstone_value = get_cs_architecture(arch);
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
    auto uc_value = get_uc_architecture(arch);
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

} // end namespace banal
