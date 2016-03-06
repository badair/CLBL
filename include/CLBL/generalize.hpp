 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_GENERALIZE_HPP
#define CLBL_GENERALIZE_HPP

namespace clbl {

namespace detail {

    template<typename T, typename = void>
    struct generalize_t {
        template<typename U>
        inline constexpr decltype(auto)
        operator()(U&& u) const {
            return static_cast<U&&>(u);
        }
    };

    template<typename T>
    struct generalize_t<dereferenceable_object<T>, void> {
        template<typename U>
        inline constexpr decltype(auto)
        operator()(U&& u) const {
            return *static_cast<U&&>(u);
        }
    };

    template<typename T>
    struct generalize_t<std::reference_wrapper<T>, void> {
        template<typename U>
        inline constexpr decltype(auto)
        operator()(std::reference_wrapper<U> wrapped_u) const {
            return wrapped_u.get();
        }
    };
}

template<typename T>
inline constexpr decltype(auto)
generalize(T&& t) {
    return detail::generalize_t<
            typename std::remove_reference<T>::type
    >{}(static_cast<T&&>(t));
}

}

#endif