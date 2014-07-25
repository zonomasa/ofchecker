#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(ofchecker, 0.0.1, hoge)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/ofchecker.c])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])

# Checks for header files.
AC_CHECK_HEADERS([malloc.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

AC_ENABLE_SHARED
AC_DISABLE_STATIC
#AC_PROG_LIBTOOL(libtool)

AC_CONFIG_FILES([Makefile])
AC_PROG_RANLIB
AC_OUTPUT
