cmake_minimum_required(VERSION 3.0)
project(banal)
set(PACKAGE_VERSION 1.0.0)

if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if (POLICY CMP0057)
  cmake_policy(SET CMP0057 NEW)
endif()

# Add path for custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# check for git submodules
include(FindGit)
if (NOT GIT_FOUND)
  message(FATAL_ERROR "Git was not found on this system.")
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/elfio/elfio/elfio.hpp")
	message(FATAL_ERROR "Submodules were not downloaded. Please update submodules using git, and try again")
endif()

set(ARCH_SIZES "32" "64")

if(NOT ARCH_SIZE)
  set(ARCH_SIZE "64")
else()
  if(NOT ARCH_SIZE IN_LIST ARCH_SIZES)
      message(FATAL_ERROR "Architecture size are 32 or 64")
  endif()
endif()

set(BANAL_NAME "${CMAKE_PROJECT_NAME}.${ARCH_SIZE}")
set(BANAL_SRC_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_executable(${BANAL_NAME}
  ${BANAL_SRC_DIRS}/analysis.cpp
  ${BANAL_SRC_DIRS}/banal.cpp
  ${BANAL_SRC_DIRS}/binary/binary.cpp
  ${BANAL_SRC_DIRS}/binary/component/symbol.cpp
  ${BANAL_SRC_DIRS}/binary/elf/component/segment.cpp
  ${BANAL_SRC_DIRS}/binary/elf/component/section.cpp
  ${BANAL_SRC_DIRS}/binary/elf/elf.cpp
  ${BANAL_SRC_DIRS}/execution/engine.cpp
  ${BANAL_SRC_DIRS}/execution/map.cpp
  ${BANAL_SRC_DIRS}/execution/stack.cpp
  ${BANAL_SRC_DIRS}/format.cpp
  ${BANAL_SRC_DIRS}/options.cpp
  ${BANAL_SRC_DIRS}/util/log.cpp
  ${BANAL_SRC_DIRS}/util/mem_based_stream.cpp
)
target_compile_definitions(${BANAL_NAME} PUBLIC "ARCH_SIZE=${ARCH_SIZE}")

target_include_directories(${BANAL_NAME} SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/elfio/")

# Threads
find_package(Threads REQUIRED)
target_link_libraries(${BANAL_NAME} PUBLIC "${CMAKE_THREAD_LIBS_INIT}")

# Capstone
include(FindCAPSTONE)
if (NOT CAPSTONE_FOUND)
  message(FATAL_ERROR "Capstone is required.")
else()
  target_include_directories(${BANAL_NAME} SYSTEM PRIVATE ${CAPSTONE_INCLUDE_DIRS})
  target_link_libraries(${BANAL_NAME} PRIVATE "${CAPSTONE_LIBRARIES}")
endif()

# Unicorn
include(FindUnicorn)
if (NOT UNICORN_FOUND)
  message(FATAL_ERROR "Unicorn engine is required for emulation.")
else()
  target_include_directories(${BANAL_NAME} SYSTEM PRIVATE ${UNICORN_INCLUDE_DIRS})
  target_link_libraries(${BANAL_NAME} PRIVATE "${UNICORN_LIBRARIES}")
endif()

# LLVM
find_package(LLVM REQUIRED)
if (NOT LLVM_FOUND)
  message(FATAL_ERROR "LLVM is required.")
else()
  list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
  include(LLVMConfig)
  target_include_directories(${BANAL_NAME} SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
  llvm_map_components_to_libnames(llvm_libs support)
  target_link_libraries(${BANAL_NAME} PRIVATE ${llvm_libs})
endif()

# C flags
include(CheckCXXCompilerFlag)
macro(add_cxx_flag target policy name flag)
  check_cxx_compiler_flag(${flag} "CXX_SUPPORTS_${name}")
  if (CXX_SUPPORTS_${name})
    target_compile_options(${target} PUBLIC "${flag}")
  elseif (NOT (${policy} STREQUAL "OPTIONAL"))
    message(FATAL_ERROR "${flag} flag is not supported by ${CMAKE_CXX_COMPILER}")
  endif()
endmacro()

add_cxx_flag(${BANAL_NAME} REQUIRED "STDC++17" "-std=c++17")
add_cxx_flag(${BANAL_NAME} REQUIRED "WALL" "-Wall")
add_cxx_flag(${BANAL_NAME} REQUIRED "WEXTRA" "-Wextra")
add_cxx_flag(${BANAL_NAME} REQUIRED "WERROR" "-Werror")
add_cxx_flag(${BANAL_NAME} REQUIRED "FUNCTION_SECTIONS" "-ffunction-sections")
add_cxx_flag(${BANAL_NAME} REQUIRED "DATA_SECTIONS" "-fdata-sections")
add_cxx_flag(${BANAL_NAME} OPTIONAL "WLIFETIME" "-Wlifetime")
add_cxx_flag(${BANAL_NAME} OPTIONAL "WEVERYTHING" "-Weverything")
add_cxx_flag(${BANAL_NAME} OPTIONAL "WEFFC++" "-Weffc++")

# Disable some warnings
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_CXX_98_COMPAT" "-Wno-c++98-compat")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_PADDED" "-Wno-padded")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_SWITCH" "-Wno-switch")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_SWITCH_ENUM" "-Wno-switch-enum")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_EXIT_TIME_DESTRUCTORS" "-Wno-exit-time-destructors")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_GLOBAL_CONSTRUCTORS" "-Wno-global-constructors")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_COVERED_SWITCH_DEFAULT" "-Wno-covered-switch-default")
add_cxx_flag(${BANAL_NAME} REQUIRED "WNO_WEAK_VTABLES" "-Wno-weak-vtables")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_compile_definitions(${BANAL_NAME} PUBLIC "NDEBUG=1")
else()
  add_cxx_flag(${BANAL_NAME} REQUIRED "G3" "-g3")
endif()

if (${CMAKE_ASAN})
  target_compile_options(${BANAL_NAME} PUBLIC "-fsanitize=address")
  target_compile_options(${BANAL_NAME} PUBLIC "-fno-omit-frame-pointer")
  target_link_options(${BANAL_NAME} PUBLIC "-fsanitize=address")
  target_link_options(${BANAL_NAME} PUBLIC "-fno-omit-frame-pointer")
endif()

# include headers
set(BANAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${BANAL_NAME} PUBLIC ${BANAL_INCLUDE_DIRS})

