#ifndef CLBL_UTILITY_H
#define CLBL_UTILITY_H

#include "CLBL/tags.h"
#include "CLBL/qualify_flags.h"
#include "CLBL/is_valid.h"

#define CLBL_CALL_PTR(cv_ignored, ptr, args) (*ptr)(args)
#define CLBL_UPCAST_AND_CALL_PTR(qual, ptr, args) harden_cast<cv<qual dummy> | cv_flags>(*ptr)(args)
#define CLBL_UPCAST_AND_CALL_VAL(qual, val, args) harden_cast<cv<qual dummy> | cv_flags>(val)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_PTR(qual, optr, member, args)(harden_cast<cv<qual dummy> | cv_flags>(*optr).*member)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_VAL(qual, obj, member, args) (harden_cast<cv<qual dummy> | cv_flags>(obj).*member)(args)

namespace clbl {

    template<typename T>
    using no_ref = std::remove_reference_t<T>;

    template<typename Callable>
    using args = typename no_ref<Callable>::args_t;

    namespace detail {

        template<typename T>
        struct is_reference_wrapper_t : std::false_type {};

        template <typename T>
        struct is_reference_wrapper_t<std::reference_wrapper<T> > : std::true_type {};

        auto can_dereference_impl = is_valid([](auto arg)->decltype(*arg) {});

        static auto already_has_cv_flags_t = is_valid([](auto c) -> decltype(decltype(c)::cv_flags) {});

        auto has_creator_t = is_valid([](auto arg)-> typename decltype(arg)::creator{});
    }

    template<typename T>
    constexpr bool is_reference_wrapper = detail::is_reference_wrapper_t<T>::value;

    template<typename Callable>
    using invocation_copy = decltype(Callable::copy_invocation(std::declval<std::add_lvalue_reference_t<Callable> >()));

    template<typename T>
    constexpr bool can_dereference = decltype(detail::can_dereference_impl(std::declval<T>()))::value;

    template<typename T>
    constexpr auto has_creator = decltype(detail::has_creator_t(std::declval<T>()))::value;

    template<typename T>
    static constexpr auto already_has_cv_flags = decltype(detail::already_has_cv_flags_t(std::declval<T>()))::value;

    template<typename T>
    static constexpr auto is_clbl = already_has_cv_flags<T>;

    namespace underlying_type_detail {
        template<typename T>
        struct identity {
            using underlying_type = T;
        };
    }

    template<typename T>
    using underlying_type = typename std::conditional_t<is_clbl<T>, T, underlying_type_detail::identity<T> >::underlying_type;
}


#endif