/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_MACROS_HPP
#define QUALI_MACROS_HPP

#ifdef QUALI_MACROS_UNDEF_HPP
#undef QUALI_MACROS_UNDEF_HPP
#endif

#include <quali/quali.hpp>
#include <quali/lexical_flag_map.hpp>
#include <type_traits>

#define QUALI_DEFAULT

#define QUALI_PP_CAT_(X, Y) X ## Y
#define QUALI_PP_CAT(X, Y)  QUALI_PP_CAT_(X, Y)
#define QUALI_STRINGIFY(...) #__VA_ARGS__

/*********** QUALI_TOKENS expansion *************/

#define QUALI_BEGIN_TOKENS
#define QUALI_DELIMITER_TOKENS
#define QUALI_END_TOKENS 
#define QUALI_EXPAND_TOKENS(x) x

/*********** QUALI_FLAGS expansion *************/

#define QUALI_BEGIN_FLAGS (
#define QUALI_DELIMITER_FLAGS |
#define QUALI_END_FLAGS )
#define QUALI_EXPAND_FLAGS(x) \
    quali::pp_detail::lexical_flag_map< \
        std::extent< \
            typename std::remove_reference<decltype(QUALI_STRINGIFY(x))>::type \
        >::value, \
        int \
    >::value
/**/

/*********** QUALI_INTC expansion **************/

#define QUALI_BEGIN_INTC std::integral_constant<quali::flags, (
#define QUALI_DELIMITER_INTC |
#define QUALI_END_INTC )>
#define QUALI_EXPAND_INTC(x) \
    quali::pp_detail::lexical_flag_map< \
        std::extent< \
            typename std::remove_reference<decltype(QUALI_STRINGIFY(x))>::type \
        >::value, \
        int \
    >::value

/*********** boilerplate for macro overloads **************/

/*
taken from potatoswatter's answer on macro overloading at 
http://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
*/
#define QUALI_MACRO_SELECT(MACRO_NAME, INDEX) QUALI_PP_CAT(MACRO_NAME ## _, INDEX)
#define QUALI_GET_COUNT(_1, _2, _3, _4, _5, COUNT, ...) COUNT
#define QUALI_VA_SIZE(...) QUALI_GET_COUNT(__VA_ARGS__, 5, 4, 3, 2, 1 )
#define QUALI_VA_SELECT(NAME, ...) QUALI_MACRO_SELECT(NAME, QUALI_VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
#define QUALI_EXPAND(x, ...) QUALI_VA_SELECT(QUALI_IMPL, x, __VA_ARGS__)

#define QUALI_ELEMENT(tag, e) QUALI_PP_CAT(QUALI_EXPAND_, tag)(e)

#define QUALI_IMPL_1( tag ) //not used

#define QUALI_IMPL_2( tag, arg0 ) \
QUALI_PP_CAT(QUALI_BEGIN_, tag) \
QUALI_ELEMENT(tag, arg0) \
QUALI_PP_CAT(QUALI_END_, tag)
/**/

#define QUALI_IMPL_3( tag, arg0, arg1 ) \
QUALI_PP_CAT(QUALI_BEGIN_, tag) \
QUALI_ELEMENT(tag, arg0) \
QUALI_PP_CAT(QUALI_DELIMITER_, tag) \
QUALI_ELEMENT(tag, arg1) \
QUALI_PP_CAT(QUALI_END_, tag)
/**/

#define QUALI_IMPL_4( tag, arg0, arg1, arg2 ) \
QUALI_PP_CAT(QUALI_BEGIN_, tag) \
QUALI_ELEMENT(tag, arg0) \
QUALI_PP_CAT(QUALI_DELIMITER_, tag) \
QUALI_ELEMENT(tag, arg1) \
QUALI_PP_CAT(QUALI_DELIMITER_, tag) \
QUALI_ELEMENT(tag, arg2) \
QUALI_PP_CAT(QUALI_END_, tag)
/**/

#define QUALI_IMPL_5(...) ; static_assert(false, "TOO MANY ARGUMENTS PASSED TO QUALI MACRO") ;

#define QUALI_FLAGS(...) QUALI_EXPAND(FLAGS, __VA_ARGS__)
#define QUALI_TOKENS(...) QUALI_EXPAND(TOKENS, __VA_ARGS__)
#define QUALI_INTC(...) QUALI_EXPAND(INTC, __VA_ARGS__)

#endif //QUALI_MACROS_HPP