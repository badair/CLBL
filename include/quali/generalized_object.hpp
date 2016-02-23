 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef QUALI_OBJECT_WRAPPER_HPP
#define QUALI_OBJECT_WRAPPER_HPP

namespace quali {

template<typename T, typename = void>
struct generalized_object {

    T object;
    using type = T;
    using original_type = T;

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
    generalized_object(generalized_object<U, void> const & t)
        : object{t.object}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U, void> const volatile & t)
        : object{t.object}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U, void>&& t)
        : object{static_cast<generalized_object<U, void>&&>(t).object}{}

    template<flags QFlags = default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return cast<QFlags>(object);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const {
        return cast<QFlags | const_>(object);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() volatile {
        return cast<QFlags | volatile_>(object);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return cast<QFlags | const_ | volatile_>(object);
    }
};

template<typename T>
struct generalized_object<dereferenceable_object<T>, void> {

    T object_ptr;
    using type = typename std::remove_reference<decltype(*object_ptr)>::type;
    using original_type = T;

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
    generalized_object(generalized_object<U, void> const & t)
        : object_ptr{t.object_ptr}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U, void> const volatile & t)
        : object_ptr{t.object_ptr}{}

    template<typename U>
    inline constexpr
    generalized_object(generalized_object<U, void>&& t)
        : object_ptr{static_cast<generalized_object<U, void>&&>(t).object_ptr}{}

    template<flags QFlags = default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return cast<QFlags>(*object_ptr);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const {
        return cast<QFlags | const_>(*object_ptr);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() volatile {
        return cast<QFlags | volatile_>(*object_ptr);
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return cast<QFlags | const_ | volatile_>(*object_ptr);
    }
};

template<typename T>
struct generalized_object<std::reference_wrapper<T>, void> {

    std::reference_wrapper<T> ref_wrapped_object;
    using type = T;
    using original_type = std::reference_wrapper<T>;

    generalized_object(generalized_object const &) = default;
    generalized_object(generalized_object&&) = default;

    inline constexpr
    generalized_object(generalized_object const volatile & t)
        : ref_wrapped_object{t.ref_wrapped_object}{}

    inline constexpr
    generalized_object(std::reference_wrapper<T> t)
        : ref_wrapped_object{t}{}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U, void> const & t)
        : ref_wrapped_object{ t.ref_wrapped_object } {}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U, void> const volatile & t)
        : ref_wrapped_object{ t.ref_wrapped_object } {}

    template<typename U>
    inline constexpr
        generalized_object(generalized_object<U, void>&& t)
        : ref_wrapped_object{ static_cast<generalized_object<U, void>&&>(t).ref_wrapped_object } {}

    template<flags QFlags = default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return cast<QFlags>(ref_wrapped_object.get());
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const {
        return cast<QFlags | const_>(ref_wrapped_object.get());
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() volatile {
        return cast<QFlags | volatile_>(ref_wrapped_object.get());
    }

    template<flags QFlags = default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return cast<QFlags | const_ | volatile_>(ref_wrapped_object.get());
    }
};

namespace detail {
    template<typename T>
    struct unwrap_generalized_object {
        using type = T;
    };

    template<typename T>
    struct unwrap_generalized_object<generalized_object<T, void>> {
        using type = typename generalized_object<T, void>::type;
    };
}

template<typename T>
using general_object_type = typename detail::unwrap_generalized_object<T>::type;

}

#endif