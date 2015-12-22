
# Copyright Louis Dionne 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
cmake_minimum_required(VERSION 3.0)
project(CLBL CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
enable_testing()

# Perform checks to make sure we support the current compiler
include(CheckCxxCompilerSupport)


##############################################################################
# Setup CMake options
##############################################################################
option(CLBL_ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(CLBL_ENABLE_MEMCHECK "Run the unit tests and examples under Valgrind if it is found." OFF)
#option(CLBL_ENABLE_CONCEPT_CHECKS "Enable concept checking in the interface methods." ON)

##############################################################################
# Setup compiler flags (more can be set on a per-target basis or in subdirectories)
##############################################################################
include(CheckCXXCompilerFlag)
macro(clbl_append_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

# Compiler flags controlled by CMake options above
if (CLBL_ENABLE_WERROR)
    clbl_append_flag(CLBL_HAS_WERROR -Werror)
    clbl_append_flag(CLBL_HAS_WX -WX)
endif()

#if (NOT CLBL_ENABLE_CONCEPT_CHECKS)
#    add_definitions(-DCLBL_CONFIG_DISABLE_CONCEPT_CHECKS)
#endif()

# Other compiler flags
#clbl_append_flag(CLBL_HAS_FTEMPLATE_BACKTRACE_LIMIT  -ftemplate-backtrace-limit=0)
#clbl_append_flag(CLBL_HAS_PEDANTIC                   -pedantic)
clbl_append_flag(CLBL_HAS_STDCXX1Y                   -std=c++1y)
clbl_append_flag(CLBL_HAS_QUNUSED_ARGUMENTS          -Qunused-arguments)
clbl_append_flag(CLBL_HAS_W                          -W)
clbl_append_flag(CLBL_HAS_WALL                       -Wall)
clbl_append_flag(CLBL_HAS_WEXTRA                     -Wextra)
clbl_append_flag(CLBL_HAS_WNO_UNUSED_LOCAL_TYPEDEFS  -Wno-unused-local-typedefs)
clbl_append_flag(CLBL_HAS_WWRITE_STRINGS             -Wwrite-strings)


##############################################################################
# Search for packages.
#
# Behavior when the package is found or not is customized at the
# point where the package is required.
##############################################################################
#find_package(Boost)
#find_package(Doxygen)
#find_package(Meta)
#find_package(MPL11)
#find_package(PythonInterp 2.7)
#find_package(Ruby 2.1)

if (CLBL_ENABLE_MEMCHECK)
    find_package(Valgrind REQUIRED)
endif()

#if (NOT ${Boost_FOUND})
#    message(WARNING
#        "The Boost library headers were not found; targets depending "
#        "on Boost won't be available.")
#endif()


##############################################################################
# Setup custom functions to ease the creation of targets
##############################################################################
#   clbl_target_name_for(<output variable> <source file> [ext])
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of CLBL is `path/to/source/file.ext`,
# the target name associated to it will be `path.to.source.file`.
#
# The extension of the file should be specified as a last argument. If no
# extension is specified, the `.cpp` extension is assumed.
function(clbl_target_name_for out file)
    if (NOT ARGV2)
        set(_extension ".cpp")
    else()
        set(_extension "${ARGV2}")
    endif()

    file(RELATIVE_PATH _relative ${CLBL_SOURCE_DIR} ${file})
    string(REPLACE "${_extension}" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

#   clbl_add_test(<name> <command> [<arg>...])
#
# Creates a test called `name`, which runs the given `command` with the given
# `arg`uments. However, if `CLBL_ENABLE_MEMCHECK` is set to `ON`, the
# test will run the provided command under the memory checker.
function(clbl_add_test name)
    if (CLBL_ENABLE_MEMCHECK)
        add_test(${name} ${Valgrind_EXECUTABLE} --leak-check=full --error-exitcode=1 ${ARGN})
    else()
        add_test(${name} ${ARGN})
    endif()
endfunction()


##############################################################################
# Setup the `check` target to build and then run all the tests and examples.
##############################################################################
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Build and then run all the tests and examples.")


##############################################################################
# Setup subdirectories
##############################################################################
#add_subdirectory(benchmark)
#add_subdirectory(doc)
#add_subdirectory(example)
#add_subdirectory(experimental)
add_subdirectory(tests)


##############################################################################
# Setup the 'install' target.
# This copies the whole content of include/ to ${CMAKE_INSTALL_PREFIX}.
##############################################################################
install(DIRECTORY include/boost DESTINATION include
        PATTERN ".DS_Store" EXCLUDE)
