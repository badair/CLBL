#ifndef CLBL_FREE_FUNCTION_H
#define CLBL_FREE_FUNCTION_H

#include "CLBL/callable.h"

namespace clbl {

    //dispatch failure case for free functions
    template<typename Failure>
    struct free_function {

        template<typename... T>
        free_function(T...) {}

        free_function() {}

        template<typename... T>
        auto operator()(T...) { static_assert(sizeof...(T) < 0, "Not a function."); }

        template<typename T = void>
        auto operator()(...) { static_assert(sizeof(T) < 0, "Not a function."); }
    };

    //specialization for free function types
    template<typename Return, typename... Args>
    struct free_function<Return(Args...)> : callable<free_function<Return(Args...)>, Return(Args...)> {

        free_function(Return(*f_ptr)(Args...)) 
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
    struct free_function<Return(Args..., ...)> : callable<free_function<Return(Args..., ...)>, Return(Args..., ...)> {

        free_function(Return(*p)(Args..., ...)) : value(p) {}

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