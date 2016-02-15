/*!
@file
Defines `clbl::internal::casted_function_object_wrapper`.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
    
#ifndef CLBL_CASTED_FUNCTION_OBJECT_WRAPPER_H
#define CLBL_CASTED_FUNCTION_OBJECT_WRAPPER_H

#include <CLBL/pmf.h>
#include <CLBL/constraints.h>

namespace clbl { namespace internal {

/*!
Wraps an ambiguous callable object, but uses a static_cast on
operator() to disambiguate it.
*/

template<typename Creator, qualify_flags QFlags, typename T, typename Pmf>
struct casted_function_object_wrapper {

    using mf = pmf<Pmf>;

    template<qualify_flags Flags>
    struct pmf_cast {
        static constexpr const auto value = 
            static_cast<
                typename mf::template apply_qualifiers<QFlags | Flags>
            >(&T::operator());
    };

    using return_type = typename mf::return_type;
    using class_type = typename mf::class_type;
    using arg_types = typename mf::template unpack_args<std::tuple>;
    using clbl_tag = pmf_tag;
    using creator = Creator;
    using forwarding_glue = typename mf::forwarding_glue;

    struct invocation_data_type {
        static constexpr const auto pmf = pmf_cast<qflags::default_>::value;
        qualified_type<T, QFlags> object;
    };

    using this_t = casted_function_object_wrapper<
            Creator, QFlags, T, Pmf>;

    
    using type = typename mf::template unpack_args_to_function<return_type>;
    using underlying_type = qualified_type<T, QFlags>;

    template<qualify_flags Flags>
    using add_qualifiers = casted_function_object_wrapper<Creator,
        QFlags | Flags, T, Pmf>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(data.object)
            .*pmf_cast<qflags::default_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(data.object)
            .*pmf_cast<qflags::const_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(data.object)
            .*pmf_cast<qflags::volatile_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const volatile {
        return (
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(
                data.object
            ).*pmf_cast<qflags::const_ | qflags::volatile_>::value
        )(static_cast<Fargs&&>(a)...);
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(c.data.object)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(data.object)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(c.data.object)
        );
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{{c.data.object}};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{{c.data.object}};
    }

    template<
        typename U = underlying_type,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{{c.data.object}};
    }
};

}}

#endif
