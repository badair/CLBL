#ifndef CLBL_FORWARDABLE_H
#define CLBL_FORWARDABLE_H

namespace clbl {

    namespace detail {
        template<typename T>
        struct forwardable_t { using type = const T&; };

        template<typename T>
        struct forwardable_t<T&&> { using type = T&&; };

        template<typename T>
        struct forwardable_t<T&> { using type = T&; };
    }

    template<typename T>
    using forwardable = typename detail::forwardable_t<T>::type;

    namespace detail {
        template<typename T>  
        struct return_forwardable_t { using type = T&&; };

        template<typename T> 
        struct return_forwardable_t<T&&> { using type = T&&; };

        template<typename T> 
        struct return_forwardable_t<T&> { using type = T&; };
    }

    template<typename T>
    using return_forwardable = typename detail::return_forwardable_t<T>::type;

    template<typename FwdType>
    struct forward {

        using T = std::conditional_t <
            std::is_lvalue_reference<FwdType>::value || std::is_rvalue_reference<FwdType>::value,
            forwardable<FwdType>,
            std::add_lvalue_reference_t<forwardable<FwdType> >
        >;

        T value;

        inline forward(std::remove_reference_t<T>&& t) : value(t) {}
        inline forward(std::remove_const<std::remove_reference_t<T> >& t) : value(t) {}
        inline forward(const std::remove_const<std::remove_reference_t<T> >& t) : value(t) {}

        inline forward() = default;
        inline forward(forward<FwdType>&) = default;
        inline forward(const forward<FwdType>&) = default;
        inline forward(forward<FwdType>&&) = default;
        inline forward(volatile forward<FwdType>& other) : value(other.value) {}
        inline forward(const volatile forward<FwdType>& other) : value(other.value) {}

        using no_ref_type = std::remove_reference_t<T>;

        /*template<typename U>
        static constexpr inline T convert(U&& v) {

            if (std::is_rvalue_reference<T>::value) {
                return std::move(v);
            }

            return v;
        }*/

        //todo move rvalues 

        inline operator std::conditional_t<std::is_rvalue_reference<T>::value, no_ref_type, T>() const { return value; }
        inline operator std::conditional_t<std::is_rvalue_reference<T>::value, no_ref_type, T>() const volatile { return value; }
    };
}

#endif