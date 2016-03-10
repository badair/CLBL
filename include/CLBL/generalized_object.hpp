 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_OBJECT_WRAPPER_HPP
#define CLBL_OBJECT_WRAPPER_HPP

#include <CLBL/constraints.hpp>

namespace clbl {
template<typename T, typename = std::true_type>
struct generalized_object;

template<typename T>
struct generalized_object<T, cannot_dereference_object<typename std::remove_reference<T>::type>> {

    static constexpr bool volatile_enabled = true;

    using type = typename std::remove_reference<T>::type;
    type object;
    using original_type = T;

    static constexpr const quali::flags q_flags = quali::cv_of<type>::value;

    generalized_object(generalized_object const &) = default;
    generalized_object(generalized_object&&) = default;

    inline constexpr
    generalized_object(generalized_object const volatile & t)
        : object{t.object}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const & t
    ) : object{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const volatile & t
    ) : object{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type&& t
    ) : object{static_cast<T&&>(t)}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U> const & t)
        : object{t.object}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U> const volatile & t)
        : object{t.object}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U>&& t)
        : object{static_cast<generalized_object<U>&&>(t).object}{}

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return quali::cast<QFlags>(object);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const {
        return quali::cast<QFlags | quali::const_>(object);
    }

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() volatile {
        return quali::cast<QFlags | quali::volatile_>(object);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return quali::cast<QFlags | quali::const_ | quali::volatile_>(object);
    }
};

template<typename T>
struct generalized_object<T, can_dereference_object_only_when_non_volatile<typename std::remove_reference<T>::type>> {

    static constexpr bool volatile_enabled = false;

    typename std::remove_reference<T>::type object_ptr;
    using type = typename std::remove_reference<decltype(*object_ptr)>::type;
    using original_type = T;
    static constexpr const quali::flags q_flags =
        quali::cv_of<type>::value | quali::ref_of<decltype(*object_ptr)>::value;

    generalized_object(generalized_object const &) = default;
    generalized_object(generalized_object&&) = default;

    inline constexpr
    generalized_object(generalized_object const volatile & t)
        : object_ptr{t.object_ptr}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const & t
    ) : object_ptr{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const volatile & t
    ) : object_ptr{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type&& t
    ) : object_ptr{static_cast<
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type&&>(t)}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U> const & t)
        : object_ptr{t.object_ptr}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U>&& t)
        : object_ptr{static_cast<generalized_object<U>&&>(t).object_ptr}{}

    template<quali::flags Flags>
    using default_flags = std::integral_constant<quali::flags,
        quali::remove_volatile<quali::remove_reference<Flags>{}>{} | quali::lvalue_reference_
    >;

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR auto
    get() ->
        decltype(quali::cast<default_flags<QFlags>{}>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{}>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr auto
    get() const ->
        decltype(quali::cast<default_flags<QFlags>{} | quali::const_>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{} | quali::const_>(*object_ptr);
    }
};

template<typename T>
struct generalized_object<T, can_dereference_object_when_volatile<typename std::remove_reference<T>::type>> {

    static constexpr bool volatile_enabled = true;

    typename std::remove_reference<T>::type object_ptr;
    using type = typename std::remove_reference<decltype(*object_ptr)>::type;
    using original_type = T;
    static constexpr const quali::flags q_flags =
        quali::cv_of<type>::value | quali::ref_of<decltype(*object_ptr)>::value;

    generalized_object(generalized_object const &) = default;
    generalized_object(generalized_object&&) = default;

    inline constexpr
    generalized_object(generalized_object const volatile & t)
        : object_ptr{t.object_ptr}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const & t
    ) : object_ptr{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type const volatile & t
    ) : object_ptr{t}{}

    inline constexpr
    generalized_object(
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type&& t
    ) : object_ptr{static_cast<
        typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type&&>(t)}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U> const & t)
        : object_ptr{t.object_ptr}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U> const volatile & t)
        : object_ptr{t.object_ptr}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U>&& t)
        : object_ptr{static_cast<generalized_object<U>&&>(t).object_ptr}{}

    template<quali::flags Flags>
    using default_flags = std::integral_constant<quali::flags,
        quali::remove_reference<Flags>{} | quali::lvalue_reference_
    >;

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR auto
    get() ->
        decltype(quali::cast<default_flags<QFlags>{}>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{}>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr auto
    get() const ->
        decltype(quali::cast<default_flags<QFlags>{} | quali::const_>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{} | quali::const_>(*object_ptr);
    }
    
    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR auto
    get() volatile ->
        decltype(quali::cast<default_flags<QFlags>{} | quali::volatile_>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{} | quali::volatile_>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr auto
    get() const volatile ->
        decltype(quali::cast<default_flags<QFlags>{} | quali::const_ | quali::volatile_>(*object_ptr)) {
        return   quali::cast<default_flags<QFlags>{} | quali::const_ | quali::volatile_>(*object_ptr);
    }
};

template<typename T>
struct generalized_object<T, is_reference_wrapper<T>> {

    static constexpr bool volatile_enabled = false;
    
    no_ref<T> ref_wrapped_object;
    using type = no_ref<decltype(std::declval<T>().get())>;
    using original_type = T;

    static constexpr const quali::flags q_flags =
        quali::cv_of<type>::value | quali::ref_of<type>::value;

    generalized_object(generalized_object const &) = default;
    generalized_object(generalized_object&&) = default;

    inline constexpr
    generalized_object(generalized_object const volatile & t)
        : ref_wrapped_object{t.ref_wrapped_object}{}

    inline constexpr
    generalized_object(unqualified<T> t)
        : ref_wrapped_object{t}{}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U> const & t)
        : ref_wrapped_object{ t.ref_wrapped_object } {}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U> const volatile & t)
        : ref_wrapped_object{ t.ref_wrapped_object } {}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U>&& t)
        : ref_wrapped_object{ static_cast<generalized_object<U>&&>(t).ref_wrapped_object } {}

    template<quali::flags Flags>
    using default_flags = std::integral_constant<quali::flags,
        quali::remove_reference<Flags>{} | quali::lvalue_reference_
    >;

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR auto
    get() -> decltype(quali::cast<default_flags<QFlags>{}>(ref_wrapped_object.get())) {
        return ref_wrapped_object.get();
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr auto
    get() const -> decltype(quali::cast<default_flags<QFlags>{} | quali::const_>(ref_wrapped_object.get())) {
        return ref_wrapped_object.get();
    }
};

namespace detail {
    template<typename T>
    struct unwrap_generalized_object {
        using type = T;
    };

    template<typename T>
    struct unwrap_generalized_object<generalized_object<T>> {
        using type = typename generalized_object<T>::type;
    };
}

template<typename T>
using general_object_type = typename detail::unwrap_generalized_object<T>::type;

}

#endif