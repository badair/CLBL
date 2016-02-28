 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_FUNCTION_OBJECT_WRAPPER_BASE_H
#define CLBL_FUNCTION_OBJECT_WRAPPER_BASE_H

namespace clbl { namespace internal {

template<qualify_flags QFlags, typename GeneralizedObject, typename Pmf = dummy>
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

    template<qualify_flags Flags>
    using add_qualifiers = 
    callable_wrapper<
        ambiguous_function_object_wrapper_base<
            qflags::collapse_reference<QFlags, Flags>::value,
            GeneralizedObject,
            Pmf
        >
    >;

    invocation_data_type data;

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
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return data.template get<qflags::const_ | q_flags>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) volatile {
        return data.template get<qflags::volatile_ | q_flags>()(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return data.template get<qflags::const_ | qflags::volatile_ | q_flags>()(
            static_cast<Args&&>(args)...
        );
    }
};

template<
    qualify_flags QFlags,
    typename GeneralizedObject,
    typename Pmf = call_operator<default_normal_callable<typename GeneralizedObject::type>>
>
struct function_object_wrapper_base 
    : public pmf<Pmf> {

    using pmf_type = Pmf;
    using generalized_object = GeneralizedObject;
    using base = pmf<Pmf>;
    using this_t = function_object_wrapper_base;

    static constexpr const qualify_flags q_flags = 
        qflags::collapse_reference<
            base::q_flags | pmf<decltype(&default_normal_callable<typename GeneralizedObject::type>::operator())>::q_flags,
            QFlags
        >::value;

    using underlying_type = qualified_type<typename GeneralizedObject::type, q_flags>;
    static constexpr const bool is_ambiguous = false;

    template<qualify_flags Flags>
    static inline constexpr decltype(auto)
    pmf_cast(){
        return static_cast<
            typename base::template add_qualifiers<
                qflags::collapse_reference<this_t::q_flags, Flags>::value
            >
        >(&underlying_type::operator());
    }

    template<qualify_flags Flags>
    using add_qualifiers =
        callable_wrapper<
            function_object_wrapper_base<
            qflags::collapse_reference<QFlags, Flags>::value,
            GeneralizedObject,
            typename base::template add_qualifiers<qflags::collapse_reference<QFlags, Flags>::value>
        >
    >;

    using invocation_data_type = GeneralizedObject;
    using type = typename base::decay_to_function;
    using clbl_tag = function_object_wrapper_tag;

    invocation_data_type data;

    function_object_wrapper_base() = default;
    function_object_wrapper_base(const function_object_wrapper_base&) = default;
    function_object_wrapper_base(function_object_wrapper_base&&) = default;

    function_object_wrapper_base(volatile function_object_wrapper_base& w)
        : data{ w.data } {}

    function_object_wrapper_base(const volatile function_object_wrapper_base& w)
        : data{w.data}{}

    function_object_wrapper_base(unqualified<invocation_data_type> const & d)
        : data{d}{};

    function_object_wrapper_base(unqualified<invocation_data_type> const volatile & d)
        : data{d}{};

    function_object_wrapper_base(unqualified<invocation_data_type>&& d)
        : data{static_cast<unqualified<invocation_data_type>&&>(d)}{};

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) {
        return (data.template get<q_flags>()
            .*pmf_cast<qflags::default_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const {
        return (data.template get<qflags::const_ | q_flags>()
            .*pmf_cast<qflags::const_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline CLBL_CXX14_CONSTEXPR decltype(auto)
    invoke(Args&&... args) volatile {
        return (data.template get<qflags::volatile_ | q_flags>()
            .*pmf_cast<qflags::volatile_>())(static_cast<Args&&>(args)...);
    }

    template<typename... Args>
    inline constexpr decltype(auto)
    invoke(Args&&... args) const volatile {
        return (data.template get<qflags::const_ | qflags::volatile_ | q_flags>()
            .*pmf_cast<qflags::const_ | qflags::volatile_>())(static_cast<Args&&>(args)...);
    }
};

}}

#endif