//welcome to the party

#ifndef CLBL_HARDEN_H
#define CLBL_HARDEN_H

#include <functional>

#include <boost/hana.hpp>

#include "CLBL/cv_checks.h"
#include "CLBL/tags.h"
#include "CLBL/forwardable.h"
#include "CLBL/fwrap.h"
#include "CLBL/wrappers/pmf_ptr_wrapper.h"
#include "CLBL/utility.h"

namespace hana = boost::hana;

namespace clbl {

    namespace detail {

        template<typename TMemberFnPtr, typename Callable, typename Creator>
        struct disambiguate : Creator {};

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename pointer_to_function_object::ambiguous> {
            template<qualify_flags Flags, typename T, typename Dummy>
            static inline constexpr auto
            wrap(T&& t, Dummy&&) {
                using object_type = no_ref<decltype(*std::declval<T>())>;
                constexpr auto member_fn = static_cast<TMemberFnPtr>(&object_type::operator());
                return member_function_with_pointer_to_object::template wrap<Flags>(member_fn, t);
            }
        };

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, typename function_object::ambiguous> {
            template<qualify_flags Flags, typename T, typename Dummy>
            static inline constexpr auto
            wrap(T&& t, Dummy&&) {
                constexpr auto member_fn = static_cast<TMemberFnPtr>(&no_ref<T>::operator());
                return member_function_with_object::template wrap<Flags>(member_fn, std::forward<T>(t));
            }
        };

        template<typename TMemberFnPtr, typename C>
        struct disambiguate<TMemberFnPtr, C, std::enable_if_t<C::creator::has_member_function_pointer, typename C::creator> > {
            template<qualify_flags Flags, typename T, typename TExistingMemberFnPtr>
            static inline constexpr auto
            wrap(TExistingMemberFnPtr member_fn, T&& t) {
                return C::creator::template wrap<Flags>(member_fn, std::forward<T>(t));
            }
        };

        template<typename Bad>
        struct harden_t {
            static_assert(sizeof(Bad) < 0, "Not a valid function type.");
        };

#define __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, cv_present) \
        template<typename Callable> \
        inline constexpr auto operator()(cv_present Callable& c) const { \
            constexpr qualify_flags requested = cv<cv_requested dummy>; \
            constexpr qualify_flags present = cv<cv_present dummy>; \
            using C = no_ref<Callable>; \
            using underlying_type = typename C::underlying_type; \
            using abominable_fn_type = std::conditional_t<is_clbl<underlying_type>, \
                                                            Return(forwardable<Args>...) cv_requested, \
                                                            Return(Args...) cv_requested>; \
            using requested_pmf_type = abominable_fn_type underlying_type::*; \
            using disambiguator = disambiguate<requested_pmf_type, C, typename C::creator>; \
            return disambiguator::template wrap<requested | present>(c._value, c._object); \
        }
            
#define __CLBL_SPECIALIZE_HARDEN_T(cv_requested) \
        template<typename Return, typename... Args> \
            struct harden_t<Return(Args...) cv_requested> { \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, CLBL_NOTHING) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, const) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, volatile) \
            __CLBL_DEFINE_HARDEN_T_OVERLOADS(cv_requested, const volatile) \
        }

        //todo ellipses and ref qualifiers... :(

        __CLBL_SPECIALIZE_HARDEN_T(CLBL_NOTHING);
        __CLBL_SPECIALIZE_HARDEN_T(const);
        __CLBL_SPECIALIZE_HARDEN_T(volatile);
        __CLBL_SPECIALIZE_HARDEN_T(const volatile);

       
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
                    using ret = typename no_ref<Callable>::return_t; \
                    return std::move(fwrap(std::forward<Callable>(c), \
                        static_cast<ret(no_ref<Callable>::*)(forwardable<Args>...) qualifiers>( \
                            &no_ref<Callable>::operator())) \
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
                    static_assert(!std::is_same<typename no_ref<Callable>::args_t, ambiguous_args>::value, \
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

                __CLBL_SPECIALIZE_DEFAULT_HARDEN(no_ref<Callable>::clbl_is_deep_const && no_ref<Callable>::clbl_is_deep_volatile, harden_cv)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(no_ref<Callable>::clbl_is_deep_const && !no_ref<Callable>::clbl_is_deep_volatile, harden_c)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(!no_ref<Callable>::clbl_is_deep_const && no_ref<Callable>::clbl_is_deep_volatile, harden_v)
                __CLBL_SPECIALIZE_DEFAULT_HARDEN(!no_ref<Callable>::clbl_is_deep_const && !no_ref<Callable>::clbl_is_deep_volatile, harden_no_cv)
                //todo ellipses, ref qualifiers
            };

            template<typename Callable>
            inline constexpr auto operator()(Callable&& c) const {
                return unpack_args<typename no_ref<Callable>::args_t>::default_harden(std::forward<Callable>(c));
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
        return detail::harden_v<default_harden_tag>(std::forward<Callable>(c));
    }

    template<typename FunctionType, typename Callable>
    inline constexpr auto harden(Callable&& c) {
        return detail::harden_v<FunctionType>(std::forward<Callable>(c));
    }
}

#endif