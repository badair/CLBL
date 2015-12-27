/*!
@file
Defines `clbl::result_of` and clbl::has_return.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_RETURN_H
#define CLBL_RETURN_H

namespace clbl {

    template<typename Callable>
    using result_of = typename no_ref<Callable>::return_type;

    template<typename ReturnType, typename Callable, std::enable_if_t<is_clbl<no_ref<Callable> >, dummy>*  = nullptr>
    static inline constexpr auto
    has_return(Callable&&) {
        return std::is_same<typename no_ref<Callable>::return_type, ReturnType>::value;
    }

    template<typename ReturnType, typename T, std::enable_if_t<!is_clbl<no_ref<T> >, dummy>* = nullptr>
    static constexpr auto
    has_return(T&&) {
        static_assert(sizeof(T) < 0,
            "You didn't pass a CLBL callable wrapper to clbl::returns.");
        return false;
    }
}

#endif