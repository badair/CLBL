#ifndef CLBL_ABOMINABLE_FUNCTION_DECAY
#define CLBL_ABOMINABLE_FUNCTION_DECAY


#define __SPECIALIZE_ABOMINABLE_DECAY(qualifiers) \
    template<typename Return, typename... Args> \
    struct abominable_decay_t<Return(Args...) qualifiers> { \
        using type = Return(Args...); \
    }

#define __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(qualifiers) \
    template<typename Return, typename... Args> \
    struct abominable_decay_t<Return(Args...,...) qualifiers> { \
        using type = Return(Args...,...); \
    }

namespace clbl {

    //primary template fails silently
    template<typename Other> struct abominable_decay_t { using type = std::remove_cv_t<Other>; };

    __SPECIALIZE_ABOMINABLE_DECAY(CLBL_NOTHING);
    __SPECIALIZE_ABOMINABLE_DECAY(&);
    __SPECIALIZE_ABOMINABLE_DECAY(&&);
    __SPECIALIZE_ABOMINABLE_DECAY(const);
    __SPECIALIZE_ABOMINABLE_DECAY(volatile);
    __SPECIALIZE_ABOMINABLE_DECAY(const volatile);
    __SPECIALIZE_ABOMINABLE_DECAY(const &);
    __SPECIALIZE_ABOMINABLE_DECAY(volatile &);
    __SPECIALIZE_ABOMINABLE_DECAY(const volatile &);
    __SPECIALIZE_ABOMINABLE_DECAY(const &&);
    __SPECIALIZE_ABOMINABLE_DECAY(volatile &&);
    __SPECIALIZE_ABOMINABLE_DECAY(const volatile &&);

    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(CLBL_NOTHING);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(&);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(&&);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(volatile);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const volatile);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const &);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(volatile &);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const volatile &);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const &&);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(volatile &&);
    __SPECIALIZE_ABOMINABLE_DECAY_ELLIPSES(const volatile &&);

    template<typename T>
    using abominable_function_decay = typename abominable_decay_t<T>::type;
}

#endif