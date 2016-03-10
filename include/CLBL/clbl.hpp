/*!
@file
Includes all necessary headers.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CLBL_H
#define CLBL_CLBL_H

#if defined(__GNUC__ ) && __GNUC__ < 6
    //GCC 5 can't resolve ref-qualified overloads with auto deduced return type,
    //so we omit this part of the test for those versions. TODO mention this bug in documentation.
    //https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60943
#define CLBL_GCC_5_REF_OVERLOAD_WORKAROUND
#endif

//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html
#ifdef __cpp_constexpr
#define CLBL_CXX14_CONSTEXPR constexpr
#else
#define CLBL_CXX14_CONSTEXPR
#endif

#define CLBL_FORCE_SFINAE(type_alias, type_actual) \
    bool force_sfinae = true, typename type_alias = typename std::enable_if<force_sfinae, type_actual>::type

#ifdef QUALI_MACROS_HPP
#define CLBL_LEAVE_QUALI_MACROS_DEFINED
#endif

#include <quali/quali_macros.hpp>

#include <CLBL/tags.hpp>
#include <quali/quali.hpp>
#include <CLBL/can_dereference.hpp>
#include <CLBL/dereferenceable_object.hpp>
#include <CLBL/generalized_object.hpp>
#include <CLBL/generalize.hpp>
#include <CLBL/args.hpp>
#include <CLBL/return.hpp>
#include <CLBL/emulates.hpp>
#include <CLBL/is_ambiguous.hpp>
#include <CLBL/fwrap.hpp>
#include <CLBL/convert_to.hpp>
#include <CLBL/harden.hpp>
#include <CLBL/forward.hpp>

#ifndef CLBL_LEAVE_QUALI_MACROS_DEFINED
#include <quali/quali_macros_undef.hpp>
#endif
    
#endif