#ifndef CLBL_CV_CHECKS_H
#define CLBL_CV_CHECKS_H

#include<type_traits>
#include <boost/hana.hpp>

//sorry to anyone reading this - this is terribly tricky, and is very hard without macros

namespace hana = boost::hana;

namespace clbl {

    static auto is_const_checker = hana::is_valid([](auto c)
        -> decltype(decltype(c)::clbl_is_deep_const) {}
    );

    static auto is_volatile_checker = hana::is_valid([](auto c)
        -> decltype(decltype(c)::clbl_is_deep_volatile) {}
    );

    template<typename T>
    static inline constexpr auto is_deep_const() -> std::enable_if_t<decltype(is_const_checker(std::declval<T>()))::value, bool> {
        return T::clbl_is_deep_const;
    }

    template<typename T>
    static inline constexpr auto is_deep_const() -> std::enable_if_t<!decltype(is_const_checker(std::declval<T>()))::value, bool> {
        return false;
    }

    template<typename T>
    static inline constexpr auto is_deep_volatile() -> std::enable_if_t<decltype(is_volatile_checker(std::declval<T>()))::value, bool> {
        return T::clbl_is_deep_volatile;
    }

    template<typename T>
    static inline constexpr auto is_deep_volatile() -> std::enable_if_t<!decltype(is_volatile_checker(std::declval<T>()))::value, bool> {
        return false;
    }

    template<typename T>
    static inline constexpr auto is_clbl() -> std::enable_if_t<decltype(is_const_checker(std::declval<T>()))::value, bool> {
        return true;
    }

    template<typename T>
    static inline constexpr auto is_clbl() -> std::enable_if_t<!decltype(is_const_checker(std::declval<T>()))::value, bool> {
        return false;
    }
}

#endif