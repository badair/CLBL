#ifndef CLBL_CALLABLE_H
#define CLBL_CALLABLE_H

#include <boost/hana.hpp>
#include <type_traits>

#include "CLBL/func.h"
#include "CLBL/tags.h"

namespace hana = boost::hana;
using namespace hana::literals;

namespace clbl {

    namespace detail {

        //operator() call checker used by can_call_impl below
        template<typename T, typename... Args>
        auto can_call_v = hana::is_valid([](auto&&... args)
            -> decltype(std::declval<T>()(std::forward<Args>(args)...)) {}
        );

        template<typename CallableDerived>
        struct can_call_impl {

            /*
            Returns a true type when passing arguments to the CallableDerived operator()
            does not result in a compiler error, otherwise a returns a false type
            */

            template<typename... Args>
            inline constexpr auto can_call(Args&&... args) {
                return can_call_v<CallableDerived, Args...>(std::forward<Args>(args)...);
            }
        };

        template<typename T>
        auto unwrap(T&& t) {
            return std::forward<T>(t);
        }

        template<>
        template<typename T>
        T& unwrap(std::reference_wrapper<T> t) {
            return T.get();
        }

        struct partial_t {
            template<typename Callable, typename... Args>
            inline auto operator()(Callable c, Args... args) {
                return func(hana::partial(std::move(c), unwrap(args)...));
            }
        };

        //todo test partial

        template<typename T>
        constexpr partial_t partial{};

        /*
            //todo .reorder() member function creates a new callable with the arguments reordered
        */

        /*
        todo .bind(Args...) function on callable that returns a new callable with certain arguments bound from left to right
        by default, with specializations for indexed_argument_bind arguments
        */

        /*
        todo free function bind_arg(i, arg) creates an indexed_argument_bind for passing to .bind()
        */

        /*
        todo .harden<function_type>() member function to specify valid function argument types
        design a mechanism for "overloading" .harden()-ed callables by calling .overload()

        make sure to forward args to hardened callables while allowing user to not worry about
        reference types. This will be the trickiest part
        */

        /*
        todo ellipses specializations for all callable implementations
        */
    }

    

    //dispatch failure case
    template<typename, typename Bad>
    struct callable { static_assert(sizeof(Bad) < 0, "Not a valid callable type."); };

    //todo specialize for ambiguous case - allow arity to be passed? 

    //callable is the end result of a func(...) call
    template<typename Derived, typename Return, typename... Args>
    struct callable<Derived, Return(Args...)> {

        using type = Return(Args...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;
    };//todo ellipses
}

#endif