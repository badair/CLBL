#ifndef CLBL_FWRAP_H
#define CLBL_FWRAP_H

#include <type_traits>
#include <functional>

#include "CLBL/wrappers/free_fn_wrapper.h"
#include "CLBL/wrappers/pmf_wrapper.h"
#include "CLBL/wrappers/pmf_ptr_wrapper.h"
#include "CLBL/wrappers/ambi_fn_obj_wrapper.h"
#include "CLBL/wrappers/ambi_fn_obj_ptr_wrapper.h"
#include "CLBL/member_function_decay.h"
#include "CLBL/tags.h"
#include "CLBL/utility.h"

namespace clbl {
    namespace detail {

        /* todo why does this not work but the macro does?
        template<typename T>
        struct underlying_type_t{
            using type = std::remove_cv_t<no_ref<decltype(*std::declval<T>())> >;
        };

        template<typename T>
        using underlying_type = typename underlying_type_t<T>::type;
        */

        auto has_normal_call_operator_impl = hana::is_valid([](auto arg)->decltype(&decltype(arg)::operator()) {});
        auto ptr_has_normal_call_operator_impl = hana::is_valid([](auto arg)->decltype(&no_ref<decltype(*arg)>::operator()) {});

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


    /********************
    free function pointer
    *********************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::function_ptr_case,
            free_fn_wrapper<no_ref<std::remove_pointer_t<T> > >
        > {
        return free_fn_wrapper<no_ref<std::remove_pointer_t<T> > >{ std::forward<T>(t) };
    }


    /***********************************************
    Pointer to object with a member function pointer
    ************************************************/

    template<typename T, typename TMemberFnPtr>
    constexpr auto fwrap(T&& t, TMemberFnPtr member_fn_ptr) ->
        std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case 
                         && detail::sfinae_switch<T>::is_ptr,
        pmf_ptr_wrapper<
        no_ref<decltype(*std::declval<T>())>,
        no_ref<T>,
        TMemberFnPtr,
        member_function_decay<TMemberFnPtr>
        >
        > {

        return pmf_ptr_wrapper<
            no_ref<decltype(*std::declval<T>())>,
            no_ref<T>,
            TMemberFnPtr,
            member_function_decay<TMemberFnPtr>
        >{ std::forward<T>(t), member_fn_ptr };
    }


    /**********************************
    Object with member function pointer
    ***********************************/

    template<typename T, typename TMemberFnPtr>
    constexpr auto fwrap(T&& t, TMemberFnPtr member_fn_ptr) ->
        std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case
                         && !detail::sfinae_switch<T>::is_ptr 
                         && !detail::sfinae_switch<T>::reference_wrapper_case,
        pmf_wrapper<
        no_ref<T>,
        TMemberFnPtr,
        member_function_decay<TMemberFnPtr>
        >
        > {

        return pmf_wrapper<
            no_ref<T>,
            TMemberFnPtr,
            member_function_decay<TMemberFnPtr>
        >{ std::forward<T>(t), member_fn_ptr };
    }

    /********************************
    Pointer to object with operator()
    *********************************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::function_object_ptr_case,
            pmf_ptr_wrapper<
                no_ref<decltype(*std::declval<T>())>,
                no_ref<T>,
                decltype(&no_ref<decltype(*std::declval<T>())>::operator()),
                member_function_decay<decltype(&no_ref<decltype(*std::declval<T>())>::operator())>
            > 
        > {

        return pmf_ptr_wrapper<
            no_ref<decltype(*std::declval<T>())>,
            no_ref<T>,
            decltype(&no_ref<decltype(*std::declval<T>())>::operator()),
            member_function_decay<decltype(&no_ref<decltype(*std::declval<T>())>::operator())>
        >{ std::forward<T>(t), &no_ref<decltype(*std::declval<T>())>::operator() };
    }


    /********************************************
    Pointer to object with AMBIGUOUS operator()
    *********************************************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::overloaded_function_object_ptr_case,
        ambi_fn_obj_ptr_wrapper<no_ref<T> >
        > {

        return ambi_fn_obj_ptr_wrapper<no_ref<T> >{ std::forward<T>(t) };
    }


    /**********************
    Object with operator()
    ***********************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::function_object_case,
            pmf_wrapper<
                no_ref<T>,
                decltype(&no_ref<T>::operator()),
                member_function_decay<decltype(&no_ref<T>::operator())>
            >
        > {

        return pmf_wrapper<
            no_ref<T>,
            decltype(&no_ref<T>::operator()),
            member_function_decay<decltype(&no_ref<T>::operator())>
        >{std::forward<T>(t), &no_ref<T>::operator()};
    }


    /*******************************
    Object with AMBIGUOUS operator()
    ********************************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::overloaded_function_object_case,
            ambi_fn_obj_wrapper<no_ref<T> > 
        > {

        return ambi_fn_obj_wrapper< no_ref<T> >{std::forward<T>(t)};
    }

    /*********************
    std::reference_wrapper
    **********************/

    template<typename T>
    constexpr auto fwrap(T&& t)
        -> std::enable_if_t<detail::sfinae_switch<T>::reference_wrapper_case,
            decltype(fwrap(std::addressof(t.get())))
        > {

        //toss the reference
        return fwrap(std::addressof(t.get()));
    }

    template<typename T, typename TMemberFnPtr>
    constexpr auto fwrap(T&& t, TMemberFnPtr ptr)
        -> std::enable_if_t<detail::sfinae_switch<TMemberFnPtr>::member_function_ptr_case && 
                            detail::sfinae_switch<T>::reference_wrapper_case,
        decltype(fwrap(std::addressof(t.get())))
        > {

        //toss the reference
        return fwrap(std::addressof(t.get()), ptr);
    }
}

#endif