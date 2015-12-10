#ifndef CLBL_CV_CHECKS_H
#define CLBL_CV_CHECKS_H

#include<type_traits>
#include <boost/hana.hpp>

//sorry to anyone reading this - this is terribly tricky, and is very hard without macros

namespace hana = boost::hana;

namespace clbl {

    static auto has_deeper_const = hana::is_valid([](auto c)
        -> decltype(decltype(c)::clbl_is_deep_const) {}
    );

    static auto has_deeper_volatile = hana::is_valid([](auto c)
        -> decltype(decltype(c)::clbl_is_deep_volatile) {}
    );

    template<typename T>
    static inline constexpr auto is_deep_const() -> std::enable_if_t<decltype(has_deeper_const(std::declval<T>()))::value, bool> {
        return T::clbl_is_deep_const;
    }

    template<typename T>
    static inline constexpr auto is_deep_const() -> std::enable_if_t<!decltype(has_deeper_const(std::declval<T>()))::value, bool> {
        return false;
    }

    template<typename T>
    static inline constexpr auto is_deep_volatile() -> std::enable_if_t<decltype(has_deeper_volatile(std::declval<T>()))::value, bool> {
        return T::clbl_is_deep_volatile;
    }

    template<typename T>
    static inline constexpr auto is_deep_volatile() -> std::enable_if_t<!decltype(has_deeper_volatile(std::declval<T>()))::value, bool> {
        return false;
    }

    template<typename T>
    static constexpr auto is_clbl = decltype(has_deeper_const(std::declval<T>()))::value;
}

#endif