#ifndef CLBL_FREE_FUNCTION_H
#define CLBL_FREE_FUNCTION_H

#include "CLBL/callable.h"

namespace clbl {

    //dispatch failure case for free functions
    template<typename Failure>
    struct free_fn_wrapper { static_assert(sizeof(Failure) < 0, "Not a function."); };

    template<typename Return, typename... Args>
    struct free_fn_wrapper<Return(Args...)> {

        using clbl_tag = free_fn_tag;
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
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return (*value)(std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_deep_const = false;
        static constexpr auto clbl_is_deep_volatile = false;

    private:
        Return(*value)(Args...);
    };

    template<typename Return, typename... Args>
    struct free_fn_wrapper<Return(Args..., ...)> {

        using clbl_tag = free_fn_tag;
        using type = Return(Args..., ...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;

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

    private:
        Return(*value)(Args..., ...);
    };
}

#endif