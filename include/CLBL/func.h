#ifndef CLBL_FUNC_H
#define CLBL_FUNC_H

#include "CLBL/callable.h"
#include "CLBL/free_function.h"
#include "CLBL/member_function.h"
#include "CLBL/overloaded_function_object.h"
#include "CLBL/overloaded_function_object_ptr.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/tags.h"
#include "CLBL/dispatch.h"

#include <type_traits>
#include <functional>

namespace clbl {
    namespace detail {

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

        template<typename T>
        constexpr bool has_normal_call_operator = decltype(has_normal_call_operator_impl(std::declval<T>()))::value;

        auto can_dereference_impl = hana::is_valid([](auto arg)->decltype(*arg) {});

        template<typename T>
        constexpr bool can_dereference = decltype(can_dereference_impl(std::declval<T>()))::value;

        template<typename T> 
        struct sfinae_switch {
        private:
            static constexpr auto is_ptr = can_dereference<T>;
            static constexpr auto is_ref_wrapper = detail::is_reference_wrapper<T>;
            static constexpr auto is_overloaded = !has_normal_call_operator<T>;

        public:
            static constexpr auto ptr_case = is_ptr;
            static constexpr auto function_object_case = !is_ptr && !is_ref_wrapper && !is_overloaded;
            static constexpr auto overloaded_function_object_case = !is_ptr && !is_ref_wrapper && is_overloaded;
            static constexpr auto reference_wrapper_case = is_ref_wrapper;
        };

        template<typename TPtr, typename TMemberFnPtr>
        struct dispatch_context_for_ptr {

            using underlying_type = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<TPtr>())> >;

            static constexpr auto is_function_ptr = std::is_function<std::remove_pointer_t<TPtr> >::value;
            static constexpr auto is_member_function_ptr = std::is_member_function_pointer<TMemberFnPtr>::value;
            static constexpr auto is_function_object_ptr = !is_function_ptr
                && !is_member_function_ptr
                && detail::has_normal_call_operator<underlying_type>;

            static constexpr auto is_normal = is_function_ptr || is_member_function_ptr || is_function_object_ptr;

            using function_object_type = std::conditional_t<is_function_object_ptr, underlying_type, null_function_object>;

            using call_operator_type = decltype(&function_object_type::operator());
            static constexpr auto call_operator_ptr = &function_object_type::operator();
        };
    }

    template<typename TPtr, typename TMemberFnPtr = nullptr_t, 
        typename = std::enable_if_t<detail::sfinae_switch<TPtr>::ptr_case, void> >
    constexpr auto func(TPtr tptr, TMemberFnPtr member_fn_ptr = nullptr) {

        using context = detail::dispatch_context_for_ptr<TPtr, TMemberFnPtr>;

        using underlying_type = typename context::underlying_type;
        using function_object_type = typename context::function_object_type;
        using call_operator_type = typename context::call_operator_type;

        auto free_function_result = free_function<std::remove_pointer_t<TPtr> >{ tptr };

        auto member_function_result = member_function<
            underlying_type,
            TPtr,
            TMemberFnPtr,
            member_function_decay<TMemberFnPtr>
        >{ tptr, member_fn_ptr };

        auto function_object_result = member_function<
            function_object_type,
            TPtr,
            call_operator_type,
            member_function_decay<call_operator_type>
        >{ tptr, context::call_operator_ptr };

        auto overloaded_function_object_ptr_result = overloaded_function_object_ptr<underlying_type, TPtr>{ tptr };

        return detail::dispatch<context::is_function_ptr, context::is_member_function_ptr,
            context::is_function_object_ptr, !context::is_normal>::result(
            free_function_result, member_function_result, function_object_result, overloaded_function_object_ptr_result);
    }

    template<typename T>
    constexpr auto func(T&& t) -> std::enable_if_t<detail::sfinae_switch<T>::function_object_case,
        decltype(overloaded_function_object<T>{std::forward<T>(t)})> {

        /*
        todo make version of member_function that takes an object instead of
        an object ptr so that we can extract the return type and arguments.
        also need to make a by-value version that accepts member functions 
        */

        return overloaded_function_object<T>{std::forward<T>(t)};
    }

    template<typename T>
    constexpr auto func(T&& t) -> std::enable_if_t<detail::sfinae_switch<T>::overloaded_function_object_case,
        decltype(overloaded_function_object<T>{std::forward<T>(t)})> {
        return overloaded_function_object<T>{std::forward<T>(t)};
    }

    template<typename T>
    constexpr auto func(T&& t) -> std::enable_if_t<detail::sfinae_switch<T>::reference_wrapper_case,
        decltype(func(&t.get()))> {
        return func(&t.get());
    }
}

#endif