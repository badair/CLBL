#ifndef CLBL_HARDEN_H
#define CLBL_HARDEN_H

#include <functional>
#include <boost/hana.hpp>

#include "CLBL/cv_checks.h"
#include "CLBL/tags.h"
#include "CLBL/forwardable.h"
#include "CLBL/fwrap.h"
#include "CLBL/member_function_of_ptr.h"

namespace hana = boost::hana;

namespace clbl {

    namespace detail {
        template<typename Bad>
        struct harden_t {
            static_assert(sizeof(Bad) < 0, "Not a valid function type.");
        };

        //todo ref wrapper harden, other clbl types

#define __CLBL_SPECIALIZE_HARDEN_T(qualifiers) \
        template<typename Return, typename... Args> \
        struct harden_t<Return(Args...) qualifiers> { \
            template<typename Ut, typename P, typename Pmf, typename Ret, typename... Args> \
            inline constexpr auto operator()(member_function_of_ptr<Ut, P, Pmf, Ret(Ut::*)(Args...)>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<Ut>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using qualified_object_type = qualifiers std::remove_cv_t<Ut>; \
                using requested_pmf_type = abominable_fn_type qualified_object_type::*; \
                return fwrap<qualified_object_type, requested_pmf_type>( \
                    c.object_ptr, \
                    static_cast<requested_pmf_type>(c.value)); \
            } \
            template<typename Ut, typename P, typename Pmf, typename Ret, typename... Args> \
            inline constexpr auto operator()(const member_function_of_ptr<Ut, P, Pmf, Ret(Ut::*)(Args...)>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<Ut>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using qualified_object_type = qualifiers std::remove_cv_t<Ut>; \
                using requested_pmf_type = abominable_fn_type qualified_object_type::*; \
                return fwrap<qualified_object_type, requested_pmf_type>( \
                    c.object_ptr, \
                    static_cast<requested_pmf_type>(c.value)); \
            } \
            template<typename Ut, typename P, typename Pmf, typename Ret, typename... Args> \
            inline constexpr auto operator()(volatile member_function_of_ptr<Ut, P, Pmf, Ret(Ut::*)(Args...)>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<Ut>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using qualified_object_type = qualifiers std::remove_cv_t<Ut>; \
                using requested_pmf_type = abominable_fn_type qualified_object_type::*; \
                return fwrap<qualified_object_type, requested_pmf_type>( \
                    c.object_ptr, \
                    static_cast<requested_pmf_type>(c.value)); \
            } \
            template<typename Ut, typename P, typename Pmf, typename Ret, typename... Args> \
            inline constexpr auto operator()(const volatile member_function_of_ptr<Ut, P, Pmf, Ret(Ut::*)(Args...)>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<Ut>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using qualified_object_type = qualifiers std::remove_cv_t<Ut>; \
                using requested_pmf_type = abominable_fn_type qualified_object_type::*; \
                return fwrap<qualified_object_type, requested_pmf_type>( \
                    c.object_ptr, \
                    static_cast<requested_pmf_type>(c.value)); \
            } \
            template<typename T, typename TPtr> \
            inline constexpr auto operator()(overloaded_function_object_ptr<T, TPtr>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<T>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using requested_pmf_type = abominable_fn_type T::*; \
                return fwrap<TPtr&, requested_pmf_type>( \
                    c.value, \
                    static_cast<requested_pmf_type>(&T::operator())); \
            } \
            template<typename T, typename TPtr> \
            inline constexpr auto operator()(const overloaded_function_object_ptr<T, TPtr>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<T>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using requested_pmf_type = abominable_fn_type T::*; \
                return fwrap<const TPtr&, requested_pmf_type>( \
                    c.value, \
                    static_cast<requested_pmf_type>(&T::operator())); \
            } \
            template<typename T, typename TPtr> \
            inline constexpr auto operator()(volatile overloaded_function_object_ptr<T, TPtr>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<T>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using requested_pmf_type = abominable_fn_type T::*; \
                return fwrap<volatile TPtr&, requested_pmf_type>( \
                    c.value, \
                    static_cast<requested_pmf_type>(&T::operator())); \
            } \
            template<typename T, typename TPtr> \
            inline constexpr auto operator()(const volatile overloaded_function_object_ptr<T, TPtr>& c) const { \
                using abominable_fn_type = std::conditional_t<is_clbl<T>(), Return(forwardable<Args>...) qualifiers, Return(Args...) qualifiers>; \
                using requested_pmf_type = abominable_fn_type T::*; \
                return fwrap<const volatile TPtr&, requested_pmf_type>( \
                    c.value, \
                    static_cast<requested_pmf_type>(&T::operator())); \
            } \
        }

        __CLBL_SPECIALIZE_HARDEN_T(CLBL_NOTHING);
        __CLBL_SPECIALIZE_HARDEN_T(const);
        __CLBL_SPECIALIZE_HARDEN_T(volatile);
        __CLBL_SPECIALIZE_HARDEN_T(const volatile);
        //todo ellipses and ref qualifiers

        struct default_harden_tag {};

        template<>
        struct harden_t<default_harden_tag> {

            template<typename T>
            struct unpack_args {

            };

            template<typename... Args>
            struct unpack_args<hana::tuple<Args...> > {

#define __CLBL_DEFINE_QUALIFIED_HARDEN_FUNCTION(fn, qualifiers) \
                template<typename Callable> \
                static inline constexpr auto fn(Callable&& c) { \
                    using ret = typename std::remove_reference_t<Callable>::return_t; \
                    return std::move(fwrap(std::forward<Callable>(c), \
                        static_cast<ret(std::remove_reference_t<Callable>::*)(forwardable<Args>...) qualifiers>( \
                            &std::remove_reference_t<Callable>::operator())) \
                        ); \
                } \
                template<typename Callable> \
                static inline constexpr auto fn(std::reference_wrapper<Callable> c) { \
                    return std::move(fwrap(c, \
                        static_cast<typename Callable::return_t(Callable::*)(forwardable<Args>...) qualifiers>(&Callable::operator()))); \
                }

                __CLBL_DEFINE_QUALIFIED_HARDEN_FUNCTION(harden_no_cv, CLBL_NOTHING)
                __CLBL_DEFINE_QUALIFIED_HARDEN_FUNCTION(harden_c, const)
                __CLBL_DEFINE_QUALIFIED_HARDEN_FUNCTION(harden_v, volatile)
                __CLBL_DEFINE_QUALIFIED_HARDEN_FUNCTION(harden_cv, const volatile)
                //todo ellipses, ref qualifiers

#define __CLBL_SPECIALIZE_DEFAULT_HARDEN(expr, fn) \
                template<typename Callable> \
                static inline constexpr auto default_harden(Callable&& c) -> std::enable_if_t< \
                    expr, \
                    decltype(fn(std::forward<Callable>(c)))> { \
                    static_assert(!std::is_same<typename std::remove_reference_t<Callable>::args_t, ambiguous_args>::value, \
                        "Unable to disambiguate. Please specify a function with clbl::harden<Return(Arg1, Arg2, ...) qualifiers>() "); \
                    return fn(std::forward<Callable>(c)); \
                } \
                template<typename Callable> \
                static inline constexpr auto default_harden(std::reference_wrapper<Callable> c) -> std::enable_if_t< \
                    expr, \
                    decltype(fn(c))> { \
                    static_assert(!std::is_same<typename Callable::args_t, ambiguous_args>::value, \
                        "Unable to disambiguate. Please specify a function with clbl::harden<Return(Arg1, Arg2, ...) qualifiers>() "); \
                    return fn(c); \
                }

                __CLBL_SPECIALIZE_DEFAULT_HARDEN(std::remove_reference_t<Callable>::clbl_is_deep_const && std::remove_reference_t<Callable>::clbl_is_deep_volatile, harden_cv)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(std::remove_reference_t<Callable>::clbl_is_deep_const && !std::remove_reference_t<Callable>::clbl_is_deep_volatile, harden_c)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(!std::remove_reference_t<Callable>::clbl_is_deep_const && std::remove_reference_t<Callable>::clbl_is_deep_volatile, harden_v)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(!std::remove_reference_t<Callable>::clbl_is_deep_const && !std::remove_reference_t<Callable>::clbl_is_deep_volatile, harden_no_cv)
                //todo ellipses, ref qualifiers
            };

            template<typename Callable>
            inline constexpr auto operator()(Callable&& c) const {
                return unpack_args<typename std::remove_reference_t<Callable>::args_t>::default_harden(std::forward<Callable>(c));
            }

            template<typename Callable>
            inline constexpr auto operator()(std::reference_wrapper<Callable> c) const {
                return unpack_args<typename Callable::args_t>::default_harden(c);
            }
        };

        template<typename T>
        constexpr harden_t<T> harden_v{};
    }

    template<typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<detail::default_harden_tag>(std::forward<Callable>(c));
    }

    template<typename FunctionType, typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<FunctionType>(static_cast<Callable&&>(c));
    }
}

#endif