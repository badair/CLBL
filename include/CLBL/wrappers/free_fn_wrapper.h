#ifndef CLBL_FREE_FUNCTION_H
#define CLBL_FREE_FUNCTION_H

#include "CLBL/callable.h"
#include "CLBL/utility.h"
#include "CLBL/utility.h"

namespace clbl {

    //dispatch failure case for free functions
    template<typename Failure>
    struct free_fn_wrapper { static_assert(sizeof(Failure) < 0, "Not a function."); };

    template<typename Return, typename... Args>
    struct free_fn_wrapper<Return(Args...)> {

        using clbl_tag = free_fn_tag;
        using my_type = free_fn_wrapper<Return(Args...)>;
        using type = Return(Args...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;

        free_fn_wrapper(Return(*f_ptr)(Args...)) 
            : value(f_ptr) 
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) { 
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_PTR(const, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_PTR(volatile, value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_PTR(const volatile, value, std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_deep_const = false;
        static constexpr auto clbl_is_deep_volatile = false;

        static inline constexpr auto copy_invocation(my_type& c) {
            return[&v = c.value](auto&&... args){ 
                return (*v)(args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c.value](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_PTR(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c.value](auto&&... args){
                return CLBL_UPCAST_AND_CALL_PTR(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c.value](auto&&... args){
                return CLBL_UPCAST_AND_CALL_PTR(const volatile, v, args...);
            };
        }

    private:
        Return(*value)(Args...);
    };

    template<typename Return, typename... Args>
    struct free_fn_wrapper<Return(Args..., ...)> {

        using clbl_tag = free_fn_tag;
        using type = Return(Args..., ...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;

        using my_type = free_fn_wrapper<Return(Args..., ...)>;

        free_fn_wrapper(Return(*p)(Args..., ...)) : value(p) {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... args, ...) { 
            return (*value)(std::forward<Fargs>(a)..., ...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... args, ...) const {
            return (*value)(std::forward<Fargs>(a)..., ...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... args, ...) volatile {
            return (*value)(std::forward<Fargs>(a)..., ...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... args, ...) const volatile {
            return (*value)(std::forward<Fargs>(a)..., ...);
        }
        static constexpr auto clbl_is_deep_const = false;
        static constexpr auto clbl_is_deep_volatile = false;

        static constexpr inline auto copy_invocation(my_type& c) {
            return[v = c.value](auto&&... args){ return (*v)(args...);};
        }

        static constexpr inline auto copy_invocation(const my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const, v, args...);};
        }

        static constexpr inline auto copy_invocation(volatile my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(volatile, v, args...);};
        }

        static constexpr inline auto copy_invocation(const volatile my_type& c) {
            return[v = c.value](auto&&... args){ return CLBL_UPCAST_AND_CALL_PTR(const volatile, v, args...);};
        }

    private:
        Return(*value)(Args..., ...);
    };
}

#endif