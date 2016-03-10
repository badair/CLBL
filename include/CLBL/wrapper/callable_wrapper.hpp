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

    using arity = clbl::arity<callable_wrapper>;

#ifdef CLBL_GCC_5_REF_OVERLOAD_WORKAROUND

    //https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60943
    //GCC 5 can't resolve ref-qualified overloads with auto deduced return type,
    //so we omit this functionality. TODO document this
    
    #define CLBL_LVALUE_QUALIFIER

#else

    #define CLBL_LVALUE_QUALIFIER &

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(&&)), typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    operator()(Args&&... args) &&
        -> decltype(this ->template move_invoke<Flags>(static_cast<Args&&>(args)...)){
        return Base::template move_invoke<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(const, &&)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) const &&
        -> decltype(this->template move_invoke_c<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template move_invoke_c<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(volatile, &&)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) volatile &&
        -> decltype(this->template move_invoke_v<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template move_invoke_v<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(const, volatile, &&)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) const volatile &&
        -> decltype(this->template move_invoke_cv<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template move_invoke_cv<Flags>(static_cast<Args&&>(args)...);
    }

#endif

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(QUALI_DEFAULT)), typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    operator()(Args&&... args) CLBL_LVALUE_QUALIFIER
        -> decltype(this->template invoke<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template invoke<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(const)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) const CLBL_LVALUE_QUALIFIER 
        -> decltype(this->template invoke_c<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template invoke_c<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(volatile)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) volatile CLBL_LVALUE_QUALIFIER
        -> decltype(this->template invoke_v<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template invoke_v<Flags>(static_cast<Args&&>(args)...);
    }

    template<CLBL_FORCE_SFINAE(Flags, QUALI_INTC(const, volatile)), typename... Args>
    inline constexpr auto
    operator()(Args&&... args) const volatile CLBL_LVALUE_QUALIFIER
        -> decltype(this->template invoke_cv<Flags>(static_cast<Args&&>(args)...)) {
        return Base::template invoke_cv<Flags>(static_cast<Args&&>(args)...);
    }

#undef CLBL_LVALUE_QUALIFIER
    
    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    can_call(Args&&...) //CLBL_LVALUE_QUALIFIER
        -> can_call_with<decltype(*this), Args&&...> {
        return {};
    }

    //todo forwarding
    template<typename... Args>
    decltype(auto)
    bind(Args... args) const {
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
        CLBL_REQUIRES_(
            std::is_same<
                typename no_ref<T>::clbl_tag,
                function_object_wrapper_tag
            >::value
        )
    >
    static inline constexpr auto
    copy_wrapper(T&& t) {
        return typename Base::template
                add_qualifiers<Base::q_flags | quali::cv_of<T>::value>{t.data};
    }

    template<
        typename T,
        CLBL_REQUIRES_(
            !std::is_same<
                typename no_ref<T>::clbl_tag,
                function_object_wrapper_tag
            >::value
        )
    >
    static inline constexpr auto
    copy_wrapper(T&& t) {
        return quali::cast(static_cast<T&&>(t));
    }

    template<
        typename T,
        CLBL_REQUIRES_(
            std::is_same<
                typename no_ref<T>::clbl_tag,
                function_object_wrapper_tag
            >::value
        )
    >
    static inline constexpr decltype(auto)
    move_wrapper(T&& t) {
        return typename Base::template
            add_qualifiers<Base::q_flags | quali::cv_of<T>::value>{
                static_cast<no_ref<T>&&>(t).data
            };
    }

    template<
        typename T,
        CLBL_REQUIRES_(
            !std::is_same<
                typename no_ref<T>::clbl_tag,
                function_object_wrapper_tag
            >::value
        )
    >
    static inline constexpr decltype(auto)
    move_wrapper(T&& t) {
        return static_cast<
            quali::qualify<
                    no_ref<T>,
                    quali::rvalue_reference_ | quali::cv_of<T>::value
                >
            >(static_cast<T&&>(t));
    }

    template<typename T>
    static inline constexpr decltype(auto)
    transform(T& t){
        return copy_wrapper(t);
    }

    template<typename T>
    static inline constexpr decltype(auto)
    transform(T&& t){
        return move_wrapper(static_cast<T&&>(t));
    }
};

}}

#endif