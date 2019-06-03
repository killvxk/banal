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
      _argv(),
      _status(false) {
  ::llvm::cl::ParseCommandLineOptions(argc,
                                      argv,
                                      "banal -- buffer overflow analysis");

  _filepath = InputFilename.getValue();
  _status = true;
  _argv = Argv;
}

} // end namespace banal
