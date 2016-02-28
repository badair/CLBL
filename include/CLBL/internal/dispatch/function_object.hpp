/*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_OBJECT_H
#define CLBL_FUNCTION_OBJECT_H

namespace clbl {

namespace function_object_detail {
    struct empty_base{};
}

template<typename GeneralizedObject>
struct function_object 
    : public std::conditional<
        has_normal_call_operator<typename GeneralizedObject::type>::value,
        pmf<
            std::integral_constant<
                decltype(&default_normal_callable<typename GeneralizedObject::type>::operator()),
                &default_normal_callable<typename GeneralizedObject::type>::operator()
            >
        >,
        function_object_detail::empty_base
    >::type
{
    static constexpr const bool is_valid = !is_clbl<typename GeneralizedObject::type>::value;
    static constexpr const bool value = is_valid;
    using general_type = typename GeneralizedObject::type;
    static constexpr auto is_clbl = clbl::is_clbl<general_type>::value;
    static constexpr const bool is_ambiguous = !has_normal_call_operator<general_type>::value;
    using type = GeneralizedObject;
    using constructor_type = GeneralizedObject;
    using dispatch_type = function_object;
};

template<typename T, typename U>
struct function_object <generalized_object_t<T U::*, dummy> > {
    static constexpr const bool is_valid = false;
    static constexpr const bool value = is_valid;
};

template<typename T, T Value>
struct function_object<generalized_object_t<std::integral_constant<T, Value>, dummy>> {
    static constexpr const bool is_valid = false;
    static constexpr const bool value = is_valid;
};

template<typename T>
struct wrapper_object {
    static constexpr const bool is_valid = is_clbl<T>::value;
    static constexpr const bool value = is_valid;
    using dispatch_type = wrapper_object;
};

template<typename T>
struct wrapper_object <std::reference_wrapper<T>> {
    static constexpr const bool is_valid = is_clbl<T>::value;
    static constexpr const bool value = is_valid;
    using dispatch_type = wrapper_object;
};

}

#endif
