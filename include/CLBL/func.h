#ifndef CLBL_FUNC_H
#define CLBL_FUNC_H

#include "CLBL/convert_to.h"
#include "CLBL/harden.h"
#include "CLBL/free_function.h"
#include "CLBL/member_function.h"
#include "CLBL/overloaded_function_object.h"
#include "CLBL/overloaded_function_object_ptr.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/tags.h"

#include <type_traits>
#include <functional>

namespace clbl {
    namespace detail {

        /* todo why does this not work but the macro does?
        template<typename T>
        struct underlying_type_t{
            using type = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<T>())> >;
        };

        template<typename T>
        using underlying_type = typename underlying_type_t<T>::type;
        */

#define CLBL_UNDERLYING_TYPE_OF_PTR(T) std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<T>())> >
#define CLBL_UNDERLYING_TYPE(T) std::remove_cv_t<std::remove_reference_t<T> >


        template<typename T>
        struct is_reference_wrapper_t : std::false_type {};

        template <>
        template <typename T>
        struct is_reference_wrapper_t<std::reference_wrapper<T> > : std::true_type {};
       
        template<typename T>
        constexpr bool is_reference_wrapper = is_reference_wrapper_t<T>::value;

        //todo static assert
        struct null_function_object { void operator()() const volatile {} };

        struct null_reference_wrapper {
            null_function_object obj;
            null_reference_wrapper() : obj(null_function_object{}){}
            auto get() { return &obj; }
        };

        auto has_normal_call_operator_impl = hana::is_valid([](auto arg)->decltype(&decltype(arg)::operator()) {});
        auto ptr_has_normal_call_operator_impl = hana::is_valid([](auto arg)->decltype(&std::remove_reference_t<decltype(*arg)>::operator()) {});

        template<typename T>
        constexpr bool has_normal_call_operator = decltype(has_normal_call_operator_impl(std::declval<T>()))::value;

        template<typename T>
        constexpr bool ptr_has_normal_call_operator = decltype(ptr_has_normal_call_operator_impl(std::declval<T>()))::value;

        auto can_dereference_impl = hana::is_valid([](auto arg)->decltype(*arg) {});

        template<typename T>
        constexpr bool can_dereference = decltype(can_dereference_impl(std::declval<T>()))::value;

        template<typename T> 
        struct sfinae_switch {
        private:
            
            static constexpr auto is_ref_wrapper = is_reference_wrapper<T>;
            static constexpr auto is_overloaded = !has_normal_call_operator<T>;
            static constexpr auto ptr_is_overloaded = !ptr_has_normal_call_operator<T>;
            static constexpr auto is_function_ptr = std::is_function<std::remove_pointer_t<T> >::value;
            static constexpr auto is_member_function_ptr = std::is_member_function_pointer<T>::value;

        public:
            static constexpr auto is_ptr = can_dereference<T>;
            static constexpr auto reference_wrapper_case = is_ref_wrapper;
            static constexpr auto function_ptr_case = is_function_ptr;
            static constexpr auto member_function_ptr_case = !is_function_ptr && is_member_function_ptr;

        private:
            static constexpr auto is_complex_case = !reference_wrapper_case && !function_ptr_case && !member_function_ptr_case;

        public:
            static constexpr auto function_object_case =                is_complex_case && !is_ptr && !is_overloaded;
            static constexpr auto overloaded_function_object_case =     is_complex_case && !is_ptr && is_overloaded;

            static constexpr auto function_object_ptr_case =            is_complex_case && is_ptr && !ptr_is_overloaded;
            static constexpr auto overloaded_function_object_ptr_case = is_complex_case && is_ptr &&  ptr_is_overloaded;
        };
    }

    template<typename T>
    constexpr auto func(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::function_ptr_case,
            free_function<std::remove_reference_t<std::remove_pointer_t<T> > >
        > {
        return free_function<std::remove_reference_t<std::remove_pointer_t<T> > >{ std::forward<T>(t) };
    }

    template<typename T, typename TMemberFnPtr>
    constexpr auto func(T&& t, TMemberFnPtr member_fn_ptr) ->
        std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
                         && detail::sfinae_switch<T>::is_ptr,
        member_function_of_ptr<
        CLBL_UNDERLYING_TYPE_OF_PTR(T),
        T,
        TMemberFnPtr,
        member_function_decay<TMemberFnPtr>
        >
        > {

        return member_function_of_ptr<
            CLBL_UNDERLYING_TYPE_OF_PTR(T),
            T,
            TMemberFnPtr,
            member_function_decay<TMemberFnPtr>
        >{ std::forward<T>(t), member_fn_ptr };
    }

    template<typename T, typename TMemberFnPtr>
    constexpr auto func(T&& t, TMemberFnPtr member_fn_ptr) ->
        std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case
                         && !detail::sfinae_switch<T>::is_ptr 
                         && !detail::sfinae_switch<T>::reference_wrapper_case,
        member_function<
        CLBL_UNDERLYING_TYPE(T),
        T,
        TMemberFnPtr,
        member_function_decay<TMemberFnPtr>
        >
        > {

        return member_function<
            CLBL_UNDERLYING_TYPE(T),
            T,
            TMemberFnPtr,
            member_function_decay<TMemberFnPtr>
        >{ std::forward<T>(t), member_fn_ptr };
    }

    template<typename T>
    constexpr auto func(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::overloaded_function_object_ptr_case,
        overloaded_function_object_ptr<CLBL_UNDERLYING_TYPE_OF_PTR(T), T>
        > {

        return overloaded_function_object_ptr<
            CLBL_UNDERLYING_TYPE_OF_PTR(T), 
            T
        >{ std::forward<T>(t) };
    }

    template<typename T>
    constexpr auto func(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::function_object_ptr_case,
            member_function_of_ptr<
                CLBL_UNDERLYING_TYPE_OF_PTR(T),
                T,
                decltype(&std::remove_reference_t<decltype(*std::declval<T>())>::operator()),
                member_function_decay<decltype(&std::remove_reference_t<decltype(*std::declval<T>())>::operator())>
            > 
        > {

        return member_function_of_ptr<
            CLBL_UNDERLYING_TYPE_OF_PTR(T),
            T,
            decltype(&std::remove_reference_t<decltype(*std::declval<T>())>::operator()),
            member_function_decay<decltype(&std::remove_reference_t<decltype(*std::declval<T>())>::operator())>
        >{ std::forward<T>(t), &std::remove_reference_t<decltype(*std::declval<T>())>::operator() };
    }

    template<typename T>
    constexpr auto func(T&& t) 
        -> std::enable_if_t<detail::sfinae_switch<T>::function_object_case,
            member_function<
                CLBL_UNDERLYING_TYPE(T),
                T,
                decltype(&std::remove_reference_t<decltype(std::declval<T>())>::operator()),
                member_function_decay<decltype(&std::remove_reference_t<decltype(std::declval<T>())>::operator())>
            >
        > {

        return member_function<
            CLBL_UNDERLYING_TYPE(T),
            T,
            decltype(&std::remove_reference_t<decltype(std::declval<T>())>::operator()),
            member_function_decay<decltype(&std::remove_reference_t<decltype(std::declval<T>())>::operator())>
        >{std::forward<T>(t), &std::remove_reference_t<decltype(std::declval<T>())>::operator()};
    }

    template<typename T>
    constexpr auto func(T&& t) 
        -> std::enable_if_t<detail::sfinae_switch<T>::overloaded_function_object_case,
            overloaded_function_object<T> 
        > {

        return overloaded_function_object<T>{std::forward<T>(t)};
    }

    template<typename T>
    constexpr auto func(T&& t) 
        -> std::enable_if_t<detail::sfinae_switch<T>::reference_wrapper_case,
            decltype(func(&t.get()))
        > {

        return func(&t.get());
    }

    template<typename T, typename TMemberFnPtr>
    constexpr auto func(T t, TMemberFnPtr ptr)
        -> std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case && 
                            detail::sfinae_switch<T>::reference_wrapper_case,
        decltype(func(&t.get()))
        > {

        return func(&t.get(), ptr);
    }
}

#endif