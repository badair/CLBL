#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_H

#include "CLBL/tags.h"
#include "CLBL/cv_checks.h"

#include <type_traits>

namespace clbl {

    template<typename T>
    struct overloaded_function_object
        //CRTP to callable
        : callable<overloaded_function_object<T>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object(T o)
            : obj(o)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return static_cast<std::remove_cv_t<std::remove_reference_t<decltype(obj)> > &>(obj)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return static_cast<const std::remove_cv_t<std::remove_reference_t<decltype(obj)> > &>(obj)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return static_cast<volatile std::remove_cv_t<std::remove_reference_t<decltype(obj)> > &>(obj)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return static_cast<const volatile std::remove_cv_t<std::remove_reference_t<decltype(obj)> > &>(obj)(std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_const = is_clbl_const<T>() || std::is_const<T>::value;
        static constexpr auto clbl_is_volatile = is_clbl_volatile<T>() || std::is_volatile<T>::value;

    private:
        T obj;
    };
}

#endif