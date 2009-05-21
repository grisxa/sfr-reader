#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(sfr-reader, 0.2, bga@altlinux.org)
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

dnl Host specific hacks
#AC_CANONICAL_HOST
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([foreign])
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
#AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h strings.h sys/param.h unistd.h])
AC_CHECK_HEADERS([endian.h machine/endian.h byteorder.h asm/byteorder.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_TYPE_INT16_T
#AC_TYPE_INT32_T
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero memset strerror strrchr])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
