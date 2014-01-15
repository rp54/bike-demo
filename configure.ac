#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AM_INIT_AUTOMAKE([blkedemo], [0.1])
AC_INIT([bikedemo], [0.1], [russell.potter3@gmail.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

SUBIRS = src

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P

# check for libraries
AC_SEARCH_LIBS([iconv], [iconv])
AC_SEARCH_LIBS([lzw],   [lzw_read])
AC_SEARCH_LIBS([z],     [Inflate])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h
                  wchar.h])

# Checks for typedefs, structures, and compiler
# characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
