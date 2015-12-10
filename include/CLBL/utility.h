#ifndef CLBL_UTILITY_H
#define CLBL_UTILITY_H

#define CLBL_UPCAST(cv_qual, x) (static_cast<cv_qual decltype(x)>(x))
#define CLBL_UPCAST_AND_CALL_PTR(cv_qual, ptr, args) (static_cast<cv_qual decltype(*ptr)>(*ptr))(args)
#define CLBL_UPCAST_AND_CALL_VAL(cv_qual, val, args) (static_cast<cv_qual decltype(*std::addressof(val))>(val))(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_PTR(cv_qual, obj, member, args) ((static_cast<cv_qual decltype(*obj)>(*obj)).*member)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_VAL(cv_qual, obj, member, args) ((static_cast<cv_qual decltype(*std::addressof(obj))>(*obj)).*member)(args)
namespace clbl {

    template<typename T>
    using no_ref = std::remove_reference_t<T>;

    template<typename Callable>
    using args = typename no_ref<Callable>::args_t;

    template<typename T>
    struct is_reference_wrapper_t : std::false_type {};

    template <typename T>
    struct is_reference_wrapper_t<std::reference_wrapper<T> > : std::true_type {};

    template<typename T>
    constexpr bool is_reference_wrapper = is_reference_wrapper_t<T>::value;

    template<typename Callable>
    using invocation_copy = decltype(Callable::copy_invocation(std::declval<std::add_lvalue_reference_t<Callable> >()));

}


#endif