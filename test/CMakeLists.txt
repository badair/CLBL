
# Copyright Louis Dionne 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup custom functions and master targets for the unit tests
##############################################################################
add_custom_target(tests COMMENT "Build all the unit tests.")
add_dependencies(check tests)

add_custom_target(tests.quick COMMENT "Build a subset of all the unit tests to finish faster.")

#   clbl_add_unit_test(<name> ...)
#
# Equivalent to `clbl_add_test`, except the test is also added as a
# dependency of the `tests` target.
function(clbl_add_unit_test name)
    clbl_add_test(${ARGV})
    add_dependencies(tests ${name})
    if ((NOT "${name}" MATCHES "\\.ext\\.") AND (NOT "${name}" MATCHES "_mcd"))
        add_dependencies(tests.quick ${name})
    endif()
endfunction()


##############################################################################
# Include directories
##############################################################################
include_directories(${CLBL_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})


##############################################################################
# Generate tests that include each public header.
# The headers that were excluded above due to caveats are ignored here.
##############################################################################
#file(GLOB_RECURSE PUBLIC_HEADERS
#    RELATIVE "${CLBL_SOURCE_DIR}/include"
#    "${CLBL_SOURCE_DIR}/include/*.hpp"
#)
#
#file(GLOB_RECURSE EXCLUDED_PUBLIC_HEADERS
#    RELATIVE "${CLBL_SOURCE_DIR}/include"
#    ${EXCLUDED_PUBLIC_HEADERS}
#    "${CLBL_SOURCE_DIR}/include/boost/hana/detail/*.hpp"
#    "${CLBL_SOURCE_DIR}/include/boost/hana/ext/boost/fusion/detail/*.hpp"
#)
#
#list(REMOVE_ITEM PUBLIC_HEADERS ${EXCLUDED_PUBLIC_HEADERS})
#include(TestHeaders)
#add_dependencies(tests.quick test.headers)
#add_dependencies(tests test.headers)

##############################################################################
# Add all the remaining unit tests
##############################################################################
file(GLOB_RECURSE UNIT_TESTS "*.cpp")
#file(GLOB_RECURSE EXCLUDED_UNIT_TESTS ${EXCLUDED_UNIT_TESTS})
#list(REMOVE_ITEM UNIT_TESTS ${EXCLUDED_UNIT_TESTS})

foreach(_file IN LISTS UNIT_TESTS)
    clbl_target_name_for(_target "${_file}")
    file(STRINGS "${_file}" _subparts REGEX "CLBL_TEST_PART == ([0-9]+)")

    # If the file contains the special CLBL_TEST_PART marker, we split
    # it into several subparts. This is done to avoid long compile-times and
    # other inconveniences.
    if (_subparts)
        string(REGEX MATCHALL "CLBL_TEST_PART == ([0-9]+)" _subparts "${_subparts}")
        string(REGEX REPLACE "CLBL_TEST_PART == ([0-9]+)" "\\1" _subparts "${_subparts}")
        list(REMOVE_DUPLICATES _subparts)
        add_custom_target(${_target})

        foreach(n IN LISTS _subparts)
            add_executable("${_target}.part${n}" EXCLUDE_FROM_ALL "${_file}")
            set_target_properties("${_target}.part${n}" PROPERTIES
                COMPILE_DEFINITIONS "CLBL_TEST_PART=${n}")
            add_dependencies(${_target} "${_target}.part${n}")

            clbl_add_unit_test("${_target}.part${n}"
                ${CMAKE_CURRENT_BINARY_DIR}/${_target}.part${n})
        endforeach()
    else()
        add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
        clbl_add_unit_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
    endif()
endforeach()
