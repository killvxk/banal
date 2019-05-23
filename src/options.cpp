///
/// \file
/// \brief Analysis options implementation
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#include <fstream>
#include <iostream>

#include <llvm/Support/CommandLine.h>

#include "banal/options.hpp"

namespace banal {

/// \name Main options
/// @{

/// \brief Main category
static ::llvm::cl::OptionCategory MainCategory("Main Options");

/// \brief The filename to analyze
static ::llvm::cl::opt<::std::string > InputFilename(
    ::llvm::cl::Positional,
    ::llvm::cl::desc("<binary file>"),
    ::llvm::cl::Required,
    ::llvm::cl::value_desc("filename"),
    ::llvm::cl::cat(MainCategory));

/// \brief List of args for the program
static ::llvm::cl::list<::std::string > Argv(
    ::llvm::cl::ConsumeAfter, ::llvm::cl::desc("<program arguments>..."));

/// \brief The architecture
static ::llvm::cl::opt< Architecture > Arch(
    "a",
    ::llvm::cl::Optional,
    ::llvm::cl::desc("Force architecture, using:"),
    ::llvm::cl::values(
        clEnumValN(Architecture::X86,
                   get_architecture_short_name(Architecture::X86).data(),
                   get_architecture_long_name(Architecture::X86).data()),
        clEnumValN(Architecture::X86_64,
                   get_architecture_short_name(Architecture::X86_64).data(),
                   get_architecture_long_name(Architecture::X86_64).data()),
        clEnumValN(Architecture::ARM,
                   get_architecture_short_name(Architecture::ARM).data(),
                   get_architecture_long_name(Architecture::ARM).data()),
        clEnumValN(Architecture::AArch64,
                   get_architecture_short_name(Architecture::AArch64).data(),
                   get_architecture_long_name(Architecture::AArch64).data())),
    ::llvm::cl::cat(MainCategory));

/// \brief The format
static ::llvm::cl::opt< format::Format > Format(
    "f",
    ::llvm::cl::Required,
    ::llvm::cl::desc("Executable format:"),
    ::llvm::cl::values(clEnumValN(format::Format::ELF,
                                  format::str(format::Format::ELF).data(),
                                  format::str(format::Format::ELF).data()),
                       clEnumValN(format::Format::PE,
                                  format::str(format::Format::PE).data(),
                                  format::str(format::Format::PE).data())),
    ::llvm::cl::cat(MainCategory));

/// @}
/// \name Analysis options
/// @{

/// \brief Options category
static ::llvm::cl::OptionCategory AnalysisCategory("Analysis Options");

/// @}
/// \name Debug options
/// @{

/// \brief Debug category
static ::llvm::cl::OptionCategory DebugCategory("Debug Options");

#ifndef NDEBUG

#else

#endif

/// @}

Options::Options(int argc, char** argv)
    : _filepath(),
      _arch(::std::nullopt),
      _format(::std::nullopt),
      _argv(),
      _status(false) {
  ::llvm::cl::ParseCommandLineOptions(argc,
                                      argv,
                                      "banal -- buffer overflow analysis");

  _filepath = InputFilename.getValue();
  if (Arch.getPosition()) {
    _arch = Arch;
  } else {
    _arch = ::std::nullopt;
  }
  _format = Format;
  _status = true;
  _argv = Argv;
}

} // end namespace banal
