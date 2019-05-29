#include <cassert>
#include <iostream>

#include "banal/analysis.hpp"
#include "banal/binary/binary.hpp"
#include "banal/options.hpp"

int main(int argc, char** argv) {
  ::banal::Options opt(argc, argv);
  if (!opt.good()) {
    return 1;
  }

  auto bin = ::banal::binary::open(opt);
  if (!bin) {
    return 1;
  }

  ::banal::log::cinfo() << "Using architecture " << bin->architecture()
                        << ::std::endl;
  ::banal::log::cinfo() << "Executable format: " << bin->format()
                        << ::std::endl;
  ::banal::log::cinfo() << "Setting argc to " << opt.argv().size()
                        << ::std::endl;
  ::banal::log::cinfo() << "Entry point: 0x" << ::std::hex << bin->entry()
                        << ::std::endl;
  ::banal::Analysis a(opt, *bin);
  if (!a.set_stack(0xbffff000, 4096))
    return 1;
  a.start();
}
