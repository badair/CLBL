#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H

#include "CLBL/tags.h"
#include "CLBL/cv_checks.h"

#include <type_traits>

namespace clbl {

    template<typename UnderlyingType, typename TPtr>
    struct overloaded_function_object_ptr
        : callable<overloaded_function_object_ptr<UnderlyingType, TPtr>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object_ptr()
        {}

        overloaded_function_object_ptr(TPtr&& o_ptr)
            : value(std::forward<TPtr>(o_ptr))
        {}

        using my_type = overloaded_function_object_ptr<UnderlyingType, TPtr>;

        overloaded_function_object_ptr(my_type& other) = default;
        overloaded_function_object_ptr(const my_type& other) = default;
        overloaded_function_object_ptr(my_type&& other) = default;
      
        inline overloaded_function_object_ptr(volatile my_type& other)
            : value(other.value)
        {}

        inline overloaded_function_object_ptr(const volatile my_type& other)
            : value(other.value)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return (*value)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return (*value)(std::forward<Fargs>(a)...);
        }

        static constexpr bool clbl_is_deep_const = is_deep_const<UnderlyingType>()
                                                    || std::is_const<UnderlyingType>::value;


        static constexpr bool clbl_is_deep_volatile = is_deep_volatile<UnderlyingType>()
                                                    || std::is_volatile<UnderlyingType>::value;

        TPtr value;
    };
}

#endif