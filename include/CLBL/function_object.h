#ifndef CLBL_FUNCTION_OBJECT_H
#define CLBL_FUNCTION_OBJECT_H

/*
#include <type_traits>

namespace clbl {

    template<typename T>
    struct overloaded_function_object
        //CRTP to callable
        : callable<overloaded_function_object<T>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object(T o)
            : obj(o)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return obj(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return obj(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return obj(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return obj(std::forward<Fargs>(a)...);
        }

    private:
        T obj;
    };

    template<typename UnderlyingType, typename TPtr>
    struct overloaded_function_object_ptr
        //CRTP to callable
        : callable<overloaded_function_object_ptr<UnderlyingType, TPtr>, ambiguous_return(ambiguous_args)> {

        overloaded_function_object_ptr(TPtr o_ptr)
            : ptr(o_ptr)
        {}

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) volatile {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline auto operator()(Fargs&&... a) const volatile {
            return (*ptr)(std::forward<Fargs>(a)...);
        }

    private:
        TPtr ptr;
    };
}
*/

#endif