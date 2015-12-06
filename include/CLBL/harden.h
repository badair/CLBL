#ifndef CLBL_HARDEN_H
#define CLBL_HARDEN_H

#include <functional>
#include <boost/hana.hpp>

#include "CLBL/tags.h"
#include "CLBL/forwardable.h"

namespace hana = boost::hana;

namespace clbl {

    namespace detail {
        template<typename Bad>
        struct harden_t {
            static_assert(sizeof(Bad) < 0, "Not a valid function type.");
        };

#define __CLBL_SPECIALIZE_HARDEN_T(qualifiers) \
        template<typename Return, typename... Args> \
        struct harden_t<Return(Args...) qualifiers> { \
            template<typename Callable> \
            inline constexpr auto operator()(Callable&& c) const { \
                return func(std::forward<Callable>(c), \
                static_cast<Return(std::remove_reference_t<Callable>::*)(forwardable<Args>...) qualifiers>(&std::remove_reference_t<Callable>::operator())); \
            } \
            template<typename Callable> \
            inline constexpr auto operator()(std::reference_wrapper<Callable> c) const { \
                return func(c, static_cast<Return(Callable::*)(forwardable<Args>...) qualifiers>(&Callable::operator())); \
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
                    return std::move(func(std::forward<Callable>(c), \
                        static_cast<ret(std::remove_reference_t<Callable>::*)(forwardable<Args>...) qualifiers>( \
                            &std::remove_reference_t<Callable>::operator())) \
                        ); \
                } \
                template<typename Callable> \
                static inline constexpr auto fn(std::reference_wrapper<Callable> c) { \
                    return std::move(func(c, \
                        static_cast<Callable::return_t(Callable::*)(forwardable<Args>...) qualifiers>(&Callable::operator()))); \
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
                    static_assert(!std::is_same<typename std::remove_reference_t<Callable>::return_t, ambiguous_return>::value, \
                        "Unable to disambiguate. Please specify a function type with clbl::harden<Return(Arg1, Arg2, ...)>() "); \
                    return fn(std::forward<Callable>(c)); \
                } \
                template<typename Callable> \
                static inline constexpr auto default_harden(std::reference_wrapper<Callable> c) -> std::enable_if_t< \
                    expr, \
                    decltype(fn(c))> { \
                    static_assert(!std::is_same<Callable::return_t, ambiguous_return>::value, \
                        "Unable to disambiguate. Please specify a function type with clbl::harden<Return(Arg1, Arg2, ...)>() "); \
                    return fn(c); \
                }

                    __CLBL_SPECIALIZE_DEFAULT_HARDEN(std::remove_reference_t<Callable>::clbl_is_const && std::remove_reference_t<Callable>::clbl_is_volatile, harden_cv)
                    __CLBL_SPECIALIZE_DEFAULT_HARDEN(std::remove_reference_t<Callable>::clbl_is_const && !std::remove_reference_t<Callable>::clbl_is_volatile, harden_c)
                    __CLBL_SPECIALIZE_DEFAULT_HARDEN(!std::remove_reference_t<Callable>::clbl_is_const && std::remove_reference_t<Callable>::clbl_is_volatile, harden_v)
                    __CLBL_SPECIALIZE_DEFAULT_HARDEN(!std::remove_reference_t<Callable>::clbl_is_const && !std::remove_reference_t<Callable>::clbl_is_volatile, harden_no_cv)
                    //todo ellipses, ref qualifiers


            };

            template<typename Callable>
            inline constexpr auto operator()(Callable&& c) const {
                return unpack_args<std::remove_reference_t<Callable>::args_t>::default_harden(std::forward<Callable>(c));
            }

            template<typename Callable>
            inline constexpr auto operator()(std::reference_wrapper<Callable> c) const {
                return unpack_args<Callable::args_t>::default_harden(c);
            }
        };

        template<typename T>
        constexpr harden_t<T> harden_v{};
    }

    template<typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<detail::default_harden_tag>(std::forward<Callable>(std::forward<Callable>(c)));
    }

    template<typename FunctionType, typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<FunctionType>(std::forward<Callable>(std::forward<Callable>(c)));
    }
}

#endif