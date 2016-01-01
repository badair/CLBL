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
#include <CLBL/no_ref.h>
#include <CLBL/is_clbl.h>

namespace clbl {
	/*
    clbl::args is a metafunction to extract the arg_types
    alias of a CLBL wrapper
    */
    template<typename Callable>
    using args = typename no_ref<Callable>::arg_types;

    namespace detail {
        template<typename... Args>
        struct has_args_t {

            template<typename T,
                std::enable_if_t<!is_clbl<no_ref<T> >, dummy>* = nullptr>
            inline constexpr auto
            operator()(T&&) const {
                static_assert(sizeof(T) < 0, "You didn't pass a CLBL callable wrapper to clbl::has_args.");
                return false;
            }

            template<typename T,
                std::enable_if_t<is_clbl<no_ref<T> >, dummy>* = nullptr>
            inline constexpr auto
            operator()(T&&) const {
                return std::is_same<args<T>, std::tuple<Args...> >::value;
            }
        };
    }
    
    template<typename... Args>
    constexpr auto has_args = detail::has_args_t<Args...>{};
}

#endif