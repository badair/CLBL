#ifndef CLBL_IS_VALID_H
#define CLBL_IS_VALID_H

#include <type_traits>
#include <utility>

/*
Boost.Hana is amazing, which is where this snippet originates
*/

namespace clbl {

    // Written with <3 by Louis Dionne
    // This is in the public domain; do whatever you want with it but don't sue me.

    namespace detail {
        template<typename F, typename... Args, typename = decltype(
        std::declval<F&&>()(std::declval<Args&&>()...))>
        std::true_type is_valid_impl(int);

        template<typename F, typename... Args>
        std::false_type is_valid_impl(...);

        template<typename F>
        struct is_valid_fun {
            template<typename... Args>
            constexpr decltype(is_valid_impl<F, Args&&...>(int{}))
            operator()(Args&&...) const
            { return{}; }
        };
    }

    template<typename F>
    constexpr detail::is_valid_fun<F&&> is_valid(F&&) {
        return {};
    }
}

#endif