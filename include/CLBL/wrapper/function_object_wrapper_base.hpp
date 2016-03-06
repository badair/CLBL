 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_OBJECT_WRAPPER_BASE_H
#define CLBL_FUNCTION_OBJECT_WRAPPER_BASE_H

#include <CLBL/possibly_ref_qualified_call_operator_type.hpp>
#include <CLBL/can_call_with.hpp>

namespace clbl { namespace internal {

template<
    quali::flags QFlags,
    typename GeneralizedObject,
    typename Pmf = call_operator<default_normal_callable<typename GeneralizedObject::type>>
>
struct function_object_wrapper_base;

template<quali::flags QFlags, typename GeneralizedObject, typename Pmf = dummy>
struct ambiguous_function_object_wrapper_base {

    using pmf_type = Pmf;
    using generalized_object = GeneralizedObject;
    using arg_types = ambiguous_args;
    using return_type = ambiguous_return;
    using forwarding_glue = ambiguous_return(ambiguous_args);
    using type = ambiguous_return(ambiguous_args);
    using invocation_data_type = GeneralizedObject;
    using clbl_tag = function_object_wrapper_tag;

    using underlying_type = typename GeneralizedObject::type; 

    static constexpr auto q_flags = QFlags;
    static constexpr auto is_ambiguous = true;

    template<quali::flags Flags>
    using add_qualifiers = 
    callable_wrapper<
        ambiguous_function_object_wrapper_base<
            quali::collapse<q_flags, Flags>::value,
            GeneralizedObject,
            Pmf
        >
    >;

    invocation_data_type data;

    template<quali::flags Flags, typename... Args>
    using apply_signature = callable_wrapper<
        function_object_wrapper_base<
            quali::collapse<q_flags, Flags>::value,
            GeneralizedObject,
            typename pmf<
                decltype(data.template get<quali::collapse<q_flags, Flags>::value>()
                (std::declval<Args>()...))
                (no_ref<underlying_type>::*)(Args...)
            >::template add_qualifiers<Flags>
        >
    >;

    ambiguous_function_object_wrapper_base() = default;
    ambiguous_function_object_wrapper_base(const ambiguous_function_object_wrapper_base& ) = default;
    ambiguous_function_object_wrapper_base(ambiguous_function_object_wrapper_base&&) = default;

    inline constexpr
    ambiguous_function_object_wrapper_base(unqualified<invocation_data_type>&& t)
        : data{static_cast<unqualified<invocation_data_type>&&>(t)} {}

    inline constexpr
    ambiguous_function_object_wrapper_base(unqualified<invocation_data_type> const & t)
        : data{t} {}

    inline constexpr
    ambiguous_function_object_wrapper_base(unqualified<invocation_data_type> const volatile & t)
        : data{t} {}

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) {
        return data.template get<q_flags>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(Args&&... args) {
        using flags_type = quali::force_rvalue_reference<q_flags>;
        return data.template get<flags_type::value>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return data.template get<quali::const_ | q_flags>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    move_invoke(Args&&... args) const {
        using flags_type = quali::force_rvalue_reference<quali::const_ | q_flags>;
        return data.template get<flags_type::value>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) volatile {
        return data.template get<quali::volatile_ | q_flags>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(Args&&... args) volatile {
        using flags_type = quali::force_rvalue_reference<quali::volatile_ | q_flags>;
        return data.template get<flags_type::value>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return data.template get<quali::const_ | quali::volatile_ | q_flags>()(
            static_cast<Args&&>(args)...
        );
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    move_invoke(Args&&... args) const volatile {
        using flags_type = quali::force_rvalue_reference<
            quali::const_ | quali::volatile_ | q_flags
        >;

        return data.template get<flags_type::value>()(
            static_cast<Args&&>(args)...
        );
    }
};

template<
    quali::flags QFlags,
    typename GeneralizedObject,
    typename Pmf
>
struct function_object_wrapper_base 
    : public pmf<Pmf> {

    using pmf_type = Pmf;
    using generalized_object = GeneralizedObject;
    using general_type = typename GeneralizedObject::type;
    using base = pmf<Pmf>;
    using this_t = function_object_wrapper_base;

    static constexpr const quali::flags q_flags = 
        quali::collapse<base::q_flags, QFlags>::value;

    using underlying_type = quali::qualify<general_type, q_flags>;
    static constexpr const bool is_ambiguous = false;

    template<quali::flags Ignored, CLBL_REQUIRES_(has_normal_call_operator<general_type>::value)>
    static inline constexpr decltype(auto)
    pmf_cast(){
        return &no_ref<underlying_type>::operator();
    }

    template<quali::flags Flags, CLBL_REQUIRES_(!has_normal_call_operator<general_type>::value)>
    static inline constexpr decltype(auto)
    pmf_cast(){
        return static_cast<
            possibly_ref_qualified_call_operator_type<
                base,
                general_type,
                quali::collapse<this_t::q_flags, Flags>::value
            >
        >(&no_ref<underlying_type>::operator());
    }

private:

    template<quali::flags Flags>
    using harden_underlying = quali::qualify<
        general_type,
        quali::collapse<q_flags, Flags>::value
    >;

    template<quali::flags Flags>
    using add_pmf_qualifiers = pmf<
        typename base::template add_qualifiers<
            quali::collapse<q_flags, Flags>::value
        >
    >;

    template<quali::flags Flags>
    using can_call_after_hardened = typename base::template prepend_and_unpack_args_to_template<
            can_call_with,
            harden_underlying<Flags>
    >;

    struct dummy_function_object {
        template<typename... Args>
        typename base::return_type operator()(Args...);
    };

    template<quali::flags Flags>
    using result_of_call = typename base::template result_of_invoke_with_args<
        typename std::conditional<
            can_call_after_hardened<Flags>::value,
            harden_underlying<Flags>,
            dummy_function_object
        >::type
    >;

    template<quali::flags Flags>
    using hardened_pmf = typename add_pmf_qualifiers<
        quali::collapse<q_flags, Flags>::value
    >::template apply_return<result_of_call<Flags> >;

public:

    template<quali::flags Flags>
    using add_qualifiers = callable_wrapper<
        typename std::conditional<
            can_call_after_hardened<Flags>::value,
            function_object_wrapper_base<
                quali::collapse<q_flags, Flags>::value,
                GeneralizedObject,
                hardened_pmf<Flags>
            >,
            ambiguous_function_object_wrapper_base<
                quali::collapse<q_flags, Flags>::value,
                GeneralizedObject
            >
        >::type
    >;

    using invocation_data_type = GeneralizedObject;
    using type = typename base::decay_to_function;
    using clbl_tag = function_object_wrapper_tag;

    invocation_data_type data;

    template<quali::flags Flags, typename... Args>
    using apply_signature = callable_wrapper<
        function_object_wrapper_base<
            quali::collapse<q_flags, Flags>::value,
            GeneralizedObject,
            typename pmf<
                decltype(data.template get<quali::collapse<q_flags, Flags>::value>()(
                    std::declval<Args>()...)
                )
                (no_ref<underlying_type>::*)(Args...)
            >::template add_qualifiers<Flags>
        >
    >;

    function_object_wrapper_base() = default;
    function_object_wrapper_base(const function_object_wrapper_base&) = default;
    function_object_wrapper_base(function_object_wrapper_base&&) = default;

    function_object_wrapper_base(volatile function_object_wrapper_base& w)
        : data{ w.data } {}

    function_object_wrapper_base(const volatile function_object_wrapper_base& w)
        : data{ w.data }{}

    function_object_wrapper_base(unqualified<invocation_data_type> const & d)
        : data{ d }{};

    function_object_wrapper_base(unqualified<invocation_data_type> const volatile & d)
        : data{ d }{};

    function_object_wrapper_base(unqualified<invocation_data_type>&& d)
        : data{static_cast<unqualified<invocation_data_type>&&>(d)}{};

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) {
        return (data.template get<q_flags>()
            .*pmf_cast<quali::default_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(Args&&... args) {
        using flags_type = quali::force_rvalue_reference<q_flags>;
        return (data.template get<flags_type::value>()
            .*pmf_cast<quali::rvalue_reference_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return (data.template get<quali::const_ | q_flags>()
            .*pmf_cast<quali::const_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    move_invoke(Args&&... args) const {
        using flags_type = quali::force_rvalue_reference<
            quali::const_ | q_flags
        >;
        return (data.template get<flags_type::value>()
            .*pmf_cast<quali::const_ | quali::rvalue_reference_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) volatile {
        return (data.template get<quali::volatile_ | q_flags>()
            .*pmf_cast<quali::volatile_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    move_invoke(Args&&... args) volatile {
        using flags_type = quali::force_rvalue_reference<
            quali::volatile_ | q_flags
        >;
        return (data.template get<flags_type::value>()
            .*pmf_cast<quali::volatile_ | quali::rvalue_reference_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return (data.template get<quali::const_ | quali::volatile_ | q_flags>()
            .*pmf_cast<quali::const_ | quali::volatile_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    move_invoke(Args&&... args) const volatile {
        using flags_type = quali::force_rvalue_reference<
            quali::const_ | quali::volatile_ | q_flags
        >;
        return (data.template get<quali::const_ | quali::volatile_ | q_flags>()
            .*pmf_cast<quali::const_ | quali::volatile_ | quali::rvalue_reference_>())(static_cast<Args&&>(args)...);
    }
};

}}

#endif