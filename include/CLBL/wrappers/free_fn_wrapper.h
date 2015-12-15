#ifndef CLBL_FREE_FN_WRAPPER_H
#define CLBL_FREE_FN_WRAPPER_H

#include "CLBL/utility.h"
#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/forward.h"
#include "CLBL/harden_cast.h"
#include "CLBL/invocation_macros.h"

namespace clbl {

    //dispatch failure case for free functions
    template<typename, typename Failure>
    struct free_fn_wrapper { static_assert(sizeof(Failure) < 0, "Not a function."); };

    template<typename Creator, typename Return, typename... Args>
    struct free_fn_wrapper<Creator, Return(Args...)> {

        static constexpr qualify_flags cv_flags = qflags::default_;
        using creator = Creator;
        using clbl_tag = free_fn_tag;
        using my_type = free_fn_wrapper<Creator, Return(Args...)>;
        using type = Return(Args...);
        using forwarding_glue = Return(forward<Args>...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;
        using underlying_type = my_type;

        Return(*_value)(Args...);
        dummy _object;

        template<qualify_flags>
        using apply_cv = my_type;

        free_fn_wrapper(Return(*f_ptr)(Args...), dummy d = dummy{})
            : _value(f_ptr) 
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) { 
            return (*_value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_CALL_PTR(const, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_CALL_PTR(volatile, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_CALL_PTR(const volatile, _value, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[&v = c._value](auto&&... args){ 
                return (*v)(args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c._value](auto&&... args){ 
                return CLBL_CALL_PTR(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c._value](auto&&... args){
                return CLBL_CALL_PTR(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c._value](auto&&... args){
                return CLBL_CALL_PTR(const volatile, v, args...);
            };
        }
    };
}

#endif