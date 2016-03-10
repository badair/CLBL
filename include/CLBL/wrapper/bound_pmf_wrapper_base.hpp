 /*!
@file

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_BOUND_PMF_WRAPPER_BASE_H
#define CLBL_BOUND_PMF_WRAPPER_BASE_H

namespace clbl { namespace internal {

template<typename BoundObject, typename PmfConstructorType>
class bound_pmf_wrapper_data {

public:
    
    struct invocation_data_type {
        BoundObject object;
        PmfConstructorType pmf;
    };

    inline constexpr
    bound_pmf_wrapper_data(const invocation_data_type& v)
        : data{v} {}

    inline constexpr
    bound_pmf_wrapper_data(invocation_data_type&& v)
        : data{static_cast<invocation_data_type&&>(v)} {}

protected:

    invocation_data_type data;

    inline constexpr decltype(auto)
    get_pmf() {
        return data.pmf;
    }

    inline constexpr decltype(auto)
    get_pmf() const {
        return data.pmf;
    }

    inline constexpr decltype(auto)
    get_pmf() volatile {
        return data.pmf;
    }

    inline constexpr decltype(auto)
    get_pmf() const volatile {
        return data.pmf;
    }
};


template<typename BoundObject, typename T, T Value>
class bound_pmf_wrapper_data<BoundObject, std::integral_constant<T, Value>> {

public:
    
    struct invocation_data_type {

       inline constexpr
        invocation_data_type(const BoundObject& o, std::integral_constant<T, Value> )
            : object{o} {}

        inline constexpr
        invocation_data_type(BoundObject&& o, std::integral_constant<T, Value> )
            : object{static_cast<BoundObject&&>(o)} {}

        BoundObject object;
    };

    inline constexpr
    bound_pmf_wrapper_data(const invocation_data_type& v)
        : data{v}{}

    inline constexpr
    bound_pmf_wrapper_data(invocation_data_type&& v)
        : data{static_cast<invocation_data_type&&>(v)} {}

protected:

    invocation_data_type data;

    static inline constexpr decltype(auto)
    get_pmf() {
        return Value;
    }
};

template<typename BoundObject, typename Dispatch>
struct volatile_enabled_bound_pmf_wrapper_base
    : public Dispatch,
    public bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type> {

    using base = bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type>;
    using base::base;
    using arg_types = typename Dispatch::arg_types;
    using clbl_tag = bound_pmf_tag;
    using forwarding_glue = typename Dispatch::forwarding_glue;
    using type = typename Dispatch::function_type;

    static constexpr auto q_flags = Dispatch::q_flags;
    static constexpr auto is_ambiguous = false;

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    invoke(Args&&... args) ->
        decltype((base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    move_invoke(Args&&... args) ->
        decltype((base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    invoke_c(Args&&... args) const ->
        decltype((base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    move_invoke_c(Args&&... args) const -> 
        decltype((base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    invoke_v(Args&&... args) volatile -> 
        decltype((base::data.object.template get<quali::volatile_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::volatile_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    move_invoke_v(Args&&... args) volatile -> 
        decltype((base::data.object.template get<quali::force_rvalue_reference<quali::volatile_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<quali::volatile_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    invoke_cv(Args&&... args) const volatile -> 
        decltype((base::data.object.template get<quali::const_ | quali::volatile_ | q_flags>().*base::data.pmf)(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::const_ | quali::volatile_ | q_flags>().*base::data.pmf)(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    move_invoke_cv(Args&&... args) const volatile ->
        decltype((base::data.object.template get<quali::force_rvalue_reference<quali::const_ | quali::volatile_ | q_flags>::value>().*base::data.pmf)(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<quali::const_ | quali::volatile_ | q_flags>::value>().*base::data.pmf)(static_cast<Args&&>(args)...);
    }
};

template<typename BoundObject, typename Dispatch>
struct volatile_disabled_bound_pmf_wrapper_base
    : public Dispatch,
    public bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type> {

    using base = bound_pmf_wrapper_data<BoundObject, typename Dispatch::constructor_type>;
    using base::base;
    using arg_types = typename Dispatch::arg_types;
    using clbl_tag = bound_pmf_tag;
    using forwarding_glue = typename Dispatch::forwarding_glue;
    using type = typename Dispatch::function_type;

    static constexpr auto q_flags = Dispatch::q_flags;
    static constexpr auto is_ambiguous = false;

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    invoke(Args&&... args) ->
        decltype((base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    move_invoke(Args&&... args) ->
        decltype((base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    invoke_c(Args&&... args) const ->
        decltype((base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)) {
        return   (base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    move_invoke_c(Args&&... args) const -> 
        decltype((base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    invoke_v(Args&&... args) -> 
        decltype((base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline CLBL_CXX14_CONSTEXPR auto
    move_invoke_v(Args&&... args) -> 
        decltype((base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    invoke_cv(Args&&... args) const -> 
        decltype((base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::const_ | q_flags>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }

    template<typename IgnoredFlags, typename... Args>
    inline constexpr auto
    move_invoke_cv(Args&&... args) const ->
        decltype((base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...)){
        return   (base::data.object.template get<quali::force_rvalue_reference<quali::const_ | q_flags>::value>().*base::get_pmf())(static_cast<Args&&>(args)...);
    }
};

template<typename BoundObject, typename Dispatch, typename = std::true_type>
struct bound_pmf_wrapper_base : volatile_disabled_bound_pmf_wrapper_base<BoundObject, Dispatch> {
    using base = volatile_disabled_bound_pmf_wrapper_base<BoundObject, Dispatch>;
    using base::base;
};

template<typename BoundObject, typename Dispatch>
struct bound_pmf_wrapper_base<BoundObject, Dispatch, std::integral_constant<bool, BoundObject::volatile_allowed>>
    : volatile_enabled_bound_pmf_wrapper_base<BoundObject, Dispatch> {
    using base = volatile_enabled_bound_pmf_wrapper_base<BoundObject, Dispatch>;
    using base::base;
};

}}

#endif