/*!
@file
Defines `clbl::args` and `clbl::has_args`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_ARGS_H
#define CLBL_ARGS_H

#include <type_traits>
#include <tuple>
#include <CLBL/tags.h>
#include <CLBL/constraints.h>
#include <CLBL/type_traits.h>
#include <CLBL/is_clbl.h>

namespace clbl {
	/*
    clbl::args is a metafunction to extract the arg_types
    alias of a CLBL wrapper
    */
    template<typename Callable>
    using args = typename no_ref<Callable>::arg_types;

    template<typename Callable, size_t Index>
    using arg_at = typename std::tuple_element<Index, args<Callable> >::type;
    
    //todo add tests
    template<typename Callable>
    using first_arg = arg_at<Callable, 0>;
    
    template<typename Callable>
    using second_arg = arg_at<Callable, 1>;
    
    template<typename Callable>
    using last_arg = arg_at<Callable, std::tuple_size<args<Callable> >::value - 1>;
    
    //todo - arity, better error messages, etc
    namespace detail {
        template<typename... Args>
        struct has_args_t {

            template<
                typename T,
                CLBL_REQUIRES_(!is_clbl<no_ref<T> >)
            >
            inline constexpr auto
            operator()(T&&) const {
                static_assert(sizeof(T) < 0, "You didn't pass a CLBL callable wrapper to clbl::has_args.");
                return false;
            }

            template<
                typename T,
                CLBL_REQUIRES_(is_clbl<no_ref<T> >)
            >
            inline constexpr auto
            operator()(T&&) const {
                return is_same<args<T>, std::tuple<Args...> >;
            }
        };
    }
    
    template<typename... Args>
    constexpr auto has_args = detail::has_args_t<Args...>{};
    
    
}

#endif