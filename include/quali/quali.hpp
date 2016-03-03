/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_HPP
#define QUALI_HPP

//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html
#ifdef __cpp_constexpr
#define QUALI_CXX14_CONSTEXPR constexpr
#else
#define QUALI_CXX14_CONSTEXPR
#endif

#include <utility>
#include <cstdint>
#include <type_traits>
#include <functional>

#include <quali/flags.hpp>
#include <quali/can_dereference.hpp>
#include <quali/dereferenceable_object.hpp>
#include <quali/qualified_type.hpp>
#include <quali/cv_of.hpp>
#include <quali/ref_of.hpp>
#include <quali/cast.hpp>
#include <quali/generalized_object.hpp>
#include <quali/generalize.hpp>

#endif