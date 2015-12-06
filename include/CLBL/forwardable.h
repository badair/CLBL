namespace clbl {

    namespace detail {
        template<typename T>
        struct forwardable_t {
            using type = const T&;
        };

        template<typename T>
        struct forwardable_t<T&&> {
            using type = T&&;
        };

        template<typename T>
        struct forwardable_t<T&> {
            using type = T&;
        };
    }

    template<typename T>
    using forwardable = typename detail::forwardable_t<T>::type;
}