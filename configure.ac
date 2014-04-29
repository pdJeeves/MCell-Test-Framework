#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([TestMCell], [1.0], [pdj9@pitt.edu])
m4_include([m4-ax_boost_base.m4])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([TestMCell], [1.0])
AM_CONFIG_HEADER(config.h)

# Checks for programs.
CXXFLAGS="$CXXFLAGS -std=c++11 `python-config --includes`"
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AM_MAINTAINER_MODE
AC_ISC_POSIX
AM_PROG_CC_STDC
AM_PROG_LIBTOOL

# Checks for libraries.
AX_BOOST_BASE([1.5.2])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_C_VOLATILE
AC_C_CONST
AC_C_BIGENDIAN
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit memset mkdir sqrt])
AC_CHECK_FUNCS(memset malloc free)
AC_CHECK_FUNCS(makecontext getcontext swapcontext)
AC_CHECK_FUNCS(sigaction)
AC_CHECK_FUNCS(longjmp setjmp)
AC_CHECK_FUNCS(sigaltstack)

AC_OUTPUT(Makefile src/Makefile man/Makefile)
