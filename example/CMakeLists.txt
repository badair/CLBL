# Copyright Louis Dionne 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup custom functions and master targets for the examples
##############################################################################
add_custom_target(examples COMMENT "Build all the examples.")
add_dependencies(check examples)

# Several examples have unused parameters because the name of the parameters
# are useful for illustration, even if the implementation is not actually
# presented. We don't want to generate warnings for that or need to comment
# out all unused parameter names.
clbl_append_flag(CLBL_HAS_WNO_UNUSED_PARAMETER -Wno-unused-parameter)


##############################################################################
# Include directories
##############################################################################
include_directories(${CLBL_SOURCE_DIR}/include)
#if (Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()

##############################################################################
# Add all the examples
##############################################################################
file(GLOB_RECURSE EXAMPLES "*.cpp")
file(GLOB_RECURSE EXCLUDED_EXAMPLES ${EXCLUDED_EXAMPLES})
list(REMOVE_ITEM EXAMPLES "" ${EXCLUDED_EXAMPLES})

foreach(_file IN LISTS EXAMPLES)
    clbl_target_name_for(_target "${_file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
    clbl_add_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
    add_dependencies(examples ${_target})
endforeach()


##############################################################################
# Setup the `wandbox` target, which uploads the whole library along with the
# example/wandbox.cpp example to http://melpon.org/wandbox, and prints a
# permlink to it.
##############################################################################
if (PYTHONINTERP_FOUND)
    add_custom_target(wandbox COMMAND
        ${PYTHON_EXECUTABLE} ${CLBL_SOURCE_DIR}/cmake/wandbox.py
                             ${CLBL_SOURCE_DIR}/include
                             ${CLBL_SOURCE_DIR}/example/intro.cpp
        SOURCES ${CLBL_SOURCE_DIR}/cmake/wandbox.py
                ${CLBL_SOURCE_DIR}/example/intro.cpp)
endif()
