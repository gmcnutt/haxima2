AC_INIT([haxima2], [0.0.1])
dnl AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
dnl AC_CANONICAL_HOST
dnl AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE
AC_PROG_CC
dnl AC_PROG_MAKE_SET

dnl dnl Library checks
SDL2_VERSION=2.0.10
AM_PATH_SDL2($SDL2_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL2_VERSION not found!]))

dnl dnl Header checks
dnl AC_LANG([C])

dnl dnl Workaround missing compiler keywords
dnl AC_C_CONST
dnl AC_C_INLINE
dnl AC_C_VOLATILE

dnl dnl Add in the SDL CFLAGS
dnl CFLAGS="$CFLAGS $SDL_CFLAGS"

dnl CC="$CXX"

dnl dnl Add libraries to build process
LIBS="$LIBS $SDL_LIBS"

dnl dnl User directories for saving game data
dnl AC_DEFINE(HOME_ENV, "HOME",
dnl            [Most likely your home directory is stored in $HOME.])

dnl Finally create all the generated files
AC_CONFIG_FILES([Makefile m4/Makefile src/Makefile])

AC_OUTPUT
