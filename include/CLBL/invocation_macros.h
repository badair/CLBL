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
these macros are currently useful for making changes to the way things are actually invoked. These
are temporary and will be hand-replaced in the future.
*/

#define CLBL_CALL_PTR(cv_ignored, ptr, args) (*ptr)(args)
#define CLBL_UPCAST_AND_CALL_PTR(qual, ptr, args) harden_cast<cv<qual dummy> | cv_flags>(*ptr)(args)
#define CLBL_UPCAST_AND_CALL_VAL(qual, val, args) harden_cast<cv<qual dummy> | cv_flags>(val)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_PTR(qual, optr, member, args)(harden_cast<cv<qual dummy> | cv_flags>(*optr).*member)(args)
#define CLBL_UPCAST_AND_CALL_MEMBER_VAL(qual, obj, member, args) (harden_cast<cv<qual dummy> | cv_flags>(obj).*member)(args)

#endif