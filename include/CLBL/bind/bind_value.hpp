
/*!
Copyright (c) 2001-2004 Peter Dimov and Multi Media Ltd.
Copyright (c) 2001 David Abrahams
Copyright (c) 2005 Peter Dimov
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef CLBL_BIND_VALUE_HPP
#define CLBL_BIND_VALUE_HPP

namespace clbl{ namespace internal { namespace detail { 

template<typename T>
struct bind_value {

public:
    //TODO constexpr inline
    bind_value(typename std::decay<T>::type const & t)
        : t_( t ) {}

    bind_value(const typename std::decay<T>::type && t)
        : t_( t ) {}

    CLBL_REQUIRES(!std::is_rvalue_reference<T>{})
    T & get() { return t_; }

    CLBL_REQUIRES(!std::is_rvalue_reference<T>{})
    T const & get() const { return t_; }

    //creating a new rvalue reference by invoking the copy ctor
    /*CLBL_REQUIRES(std::is_rvalue_reference<T>{})
    no_ref<T> get() const { return static_cast<no_ref<T> const &>(t_); }*/

private:

    T t_;
};

}}}

#endif