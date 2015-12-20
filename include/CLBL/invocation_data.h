#ifndef CLBL_INVOCATION_DATA_H
#define CLBL_INVOCATION_DATA_H

namespace clbl {

    template<typename TPtr>
    struct ptr_invocation_data {
        TPtr ptr;

        using my_type = ptr_invocation_data<TPtr>;

        inline ptr_invocation_data(my_type&) = default;
        inline ptr_invocation_data(const my_type&) = default;
        inline ptr_invocation_data(my_type&&) = default;

        inline ptr_invocation_data(volatile my_type& other)
            : ptr{ other.ptr }
        {}

        inline ptr_invocation_data(const volatile my_type& other)
            : ptr{ other.ptr }
        {}

        inline ptr_invocation_data(std::remove_const_t<TPtr>& p)
            : ptr{ p }
        {}

        inline ptr_invocation_data(const TPtr& p)
            : ptr{ p }

        {}
        inline ptr_invocation_data(TPtr&& p)
            : ptr{ std::move(p) }
        {}
    };

    template<typename T>
    struct object_invocation_data {
        T object;

        using my_type = object_invocation_data<T>;

        inline object_invocation_data(my_type&) = default;
        inline object_invocation_data(const my_type&) = default;
        inline object_invocation_data(my_type&&) = default;

        inline object_invocation_data(volatile my_type& other)
            : object{ other.object }
        {}

        inline object_invocation_data(const volatile my_type& other)
            : object{ other.object }
        {}

        inline object_invocation_data(std::remove_const_t<T>& o)
            : object{ o }
        {}

        inline object_invocation_data(const T& o)
            : object{ o }
        {}

        inline object_invocation_data(T&& o)
            : object{ std::move(o) }
        {}
    };

    template<typename T, typename TMemberFnPtr>
    struct pmf_invocation_data {
        TMemberFnPtr pmf;
        T object;

        using my_type = pmf_invocation_data<T, TMemberFnPtr>;

        inline pmf_invocation_data(my_type&) = default;
        inline pmf_invocation_data(const my_type&) = default;
        inline pmf_invocation_data(my_type&&) = default;

        inline pmf_invocation_data(volatile my_type& other)
            : pmf{ other.pmf }, object{ other.object }
        {}

        inline pmf_invocation_data(const volatile my_type& other)
            : pmf{ other.pmf }, object{ other.object }
        {}

        inline pmf_invocation_data(TMemberFnPtr p, std::remove_const_t<T>& o)
            : pmf{ p }, object{ o }
        {}

        inline pmf_invocation_data(TMemberFnPtr p, const T& o)
            : pmf{ p }, object{ o }

        {}
        inline pmf_invocation_data(TMemberFnPtr p, T&& o)
            : pmf{ p }, object{ std::move(o) }
        {}
    };

    template<typename TPtr, typename TMemberFnPtr>
    struct indirect_pmf_invocation_data {
        TMemberFnPtr pmf;
        TPtr object_ptr;

        using my_type = indirect_pmf_invocation_data<TPtr, TMemberFnPtr>;

        inline indirect_pmf_invocation_data(my_type&) = default;
        inline indirect_pmf_invocation_data(const my_type&) = default;
        inline indirect_pmf_invocation_data(my_type&&) = default;

        inline indirect_pmf_invocation_data(volatile my_type& other)
            : pmf{ other.pmf }, object_ptr{ other.object_ptr }
        {}

        inline indirect_pmf_invocation_data(const volatile my_type& other)
            : pmf{ other.pmf }, object_ptr{ other.object_ptr }
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, std::remove_const_t<TPtr>& o)
            : pmf{ p }, object_ptr{ o }
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, const TPtr& o)
            : pmf{ p }, object_ptr{ o }
        {}

        inline indirect_pmf_invocation_data(TMemberFnPtr p, TPtr&& o)
            : pmf{ p }, object_ptr{ std::move(o) }
        {}
    };
}

#endif