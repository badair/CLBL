/*!
@file
Defines `clbl::casted_fn_obj_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CASTED_FN_OBJ_WRAPPER_H
#define CLBL_CASTED_FN_OBJ_WRAPPER_H

#include <type_traits>
#include <tuple>
#include <utility>

#include <CLBL/cv.h>
#include <CLBL/tags.h>
#include <CLBL/forward.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    /*
    casted_fn_obj_wrapper wraps an ambiguous callable object, but uses a
    static_cast on operator() to disambiguate it.
    */

    template<typename, qualify_flags, typename, typename, typename Failure>
    struct casted_fn_obj_wrapper { static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename T, 
        typename TMemberFnPtr, typename Return, typename... Args>
    struct casted_fn_obj_wrapper<Creator, CvFlags, T, 
        TMemberFnPtr, Return(std::remove_cv_t<T>::*)(Args...)> {

        using decayed_member_fn_ptr = Return(std::remove_cv_t<T>::*)(Args...);

        using arg_types = std::tuple<Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = object_casted_invocation_data<apply_qualifiers<T, CvFlags>, TMemberFnPtr>;
        using my_type = casted_fn_obj_wrapper<Creator, CvFlags, T, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(Args...);
        using underlying_type = apply_qualifiers<T, CvFlags>;

        template<qualify_flags Flags>
        using apply_cv = casted_fn_obj_wrapper<Creator, CvFlags | Flags, T, TMemberFnPtr, decayed_member_fn_ptr>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = std::is_same<Return(Args...), ambiguous_return(ambiguous_args)>::value;

        invocation_data_type data;

        inline casted_fn_obj_wrapper(T&& o)
            : data{ std::forward<T>(o) }
        {}

        inline casted_fn_obj_wrapper(T& o)
            : data{ o }
        {}

        inline casted_fn_obj_wrapper(my_type& other) = default;
        inline casted_fn_obj_wrapper(const my_type& other) = default;
        inline casted_fn_obj_wrapper(my_type&& other) = default;

        inline casted_fn_obj_wrapper(volatile my_type& other)
            : data(other.data)
        {}

        inline casted_fn_obj_wrapper(const volatile my_type& other)
            : data(other.data)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV, 
                data.object, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const,
                data.object, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile,
                data.object, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile,
                data.object, invocation_data_type::pmf, std::forward<Fargs>(a)...);
        }

        template<typename U = underlying_type, std::enable_if_t<is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(U& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<cv_flags>(c.data.object)
            );
        }

        template<typename U = underlying_type, std::enable_if_t<is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const U& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<const_ | cv_flags>(c.data.object)
            );
        }

        template<typename U = underlying_type, std::enable_if_t<is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile U& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename U = underlying_type, std::enable_if_t<is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile U& c) {
            return no_ref<decltype(data.object)>::copy_invocation(
                harden_cast<const_ | volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename U = underlying_type, std::enable_if_t<!is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                    d.object, decltype(d)::pmf, args...
                );
            };
        }

        template<typename U = underlying_type, std::enable_if_t<!is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const,
                    d.object, decltype(d)::pmf, args...);
            };
        }

        template<typename U = underlying_type, std::enable_if_t<!is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile,
                    d.object, decltype(d)::pmf, args...);
            };
        }

        template<typename U = underlying_type, std::enable_if_t<!is_clbl<U>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile,
                    d.object, decltype(d)::pmf, args...);
            };
        }
    };
}

#endif