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

    /*
    clbl::no_ref lets us use std::remove_reference_t
    with less screen clutter, because we use it EVERYWHERE
    */
    template<typename T>
    using no_ref = std::remove_reference_t<T>;

    /*
    clbl::args is a metafunction to extract the args_t
    alias of a CLBL wrapper
    */
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

    /*
    clbl::invocation_copy get's the return type of a CLBL wrapper's
    copy_invocation function
    */
    template<typename Callable>
    using invocation_copy = decltype(Callable::copy_invocation(std::declval<std::add_lvalue_reference_t<Callable> >()));

    /*
    clbl::can_dereference is a type trait that we use to determine whether
    a type is a pointer by checking whether we can dereference it.
    This allows us to treat smart pointers and raw pointers with the same code.
    */
    template<typename T>
    constexpr bool can_dereference = decltype(detail::can_dereference_impl(std::declval<T>()))::value;

    template<typename T>
    constexpr auto has_creator = decltype(detail::has_creator_t(std::declval<T>()))::value;

    template<typename T>
    static constexpr auto already_has_cv_flags = decltype(detail::already_has_cv_flags_t(std::declval<T>()))::value;

    /*
    clbl::is_clbl is a type trait to determine whether a type
    is a CLBL wrapper. Currently, this checks for the cv_flags
    member, which isn't a very good idea, and will be changed
    in the future (TODO)
    */
    template<typename T>
    static constexpr auto is_clbl = already_has_cv_flags<T>;

    namespace underlying_type_detail {
        template<typename T>
        struct identity {
            using underlying_type = T;
        };
    }

    /*
    underlying_type is used to extract the underlying object type
    of recursive CLBL wrappers
    */

    template<typename T>
    using underlying_type = typename std::conditional_t<
                                is_clbl<T>, T, underlying_type_detail::identity<T>
                            >::underlying_type;
}


#endif