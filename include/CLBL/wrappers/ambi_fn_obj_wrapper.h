#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_H

#include "CLBL/tags.h"
#include "CLBL/cv_checks.h"

#include <type_traits>

namespace clbl {

    template<typename T>
    struct ambi_fn_obj_wrapper {

        using clbl_tag = ambi_fn_obj_tag;
        using type = ambiguous_return(ambiguous_args);
        using args_t = ambiguous_args;
        using return_t = ambiguous_return;

        ambi_fn_obj_wrapper(const std::remove_const_t<T>& o)
            : value(o)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return value(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return value(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return value(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return value(std::forward<Fargs>(a)...);
        }

        static constexpr bool clbl_is_deep_const = is_deep_const<T>() || std::is_const<T>::value;
        static constexpr bool clbl_is_deep_volatile = is_deep_volatile<T>() || std::is_volatile<T>::value;

    private:
        T value;
    };
}

#endif