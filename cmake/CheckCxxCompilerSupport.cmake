
# Copyright Louis Dionne 2015
# Copyright Markus J. Weber 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module checks whether the current compiler is supported, and
# provides friendly hints to the user.

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "3.7")
        message(WARNING "
    ### You appear to be using Clang ${CMAKE_CXX_COMPILER_VERSION}, which is known
    ### to be unable to compile CLBL. Consider switching to
    ### Clang >= 3.7. If it is already installed on your
    ### system, you can tell CMake about it with
    ###
    ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
        ")
    endif()
    if (MSVC)
        if(${MSVC_VERSION} LESS 1900)
            message(WARNING "
    ### Your version of Visual Studio is not supported.
    ### Please upgrade to Visual Studio 2015 or above.
            ")
        endif()
        if(${CMAKE_GENERATOR_TOOLSET} MATCHES "LLVM-*")
            message(STATUS "Visual Studio platform toolset is ${CMAKE_GENERATOR_TOOLSET}")
        else()
            message(WARNING "
    ### You haven't specified the platform toolset option to cmake.
    ### Please run cmake for a Windows 32bit solution with
    ###     cmake -TLLVM-vs2014 ..
    ### and for a Windows 64bit solution with
    ###     cmake -TLLVM-vs2014 -G\"Visual Studio 14 2015 Win64\" ..
    ### which should choose the correct platform toolset automatically.
            ")
        endif()
        if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio 14 2015*")
            message(WARNING "
    ### You're not using a Visual Studio 2015 generator. Please run cmake
    ### for a Windows 32bit solution with
    ###     cmake -TLLVM-vs2014 ..
    ### and for a Windows 64bit solution with
    ###     cmake -TLLVM-vs2014 -G\"Visual Studio 14 2015 Win64\" ..
    ### which should choose the correct platform toolset automatically.
            ")
        endif()
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "7")
        message(WARNING "
    ### You appear to be using Apple's Clang ${CMAKE_CXX_COMPILER_VERSION}, which is
    ### shipped with Xcode < 7. You should consider using a 
	### non-Apple Clang >= 3.7, which can be installed via 
	### Homebrew with
    ###
    ###     brew install llvm --with-clang
    ###
    ### You can then tell CMake to use that non-system Clang with
    ###
    ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
        ")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "5.2.1")
    message(WARNING "
    ### You appear to be using GCC ${CMAKE_CXX_COMPILER_VERSION}, which may
    ### not be able to compile CLBL. CLBL officially supports GCC versions >= 5.2.1.
    ")
    endif()
elseif (MSVC)
    message(WARNING "
    ### Native Visual Studio is not supported. Please install pre-built windows
    ### LLVM/Clang binaries with Visual Studio 2015 integration and run cmake
    ### for a Windows 32bit solution with
    ###     cmake -TLLVM-vs2014 ..
    ### and for a Windows 64bit solution with
    ###     cmake -TLLVM-vs2014 -G\"Visual Studio 14 2015 Win64\" ..
    ### which should choose the correct platform toolset automatically.
    ")
else()
    message(WARNING "
    ### You appear to be using a compiler that is not yet tested with CLBL.
    ### Please tell us whether it successfully compiles or if and how it
    ### fails by opening an issue at https://github.com/badair/CLBL/issues.
    ### Thanks!
    ")
endif()
