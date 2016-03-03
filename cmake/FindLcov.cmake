
# Copyright Louis Dionne 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module finds the lcov executable. This module sets the
# following CMake variables:
#
# Lcov_FOUND
#   Set to 1 when lcov is found, 0 otherwise.
#
# Lcov_EXECUTABLE
#   If the lcov executable is found, this is set to its full path.
#   Otherwise this is not set.
#
#
# The following variables can be used to give hints about search locations:
#
# Lcov_EXECUTABLE
#   The path to the genhtml executable.
#
# Genhtml_FOUND
#   Set to 1 when genhtml is found, 0 otherwise.
#
# Genhtml_EXECUTABLE
#   If the genhtml executable is found, this is set to its full path.
#   Otherwise this is not set.
#
#
# The following variables can be used to give hints about search locations:
#
# Genhtml_EXECUTABLE
#   The path to the genhtml executable.

if (NOT EXISTS "${Lcov_EXECUTABLE}")
    find_program(Lcov_EXECUTABLE NAMES lcov
        DOC "Full path to the lcov executable.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Lcov
    FOUND_VAR Lcov_FOUND
    REQUIRED_VARS Lcov_EXECUTABLE)

if (NOT EXISTS "${Genhtml_EXECUTABLE}")
    find_program(Genhtml_EXECUTABLE NAMES genhtml
        DOC "Full path to the genhtml executable.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(genhtml
    FOUND_VAR genhtml_FOUND
    REQUIRED_VARS Genhtml_EXECUTABLE)