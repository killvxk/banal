///
/// \file
/// \brief Logging specification
///
/// Author: Thomas Bailleux
///
/// Contact: thomas at bailleux.me

#pragma once

#include <iostream>

namespace banal {

/// \brief Logging namespace
namespace log {

/// \brief Reset code
#define CODE_RESET "\033[0m"
/// \brief Black code
#define CODE_BLACK "\033[30m"
/// \brief Red code
#define CODE_RED "\033[31m"
/// \brief Green code
#define CODE_GREEN "\033[32m"
/// \brief Yellow code
#define CODE_YELLOW "\033[33m"
/// \brief Blue code
#define CODE_BLUE "\033[34m"
/// \brief Magenta code
#define CODE_MAGENTA "\033[35m"
/// \brief Cyan code
#define CODE_CYAN "\033[36m"
/// \brief White code
#define CODE_WHITE "\033[37m"
/// \brief Bold black code
#define CODE_BBLACK "\033[1m\033[30m"
/// \brief Bold red code
#define CODE_BRED "\033[1m\033[31m"
/// \brief Bold green code
#define CODE_BGREEN "\033[1m\033[32m"
/// \brief Bold yellow code
#define CODE_BYELLOW "\033[1m\033[33m"
/// \brief Bold blue code
#define CODE_BBLUE "\033[1m\033[34m"
/// \brief Bold magenta code
#define CODE_BMAGENTA "\033[1m\033[35m"
/// \brief Bold cyan code
#define CODE_BCYAN "\033[1m\033[36m"
/// \brief Bold white code
#define CODE_BWHITE "\033[1m\033[37m"

/// \brief Print something in black
#define BLACK(_x_) CODE_BLACK, (_x_), CODE_RESET
/// \brief Print something in bold black
#define BBLACK(_x_) CODE_BBLACK, (_x_), CODE_RESET
/// \brief Print something in red
#define RED(_x_) CODE_RED, (_x_), CODE_RESET
/// \brief Print something in bold red
#define BRED(_x_) CODE_BRED, (_x_), CODE_RESET
/// \brief Print something in green
#define GREEN(_x_) CODE_GREEN, (_x_), CODE_RESET
/// \brief Print something in bold green
#define BGREEN(_x_) CODE_BGREEN, (_x_), CODE_RESET
/// \brief Print something in yellow
#define YELLOW(_x_) CODE_YELLOW, (_x_), CODE_RESET
/// \brief Print something in bold yellow
#define BYELLOW(_x_) CODE_BYELLOW, (_x_), CODE_RESET
/// \brief Print something in blue
#define BLUE(_x_) CODE_BLUE, (_x_), CODE_RESET
/// \brief Print something in bold blue
#define BBLUE(_x_) CODE_BBLUE, (_x_), CODE_RESET
/// \brief Print something in magenta
#define MAGENTA(_x_) CODE_MAGENTA, (_x_), CODE_RESET
/// \brief Print something in bold magenta
#define BMAGENTA(_x_) CODE_BMAGENTA, (_x_), CODE_RESET
/// \brief Print something in cyan
#define CYAN(_x_) CODE_CYAN, (_x_), CODE_RESET
/// \brief Print something in bold cyan
#define BCYAN(_x_) CODE_BCYAN, (_x_), CODE_RESET
/// \brief Print something in white
#define WHITE(_x_) CODE_WHITE, (_x_), CODE_RESET
/// \brief Print something in bold white
#define BWHITE(_x_) CODE_BWHITE, (_x_), CODE_RESET
/// \brief Print something in hexadecimal
#define HEX(_v_, _w_) "0x", ::std::hex, _v_, ::std::dec

#ifndef NDEBUG

/// \brief Log stuffs
///
/// \param args Stuffs
template < typename... Options >
void log(const Options&... args) {
  ::std::cerr << "[DEBUG] ";
  ((::std::cerr << args), ...);
  ::std::cerr << ::std::endl;
}

#else
/// \brief Log stuffs (empty, NDEBUG)
template < typename... Options >
void log(Options&...) {}

#endif // ndef NDEBUG

/// \brief Log an error
///
/// \param out The output stream
///
/// \return The output stream
::std::ostream& cerr(::std::ostream& out = ::std::cerr);

/// \brief Log a warning
///
/// \param out The output stream
///
/// \return The output stream
::std::ostream& cwarn(::std::ostream& out = ::std::cerr);

/// \brief Log a good thing
///
/// \param out The output stream
///
/// \return The output stream
::std::ostream& cgood(::std::ostream& out = ::std::cerr);

/// \brief Log an info thing
///
/// \param out The output stream
///
/// \return The output stream
::std::ostream& cinfo(::std::ostream& out = ::std::cerr);

/// \brief Called when an unreachable zone is called
///
/// \param msg The message to display
__attribute((noreturn)) inline void unreachable(std::string_view msg) {
  cerr() << "UNREACHABLE: " << msg << ::std::endl;
  exit(0);
}

} // end namespace log
} // end namespace banal
