/*
@file
Defines `clbl::casted_function_object_ptr_wrapper`.

Copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_CASTED_FUNCTION_OBJECT_PTR_WRAPPER_H
#define CLBL_CASTED_FUNCTION_OBJECT_PTR_WRAPPER_H

#include <CLBL/constraints.hpp>

namespace clbl { namespace internal {

/*!
Wraps a pointer to an ambiguous callable object, but uses a static_cast
on operator() to disambiguate it.
*/
template<typename Creator,
        qualify_flags QFlags,
        typename UnderlyingType,
        typename FunctionObjectPtr, 
        typename Pmf>
struct casted_function_object_ptr_wrapper {

    using mf = pmf<Pmf>;

    template<qualify_flags Flags>
    struct pmf_cast {
        static constexpr const auto value = 
            static_cast<
                typename mf::template apply_qualifiers<QFlags | Flags>
            >(&UnderlyingType::operator());
    };

    using arg_types = typename mf::template unpack_args<std::tuple>;
    using clbl_tag = pmf_ptr_tag;
    using creator = Creator;
    using return_type = typename mf::return_type;
    using forwarding_glue = typename mf::forwarding_glue;

    struct invocation_data_type {
        static constexpr const auto pmf = pmf_cast<qflags::default_>::value;
        FunctionObjectPtr object_ptr;
    };

    using this_t = casted_function_object_ptr_wrapper<
                        Creator,
                        QFlags,
                        UnderlyingType,
                        FunctionObjectPtr,
                        Pmf>;

    using type = typename mf::decay_to_function;
    using underlying_type = UnderlyingType;

    template<qualify_flags Flags>
    using add_qualifiers = casted_function_object_ptr_wrapper<
                                Creator,
                                QFlags | Flags,
                                UnderlyingType,
                                FunctionObjectPtr,
                                Pmf>;

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = false;

    invocation_data_type data;

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) {
        return (harden_cast<q_flags>(*data.object_ptr)
                .*pmf_cast<qflags::default_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const {
        return (harden_cast<qflags::const_ | q_flags>(*data.object_ptr)
                .*pmf_cast<qflags::const_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline CLBL_CXX14_CONSTEXPR return_type
    operator()(Fargs&&... a) volatile {
        return (harden_cast<qflags::volatile_ | q_flags>(*data.object_ptr)
                .*pmf_cast<qflags::volatile_>::value)(static_cast<Fargs&&>(a)...);
    }

    template<typename... Fargs>
    inline constexpr return_type
    operator()(Fargs&&... a) const volatile {
        return (harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(*data.object_ptr)
                .*pmf_cast<qflags::const_ | qflags::volatile_>::value)(
                    static_cast<Fargs&&>(a)...
                );
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(U& c) {
        return no_ref<decltype(*c.dat.object_ptra)>::copy_invocation(
            harden_cast<q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::volatile_ | q_flags>(*c.data.object_ptr)
        );
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile U& c) {
        return no_ref<decltype(*c.data.object_ptr)>::copy_invocation(
            harden_cast<qflags::const_ | qflags::volatile_ | q_flags>(
                *c.data.object_ptr
            )
        );
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(this_t& c) {
        return c;
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const this_t& c) {
        return add_qualifiers<qflags::const_>{{c.data.object_ptr}};
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(volatile this_t& c) {
        return add_qualifiers<qflags::volatile_>{{c.data.object_ptr}};
    }

    template<
        typename U = UnderlyingType,
        CLBL_REQUIRES_(!is_clbl<U>::value)
    >
    static inline constexpr auto
    copy_invocation(const volatile this_t& c) {
        return add_qualifiers<qflags::const_ | qflags::volatile_>{{c.data.object_ptr}};
    }

    template<typename... Args>
    decltype(auto) bind(Args... args) {
        using list_type = typename detail::wrapped_args_tuple<no_ref<Args>...>::type;
        return detail::binding_wrapper<this_t, list_type>{*this, list_type{ args... }};
    }
};

}}

#endif
