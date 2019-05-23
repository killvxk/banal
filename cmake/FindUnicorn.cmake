# Exports:
#  UNICORN_FOUND
#  UNICORN_INCLUDE_DIRS
#  UNICORN_LIBRARIES
# Hints:
#  UNICORN_LIBRARY_DIRS

find_path(UNICORN_INCLUDE_DIRS
          unicorn/unicorn.h)

find_library(UNICORN_LIBRARIES
             NAMES unicorn
             HINTS "${UNICORN_LIBRARY_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(unicorn DEFAULT_MSG
                                  UNICORN_LIBRARIES UNICORN_INCLUDE_DIRS)
mark_as_advanced(UNICORN_INCLUDE_DIRS UNICORN_LIBRARIES)
