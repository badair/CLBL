#ifndef CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H
#define CLBL_OVERLOADED_FUNCTION_OBJECT_PTR_H

#include "CLBL/tags.h"
#include "CLBL/cv_checks.h"

#include <type_traits>

namespace clbl {

    template<typename UnderlyingType, typename TPtr>
    struct overloaded_function_object_ptr
        //CRTP to callable
        : callable<overloaded_function_object_ptr<UnderlyingType, TPtr>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object_ptr(TPtr&& o_ptr)
            : ptr(std::forward<TPtr>(o_ptr))
        {}

        using my_type = overloaded_function_object_ptr<UnderlyingType, TPtr>;
        overloaded_function_object_ptr(const my_type&) = default;
        overloaded_function_object_ptr(my_type&) = default;
        overloaded_function_object_ptr(my_type&&) = default;

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return (static_cast<std::remove_cv_t<std::remove_reference_t<decltype(*ptr)> > &>(*ptr))(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return (static_cast<const std::remove_cv_t<std::remove_reference_t<decltype(*ptr)> > &>(*ptr))(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return (static_cast<volatile std::remove_cv_t<std::remove_reference_t<decltype(*ptr)> > &>(*ptr))(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return (static_cast<const volatile std::remove_cv_t<std::remove_reference_t<decltype(*ptr)> > &>(*ptr))(std::forward<Fargs>(a)...);
        }

        static constexpr auto clbl_is_const = is_clbl_const<std::remove_reference_t<decltype(*std::declval<TPtr>())> >()
                                                    || std::is_const<std::remove_reference_t<decltype(*std::declval<TPtr>())> >::value;


        static constexpr auto clbl_is_volatile = is_clbl_volatile<std::remove_reference_t<decltype(*std::declval<TPtr>())> >()
                                                    || std::is_volatile<std::remove_reference_t<decltype(*std::declval<TPtr>())> >::value;

    private:
        TPtr ptr;


    };
}

#endif