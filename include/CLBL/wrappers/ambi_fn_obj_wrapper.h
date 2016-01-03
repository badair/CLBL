/*!
@file
Defines `clbl::ambi_fn_obj_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_AMBI_FN_OBJ_WRAPPER_H
#define CLBL_AMBI_FN_OBJ_WRAPPER_H

#include <type_traits>
#include <utility>

#include <CLBL/tags.h>
#include <CLBL/no_ref.h>
#include <CLBL/cv.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    /*!
    Wraps a callable object whose operator() is overloaded/templated.
    You may still call this wrapper as you would call the original object,
    but you must disambiguate by calling clbl::harden before using any of
    the additional features in CLBL.
    */
    template<typename Creator, qualify_flags CvFlags, typename T>
    struct ambi_fn_obj_wrapper {

        using arg_types = ambiguous_args;
        using clbl_tag = ambi_fn_obj_tag;
        using creator = Creator;
        using forwarding_glue = ambiguous_return(ambiguous_args);
        using invocation_data_type = object_invocation_data<T>;
        using my_type = ambi_fn_obj_wrapper<Creator, CvFlags, T>;
        using return_type = ambiguous_return;
        using type = ambiguous_return(ambiguous_args);
        using underlying_type = T;

        template<qualify_flags Flags>
        using apply_cv = ambi_fn_obj_wrapper<Creator, CvFlags | Flags, T>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = true;

        invocation_data_type data;

        inline ambi_fn_obj_wrapper(const std::remove_const_t<T>& o)
            : data{ o }
        {}

        inline ambi_fn_obj_wrapper(std::remove_const_t<T>&& o)
            : data{ o }
        {}

        inline ambi_fn_obj_wrapper(my_type& other) = default;

        inline ambi_fn_obj_wrapper(const my_type& other) = default;

        inline ambi_fn_obj_wrapper(volatile my_type& other)
            : data{ other.data }
        {}

        inline ambi_fn_obj_wrapper(const volatile my_type& other)
            : data{ other.data }
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_VAL(__CLBL_NO_CV, data.object, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_VAL(const, data.object, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_VAL(volatile, data.object, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_VAL(const volatile, data.object, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return [v = c.data.object](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_VAL(__CLBL_NO_CV, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return [v = c.data.object](auto&&... args){
                return CLBL_UPCAST_AND_CALL_VAL(const, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return [v = c.data.object](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_VAL(volatile, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return [v = c.data.object](auto&&... args){
                return CLBL_UPCAST_AND_CALL_VAL(const volatile, v, args...);
            };
        }
    };
}

#endif