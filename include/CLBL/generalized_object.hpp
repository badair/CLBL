 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_OBJECT_WRAPPER_HPP
#define CLBL_OBJECT_WRAPPER_HPP

namespace clbl {

template<typename T, typename = void>
struct generalized_object {

    T object;
    using type = T;
    using original_type = T;

    static constexpr const quali::flags q_flags =
        quali::cv_of<type>::value | quali::ref_of<type>::value;

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
struct generalized_object<dereferenceable_object<T>, void> {

    T object_ptr;
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

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return quali::cast<QFlags>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const {
        return quali::cast<QFlags | quali::const_>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() volatile {
        return quali::cast<QFlags | quali::volatile_>(*object_ptr);
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return quali::cast<QFlags | quali::const_ | quali::volatile_>(*object_ptr);
    }
};

template<typename T>
struct generalized_object<std::reference_wrapper<T>, void> {

    std::reference_wrapper<T> ref_wrapped_object;
    using type = T;
    using original_type = std::reference_wrapper<T>;

    static constexpr const quali::flags q_flags =
        quali::cv_of<type>::value | quali::ref_of<type>::value;

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

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() {
        return quali::cast<QFlags>(ref_wrapped_object.get());
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const {
        return quali::cast<QFlags | quali::const_>(ref_wrapped_object.get());
    }

    template<quali::flags QFlags = quali::default_>
    inline QUALI_CXX14_CONSTEXPR decltype(auto)
    get() volatile {
        return quali::cast<QFlags | quali::volatile_>(ref_wrapped_object.get());
    }

    template<quali::flags QFlags = quali::default_>
    inline constexpr decltype(auto)
    get() const volatile {
        return quali::cast<QFlags | quali::const_ | quali::volatile_>(ref_wrapped_object.get());
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