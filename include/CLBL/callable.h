#ifndef CLBL_CALLABLE_H
#define CLBL_CALLABLE_H

#include <boost/hana.hpp>
#include <type_traits>

#include "CLBL/clbl.h"
#include "CLBL/tags.h"

namespace hana = boost::hana;
using namespace hana::literals;

namespace clbl {

    namespace detail {

        /*
        template<typename T, typename... Args>
        auto can_call_v = hana::is_valid([](auto&&... args)
            -> decltype(std::declval<T>()(std::forward<Args>(args)...)) {}
        );

        template<typename CallableDerived>
        struct can_call_impl {
            template<typename... Args>
            inline constexpr auto can_call(Args&&... args) {
                return can_call_v<CallableDerived, Args...>(std::forward<Args>(args)...);
            }
        };*/

        /*template<typename T>
        auto unwrap(T&& t) {
            return std::forward<T>(t);
        }

        template<typename T>
        T& unwrap(std::reference_wrapper<T> t) {
            return T.get();
        }

        struct partial_t {
            
        };*/

        //todo test partial

        /*template<typename Callable, typename... Args>
        inline constexpr auto partial(Callable c, Args... args) {
            return fwrap(hana::partial(std::move(c), unwrap(args)...));
        }*/


        /*
            //todo .reorder() member function creates a new callable with the arguments reordered
        */
        /*
        todo .bind(Args...) function on callable that returns a new callable with certain arguments bound from left to right
        by default, with specializations for indexed_argument_bind arguments
        */
        /*
        todo ellipses, ref qualifiers for all callable implementations
        */
    }


}

#endif