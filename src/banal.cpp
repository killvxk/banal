#include <cassert>
#include <iostream>

#include "banal/analysis.hpp"
#include "banal/binary.hpp"
#include "banal/format/format.hpp"
#include "banal/options.hpp"

int main(int argc, char** argv) {
  ::banal::Options opt(argc, argv);
  if (!opt.good()) {
    return 1;
  }

  auto bin = ::banal::Binary::open(opt.filepath());
  if (!bin) {
    return 1;
  }

  auto parser = ::banal::format::get_parser(opt.format(), bin->stream());
  if (!parser) {
    return 1;
  }

  ::banal::Architecture arch;
  arch = parser->architecture();
  if (opt.arch() && parser->architecture() != opt.arch()) {
    arch = *opt.arch();
    ::banal::log::cwarn()
        << "Architecture supplied by command line argument is not the same the "
           "architrecture supplied by the executable. Assuming user is right, "
           "using architecture "
        << arch << ::std::endl;
  }

  ::banal::log::cinfo() << "Using architecture " << arch << ::std::endl;
  ::banal::log::cinfo() << "Executable format: " << opt.format() << ::std::endl;
  ::banal::log::cinfo() << "Setting argc to " << opt.argv().size()
                        << ::std::endl;

  ::banal::Analysis a(opt, arch);
}
