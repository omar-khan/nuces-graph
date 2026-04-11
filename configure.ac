AC_PREREQ([2.69])
AC_INIT([nuces-graph], [1.0.0], [omar.khan@nu.edu.pk])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AM_PROG_AR
LT_INIT

AC_CONFIG_SRCDIR([src/util.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

# Checks for libraries.
AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([glpk], [glp_create_prob], [], [AC_MSG_ERROR([glpk library not found])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

PKG_PROG_PKG_CONFIG

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
])
AC_OUTPUT
