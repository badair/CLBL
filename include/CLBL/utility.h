#ifndef CLBL_UTILITY_H
#define CLBL_UTILITY_H

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
}


#endif