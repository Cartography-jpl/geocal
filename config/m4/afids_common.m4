#=================================================================
# A few things that are common to all our configure files.

AC_DEFUN([AFIDS_COMMON],[
AC_REQUIRE([AC_CONFIG_AUX_DIR_DEFAULT])
AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_F77_LIBRARY_LDFLAGS])
AC_REQUIRE([AC_PROG_FC])
# For some bizarre reason, this doesn't fail if there isn't a C++ compiler.
# This seems to be a bug, which had some discussion on the forums a while back
# (see http://lists.gnu.org/archive/html/bug-autoconf/2010-05/msg00001.html),
# but apparently hasn't been fixed. We work around this by checking if
# the CXX program is actually on the system.
AC_REQUIRE([AC_PROG_CXX])
AC_CHECK_PROG(HAVE_CXX, $CXX, yes, no)
if test "$HAVE_CXX" = "no"; then
   AC_MSG_ERROR([Could not find a c++ compiler]);
fi

# We check for the very basic programs perl and patch. Surprising if not
# found, but possible, so we check.
AC_CHECK_PROG(HAVE_PERL, perl, yes, no)
if test "$HAVE_PERL" = "no"; then
   AC_MSG_ERROR([Could not find a perl, which is required for install]);
fi
AC_CHECK_PROG(HAVE_PATCH, patch, yes, no)
if test "$HAVE_PATCH" = "no"; then
   AC_MSG_ERROR([Could not find a patch, which is required for install]);
fi

# We need to have csh to run things like vicarb
AC_CHECK_PROG(HAVE_CSH, csh, yes, no)
if test "$HAVE_CSH" = "no"; then
   AC_MSG_ERROR([Could not find csh, which is required for programs such as vicar]);
fi

# We use a few GNU make specific things, so make sure we have gnu make
AX_CHECK_GNU_MAKE()
if test "$_cv_gnu_make_command" = "" ; then
   AC_MSG_ERROR([Could not find a gnu version of make]);
fi

AC_COPYRIGHT(
[Copyright 2014, California Institute of Technology. 
ALL RIGHTS RESERVED. U.S. Government Sponsorship acknowledged.])
# The obscure looking tar-pax here sets automake to allow file names longer
# than 99 characters to be included in the dist tar. See
# http://noisebleed.blogetery.com/2010/02/27/tar-file-name-is-too-long-max-99/#howtofixit
AM_INIT_AUTOMAKE([1.9 tar-pax])
AM_MAINTAINER_MODE
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_LN_S
AC_COPY_DIR

#AC_PREFIX_DEFAULT([`pwd`/install])
AC_PROG_CC
AC_PROG_F77
AC_PROG_FC
AC_F77_LIBRARY_LDFLAGS
AC_F77_WRAPPERS

AM_PROG_CC_C_O
AC_ENABLE_DEBUG

#=================================================================
# Test if we are using GCC compiler. Some flags get set in the 
# Makefile that should only be set for GCC.
#=================================================================

AM_CONDITIONAL([HAVE_GCC], [test "$GCC" = yes])

#=================================================================
# We have a small amount of code that gets different flags depending on
# if we are using g77 or gfortran, so pass this to the Makefile.
#=================================================================

AM_CONDITIONAL([HAVE_G77], [test `expr "${F77}" : '.*g77'` != "0"])

#=================================================================
# Add prefix, THIRDPARTY, and /opt/afids_support for pkgconfig file

PKG_PROG_PKG_CONFIG

if test "x$THIRDPARTY" = x; then
  pkg_extra_path=$prefix/lib/pkgconfig:/opt/afids_support/lib/pkgconfig
else
  pkg_extra_path=$prefix/lib/pkgconfig:$THIRDPARTY/lib/pkgconfig:/opt/afids_support/lib/pkgconfig
fi
if test "x$PKG_CONFIG_PATH" = x; then
  PKG_CONFIG_PATH=$pkg_extra_path
else
  PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$pkg_extra_path
fi
export PKG_CONFIG_PATH

AC_SUBST([pkgconfigdir], [${libdir}/pkgconfig])

])
