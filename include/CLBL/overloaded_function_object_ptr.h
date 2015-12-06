#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H

#include "tags.h"

#include <type_traits>

namespace clbl {

    template<typename UnderlyingType, typename TPtr>
    struct overloaded_function_object_ptr
        //CRTP to callable
        : callable<overloaded_function_object_ptr<UnderlyingType, TPtr>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object_ptr(TPtr&& o_ptr)
            : ptr(std::forward<TPtr>(o_ptr))
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

    private:
        TPtr ptr;
    };
}

#endif