 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_WRAPPER_H
#define CLBL_FUNCTION_WRAPPER_H

namespace clbl { namespace internal {

template<typename Base>
class callable_wrapper 
    : public Base {

public:

    using Base::Base;

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    operator()(Args&&... args) {
        return Base::invoke(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    operator()(Args&&... args) const {
        return Base::invoke(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    operator()(Args&&... args) const volatile {
        return Base::invoke(static_cast<Args&&>(args)...);
    }

    //todo forwarding
    template<typename... Args>
    decltype(auto) bind(Args... args) const {
        using list_type = typename detail::wrapped_args_tuple<no_ref<Args>...>::type;
        return detail::binding_wrapper<callable_wrapper<Base>, list_type>{*this, list_type{args...}};
    }

    template <typename ...Args>
    constexpr partial_wrapper<
        std::make_index_sequence<sizeof...(Args)>,
        Base,
        typename std::decay<Args>::type...
    >
    push_bind(Args&&... args) const {
        return {*this, static_cast<Args&&>(args)...};
    }

    template<
        typename T,
        CLBL_REQUIRES_(std::is_same<typename no_ref<T>::clbl_tag, function_object_wrapper_tag>::value)
    >
    static inline constexpr auto
        copy_invocation(T&& t) {
        return typename no_ref<T>::template add_qualifiers<Base::q_flags | cv_of<T>::value>{t.data};
    }

    template<
        typename T,
        CLBL_REQUIRES_(!std::is_same<typename no_ref<T>::clbl_tag, function_object_wrapper_tag>::value)
    >
    static inline constexpr auto
        copy_invocation(T&& t) {
        return harden_cast<cv_of<T>::value>(static_cast<T&&>(t));
    }
};

}}

#endif