#ifndef CLBL_UTILITY_H
#define CLBL_UTILITY_H

#include "CLBL/tags.h"

#define CLBL_CALL_PTR(cv_ignored, ptr, args) (*ptr)(args)

/*#define CLBL_UPCAST_AND_CALL_PTR(cv_qual, ptr, args) (static_cast<cv_qual decltype(*ptr)>(*ptr))(args)
#define CLBL_UPCAST_AND_CALL_VAL(cv_qual, val, args) (static_cast<cv_qual decltype(val)>(val))(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_PTR(cv_qual, obj, member, args) ((static_cast<cv_qual decltype(*obj)>(*obj)).*member)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_VAL(cv_qual, obj, member, args) ((static_cast<cv_qual decltype(obj)>(obj)).*member)(args)*/

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

        auto can_dereference_impl = hana::is_valid([](auto arg)->decltype(*arg) {});

        static auto already_has_cv_flags_t = hana::is_valid([](auto c) -> decltype(decltype(c)::cv_flags) {});

        auto has_creator_t = hana::is_valid([](auto arg)-> decltype(arg)::creator{});
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

    namespace detail {

        namespace cv_flags_detail {
            struct dummy_clbl_type {
                static constexpr qualify_flags cv_flags = qflags::default_;
            };
        }

        template<typename T>
        inline constexpr qualify_flags cv_flags() {
            constexpr auto can_deref = can_dereference<T>;
            using derferenceable = std::conditional_t<can_deref, T, int*>;
            using cv_type = no_ref<std::conditional_t<can_deref, decltype(*std::declval<derferenceable>()), T> >;
            constexpr auto constness = std::is_const<cv_type>::value ? qflags::const_ : qflags::default_;
            constexpr auto volatileness = std::is_volatile<cv_type>::value ? qflags::volatile_ : qflags::default_;
            using clbl_type = std::conditional_t<already_has_cv_flags<cv_type>, cv_type, cv_flags_detail::dummy_clbl_type>;
            return constness | volatileness | clbl_type::cv_flags;
        }
    }

    template<typename T>
    constexpr qualify_flags cv = detail::cv_flags<T>();

    namespace detail {
        template<typename T>
        struct creator_of_t {
            using type = unknown_tag;
        };

        template<typename T>
        struct creator_of_t<T::creator> {
            using type = typename T::creator;
        };
    }

    template<typename T>
    using creator_of = typename detail::creator_of_t<no_ref<T>>::type;

    template<qualify_flags CvFlags>
    struct qualifiers {
        static_assert(CvFlags < 0, "Unknown cv-ness");
    };

    template<>
    struct qualifiers<qflags::default_> {
        template<typename T>
        using apply = T;
    };

    template<>
    struct qualifiers<qflags::const_> {
        template<typename T>
        using apply = const T;
    };

    template<>
    struct qualifiers<qflags::volatile_> {
        template<typename T>
        using apply = volatile T;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_> {
        template<typename T>
        using apply = const volatile T;
    };

    template<>
    struct qualifiers<qflags::default_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = T&;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::lvalue_reference_>{
        template<typename T>
        using apply = const T&;
    };

    template<>
    struct qualifiers<qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = volatile T&;
    };

    template<>
    struct qualifiers<qflags::const_ | qflags::volatile_ | qflags::lvalue_reference_> {
        template<typename T>
        using apply = const volatile T&;
    };
    
    using no_q = qualifiers<qflags::default_>;
    using const_q = qualifiers<qflags::const_>;
    using volatile_q = qualifiers<qflags::volatile_>;
    using const_volatile_q = qualifiers<qflags::const_ | qflags::volatile_>;

    using ref_q = qualifiers<qflags::default_ | qflags::lvalue_reference_>;
    using const_ref_q = qualifiers<qflags::const_ | qflags::lvalue_reference_>;
    using volatile_ref_q = qualifiers<qflags::volatile_ | qflags::lvalue_reference_>;
    using const_volatile_ref_q = qualifiers<qflags::volatile_ | qflags::lvalue_reference_>;

    namespace apply_qualifiers_detail {

        struct dummy_clbl_type {
            template<qualify_flags Flags>
            using apply_cv = std::nullptr_t;
        };

        template<typename U, qualify_flags Flags>
        struct apply_qualifiers_t {
            using T = no_ref<U>;
            //using clbl_type = std::conditional_t<is_clbl<T>, T, dummy_clbl_type>;
            //using type = std::conditional_t<is_clbl<T>, typename clbl_type::apply_cv<Flags>, typename qualifiers<Flags>::apply<T> >;
            using type = typename qualifiers<Flags>::apply<T>;
        };
    }

    template<typename T, qualify_flags Flags>
    using apply_qualifiers = typename apply_qualifiers_detail::apply_qualifiers_t<T, Flags>::type;

    template<qualify_flags CvFlags, typename Object>
    constexpr auto harden_cast(Object&& o) -> apply_qualifiers<Object, CvFlags | cv<Object> | qflags::lvalue_reference_> {
        return std::forward<Object>(o);
    }

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