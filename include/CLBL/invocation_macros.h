/*!
@file
Defines macros used for cv-safe invocations of different kinds of callable types.

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CLBL_INVOCATION_MACROS_H
#define CLBL_INVOCATION_MACROS_H

/*
these macros are extremely useful for tweaking invocation details for the internal data inside a
wrapper. All else being equal, it might have been be cleaner to implement and call our own version of
std::invoke instead of using these macros. However, the required SFINAE branching would be completely
redundant since we already know exactly how to make the call when invoking inside of a wrapper, and I
expect the additional SFINAE branching would SIGNIFICANTLY increase compile times, which are already
slow.
*/

#define CLBL_CALL_PTR(cv_ignored, ptr, args) (*ptr)(args)
#define CLBL_CALL_VAL(cv_ignored, val, args) val(args)
#define CLBL_UPCAST_AND_CALL_PTR(qual, ptr, args) harden_cast<cv<qual dummy> | cv_flags>(*ptr)(args)
#define CLBL_UPCAST_AND_CALL_VAL(qual, val, args) harden_cast<cv<qual dummy> | cv_flags>(val)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_PTR(qual, optr, member, args)(harden_cast<cv<qual dummy> | cv_flags>(*optr).*member)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_VAL(qual, obj, member, args) (harden_cast<cv<qual dummy> | cv_flags>(obj).*member)(args)

#endif