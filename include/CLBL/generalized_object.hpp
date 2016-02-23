 /*!
@file
Defines `clbl::object_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_OBJECT_WRAPPER_H
#define CLBL_OBJECT_WRAPPER_H

#include <CLBL/cv_of.hpp>
#include <CLBL/tags.hpp>
#include <CLBL/forward.hpp>
#include <CLBL/type_traits.hpp>
#include <CLBL/harden_cast.hpp>
#include <CLBL/constraints.hpp>
#include <CLBL/qflags.hpp>

namespace clbl {

template<typename T, typename = dummy>
struct generalized_object_t {

    T object;
    using type = T;

    template<qualify_flags QFlags = qflags::default_>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    get() {
        return harden_cast<QFlags>(object);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const {
        return harden_cast<QFlags | qflags::const_>(object);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() volatile {
        return harden_cast<QFlags | qflags::volatile_>(object);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return harden_cast<QFlags | qflags::const_ | qflags::volatile_>(object);
    }
};

template<typename T>
struct generalized_object_t<DereferenceableObject<T>, dummy> {

    T object_ptr;
    using type = no_ref<decltype(*object_ptr)>;

    template<qualify_flags QFlags = qflags::default_>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    get() {
        return harden_cast<QFlags>(*object_ptr);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const {
        return harden_cast<QFlags | qflags::const_>(*object_ptr);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() volatile {
        return harden_cast<QFlags | qflags::volatile_>(*object_ptr);
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return harden_cast<QFlags | qflags::const_ | qflags::volatile_>(*object_ptr);
    }
};

template<typename T>
struct generalized_object_t<std::reference_wrapper<T>, dummy> {

    std::reference_wrapper<T> ref_wrapped_object;
    using type = T;

    template<qualify_flags QFlags = qflags::default_>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    get() {
        return harden_cast<QFlags>(ref_wrapped_object.get());
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const {
        return harden_cast<QFlags | qflags::const_>(ref_wrapped_object.get());
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() volatile {
        return harden_cast<QFlags | qflags::volatile_>(ref_wrapped_object.get());
    }

    template<qualify_flags QFlags = qflags::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return harden_cast<QFlags | qflags::const_ | qflags::volatile_>(ref_wrapped_object.get());
    }
};

template<typename T>
using generalized_object = generalized_object_t<no_ref<T>>;

template<typename T, typename = dummy>
struct generalize_t {
    template<typename U>
    inline constexpr decltype(auto)
    operator()(U&& u) const {
        return static_cast<U&&>(u);
    }
};

template<typename T>
struct generalize_t<DereferenceableObject<T>, dummy> {
    template<typename U>
    inline constexpr decltype(auto)
    operator()(U&& u) const {
        return *static_cast<U&&>(u);
    }
};

template<typename T>
struct generalize_t<std::reference_wrapper<T>, dummy> {
    template<typename U>
    inline constexpr decltype(auto)
    operator()(std::reference_wrapper<U> wrapped_u) const {
        return wrapped_u.get();
    }
};

}

#endif