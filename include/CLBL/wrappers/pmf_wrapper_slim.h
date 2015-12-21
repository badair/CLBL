#ifndef CLBL_PMF_WRAPPER_SLIM_H
#define CLBL_PMF_WRAPPER_SLIM_H

#include <type_traits>
#include <tuple>

#include <CLBL/cv.h>
#include <CLBL/utility.h>
#include <CLBL/forward.h>
#include <CLBL/forwardable.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    /*
    pmf_wrapper_slim wraps a PMF and an object with which to call it.
    It is identical to pmf_wrapper except that the PMF is not stored
    in an object instance, but passed as a template arg instead
    */

    template<typename, qualify_flags, typename, typename TMemberFnPtr, TMemberFnPtr, typename Failure>
    struct pmf_wrapper_slim { static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename T, typename TMemberFnPtr, TMemberFnPtr Pmf, typename Return, typename... Args>
    struct pmf_wrapper_slim<Creator, CvFlags, T, TMemberFnPtr, Pmf, Return(std::remove_cv_t<T>::*)(Args...)> {

        using decayed_member_fn_ptr = Return(std::remove_cv_t<T>::*)(Args...);

        using args_t = std::tuple<Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = pmf_invocation_data_slim<apply_qualifiers<T, CvFlags>, TMemberFnPtr, Pmf>;
        using my_type = pmf_wrapper_slim<Creator, CvFlags, T, TMemberFnPtr, Pmf, decayed_member_fn_ptr>;
        using return_t = Return;
        using type = Return(Args...);
        using underlying_type = apply_qualifiers<T, CvFlags>;

        template<qualify_flags Flags>
        using apply_cv = pmf_wrapper_slim<Creator, CvFlags | Flags, T, TMemberFnPtr, Pmf, decayed_member_fn_ptr>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = std::is_same<Return(Args...), ambiguous_return(ambiguous_args)>::value;

        invocation_data_type data;

        inline pmf_wrapper_slim(T&& o)
            : data{ std::forward<T>(o) }
        {}

        inline pmf_wrapper_slim(T& o)
            : data{ o }
        {}

        inline pmf_wrapper_slim(my_type& other) = default;
        inline pmf_wrapper_slim(const my_type& other) = default;
        inline pmf_wrapper_slim(my_type&& other) = default;

        inline pmf_wrapper_slim(volatile my_type& other)
            : data(other.data)
        {}

        inline pmf_wrapper_slim(const volatile my_type& other)
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

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<qflags::const_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(volatile T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<qflags::volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const volatile T& c) {
            return no_ref<decltype(data.object)>::copy_invocation(
                harden_cast<qflags::const_ | qflags::volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                    d.object, decltype(d)::pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const,
                    d.object, decltype(d)::pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(volatile my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile,
                    d.object, decltype(d)::pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const volatile my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile,
                    d.object, decltype(d)::pmf, args...);
            };
        }
    };
}

#endif