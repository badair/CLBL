/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_MACROS_HPP
#define QUALI_MACROS_HPP

#include <quali/quali.hpp>

#define QUALI_DEFAULT
#define QUALI_BEGIN_TOKENS
#define QUALI_END_TOKENS 
#define QUALI_BEGIN_FLAGS (
#define QUALI_END_FLAGS )
#define QUALI_COMBINE_TOKENS
#define QUALI_COMBINE_FLAGS |

#define QUALI_PP_CAT_(X, Y) X ## Y
#define QUALI_PP_CAT(X, Y)  QUALI_PP_CAT_(X, Y)
#define QUALI_STRINGIFY(...) #__VA_ARGS__

#define QUALI_TOKENS(x) x

#define QUALI_FLAGS(x) \
    quali::pp_detail::map_to_flags_by_lexical_length< \
        std::extent< \
            typename std::remove_reference<decltype(QUALI_STRINGIFY(x))>::type \
        >::value, \
        int \
    >::value

#define QUALI_ELEMENT(type, e) QUALI_PP_CAT(QUALI_, type)(e)

/*
taken from potatoswatter's answer on macro overloading at 
http://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
*/
#define QUALI_MACRO_SELECT(MACRO_NAME, INDEX) QUALI_PP_CAT(MACRO_NAME ## _, INDEX)
#define QUALI_GET_COUNT(_1, _2, _3, _4, _5, _6, _7 /* ad nauseam */, COUNT, ...) COUNT
#define QUALI_VA_SIZE(...) QUALI_GET_COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1 )
#define QUALI_VA_SELECT(NAME, ...) QUALI_MACRO_SELECT(NAME, QUALI_VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
#define QUALI(x, ...) QUALI_VA_SELECT(QUALI_IMPL, x, __VA_ARGS__)



#define QUALI_IMPL_1( type ) QUALI_PP_CAT(QUALI_, type)(QUALI_DEFAULT)

#define QUALI_IMPL_2( type, arg0 ) QUALI_ELEMENT(type, arg0)

#define QUALI_IMPL_3( type, arg0, arg1 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_IMPL_4( type, arg0, arg1, arg2 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg2) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_IMPL_5( type, arg0, arg1, arg2, arg3 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg2) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg3) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_IMPL_6( type, arg0, arg1, arg2, arg3, arg4 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg2) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg3) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg4) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_IMPL_7( type, arg0, arg1, arg2, arg3, arg4, arg5 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg2) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg3) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg4) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg5) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_IMPL_8( type, arg0, arg1, arg2, arg3, arg4, arg5, arg6 ) \
QUALI_PP_CAT(QUALI_BEGIN_, type) \
QUALI_ELEMENT(type, arg0) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg1) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg2) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg3) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg4) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg5) \
QUALI_PP_CAT(QUALI_COMBINE_, type) \
QUALI_ELEMENT(type, arg6) \
QUALI_PP_CAT(QUALI_END_, type)

#define QUALI_DEBUG_(x, ...) \
    static_assert(false, "QUALI("#__VA_ARGS__") expands to '" QUALI_STRINGIFY(x) "'.")

#define QUALI_DEBUG(...) QUALI_DEBUG_(QUALI(__VA_ARGS__), __VA_ARGS__)

#define QUALI_STR_(...) QUALI_STRINGIFY(__VA_ARGS__)
#define QUALI_STR(...) QUALI_STR_(QUALI(__VA_ARGS__))

#define QUALI_APPLY_ALL_PERMUTATIONS(external_macro, ...) \
    external_macro(QUALI_DEFAULT) __VA_ARGS__       \
    external_macro(&) __VA_ARGS__                   \
    external_macro(&&) __VA_ARGS__                  \
    external_macro(const) __VA_ARGS__               \
    external_macro(volatile) __VA_ARGS__            \
    external_macro(const, &) __VA_ARGS__            \
    external_macro(const, &&) __VA_ARGS__           \
    external_macro(volatile, &) __VA_ARGS__         \
    external_macro(volatile, &&) __VA_ARGS__        \
    external_macro(const, volatile, &) __VA_ARGS__  \
    external_macro(const, volatile, &&)             \
/**/

namespace quali {

    namespace pp_detail {
        template<std::size_t N, typename T = int>
        struct map_to_flags_by_lexical_length {
            static_assert(sizeof(T) < 0,
                "Invalid argument passed to QUALI macro. Expected 'const', 'volatile', '&', or '&&' ");
        };

        //mapping flags by length of string
        template<> struct map_to_flags_by_lexical_length<1, int> { static constexpr auto value = default_; };
        template<> struct map_to_flags_by_lexical_length<2, int> { static constexpr auto value = lref_;    };
        template<> struct map_to_flags_by_lexical_length<3, int> { static constexpr auto value = rref_;    };
        template<> struct map_to_flags_by_lexical_length<6, int> { static constexpr auto value = const_;   };
        template<> struct map_to_flags_by_lexical_length<9, int> { static constexpr auto value = volatile_;};
    }
}

#endif