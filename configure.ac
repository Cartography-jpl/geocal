dnl Process this file with autoconf to produce a configure script.

AC_INIT(afids-python, 1.03)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_FILES([afids_python/script/setup_afids_python.csh afids_python/script/setup_afids_python.sh])
AC_CONFIG_FILES([afids_python/geocal/lib/GeoCalCore/Support/global_fixture_default.cc])
AFIDS_COMMON
MP_WITH_CURSES

AC_ARG_VAR([THIRDPARTY], [
It is common that either 1) you want to build a local copy of all
thirdparty software or 2) you have a central location not already
searched for that you want to be searched. You can specify
THIRDPARTY=build or THIRDPARTY=<directory>. This gets added to all
the third party library checks.
])

#=================================================================
# Need to find VICAR RTL library
AC_VICAR_RTL(required, cannot_build, default_search)

#=================================================================
# Need to have AFIDS data
AC_AFIDS_DATA(required, cannot_build, default_search)

#=================================================================
# Various thirdparty libraries.

AC_BOOST(required, cannot_build, default_search)
AC_SPICE(required, cannot_build, default_search)
AC_BLITZ(required, cannot_build, default_search)
AC_GSL(required, cannot_build, default_search)
AC_HDF5(required, cannot_build, default_search)
AC_DOXYGEN(required, cannot_build, default_search)
AC_FFTW(required, cannot_build, default_search)
AC_GDAL(required, cannot_build, default_search)
AC_VICAR_GDALPLUGIN(required, cannot_build, default_search)

#=================================================================
# Right now, always include MSPI. We may make this optional later

# Not actually ready for this, so turn off for now
#with_mspi=yes
with_mspi=no
AM_CONDITIONAL([BUILD_MSPI], [ test x$with_mspi = xyes ])
AM_CONDITIONAL([HAVE_MSPI], [ test x$with_mspi = xyes ])

#=================================================================
# Libraries needed by MSPI.
if test x$with_mspi = xyes; then
   AC_HDFEOS(required, cannot_build, default_search)
   AC_HDFEOS5(required, cannot_build, default_search)
else
   AM_CONDITIONAL([HAVE_HDFEOS], [false])
   AM_CONDITIONAL([BUILD_HDFEOS], [false])
   AM_CONDITIONAL([HAVE_HDFEOS5], [false])
   AM_CONDITIONAL([BUILD_HDFEOS5], [false])
   AM_CONDITIONAL([HAVE_HDF4], [false])
   AM_CONDITIONAL([BUILD_HDF4], [false])
fi

#=================================================================
# Need to find carto library. (Need to have this after VICAR_RTL 
# and GSL, because it uses variables from those functions)
AC_CARTO(required, cannot_build, default_search)

#=================================================================
# Build or find python

AFIDS_PYTHON(required, can_build, default_search)

#=================================================================
# Option to bypass running SWIG to update SWIG wrappers. The SWIG
# wrappers can take a bit of time to create and compile, so if you
# aren't working directly with them it can be useful to turn off during
# development.
AC_ARG_WITH([swig],
           [AS_HELP_STRING([--without-swig],
             [Bypass running SWIG to update SWIG wrappers. The SWIG wrappers can take a bit of time to create and compile, so if you aren't working directly with them it can be useful to turn off during development.])],
           [SWIG=/fake_so_fail],
           [])

AC_PROG_SWIG(2.0.4)
SWIG_ENABLE_CXX
SWIG_PYTHON

#=================================================================
# We have a small amount of code that gets different flags depending on
# if we are using g77 or gfortran, so pass this to the Makefile.
#=================================================================

AM_CONDITIONAL([HAVE_G77], [test `expr "${F77}" : '.*g77'` != "0"])

#=================================================================
# We have a few Make rules that need to be different on the Mac. This
# only affects external library builds - all of our code builds the 
# same on a Mac or Linux machine.
#=================================================================

AC_IS_MAC

#=================================================================
# Use sincos if it is available. This is on most Linux systems, but not on
# the mac.
#=================================================================
AC_CHECK_LIBM
LIBS="$LIBS $LIBM"
AC_CHECK_FUNCS(sincos)

#=================================================================
# Check for goocanvas.
# Doesn't work yet outside of my Mac.
# This is likely to go away, but leave this in place for now.
#=================================================================
#PKG_CHECK_MODULES([GTKGOO], [goocanvas], [have_gtkgoo=yes], [have_gtkgoo=no])
[have_gtkgoo=no]
AM_CONDITIONAL([HAVE_GTKGOO], [test x$have_gtkgoo = xyes])

#=================================================================
# Option to bypass running doxygen to update documentation. This is useful
# for a build environment that is used for development, but you'll want all
# of your "real" build environments to also update the documentation.
AC_ARG_WITH([documentation],
           [AS_HELP_STRING([--without-documentation],
             [Bypass running doxygen to update documentation. This is useful for a build environment that is used for development, but you'll want all of your "real" build environments to also update the documentation.])],
           [],
           [with_documentation=yes])
AM_CONDITIONAL([WITH_DOCUMENTATION], [test x$with_documentation = xyes])

#=================================================================
# Option to include "AFIDS configuration A" stuff.
AFIDS_A_OPTION
if test x$with_afids_a = xyes; then
  AC_DEFINE(INCLUDE_AFIDS_A)
fi

# This set up all the source directories. Source for this is found in
# config/m4/afids_source_directory.m4 so we can include this in multiple
# configuration files.
AFIDS_PYTHON_SOURCE_DIRECTORY

if test "$have_swig" = "yes" -a "$USE_MAINTAINER_MODE" != "no"; then
  generate_swig="yes"
else
  generate_swig="no" 
fi

AM_CONDITIONAL([BUILD_AFIDS_PYTHON], [true])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

#======================================================================
# Give a nice log message describing what has been configured, and which
# optional libraries are going to be used.
#======================================================================

LOC_MSG()
LOC_MSG([AFIDS is now configured])
LOC_MSG()
LOC_MSG([  Installation directory:        ${prefix}])
LOC_MSG([  Build debug version:           ${enable_debug}])
LOC_MSG([  Fortran compiler:              ${F77} ${FFLAGS}])
LOC_MSG([  C compiler:                    ${CC} ${CFLAGS}])
LOC_MSG([  CXX compiler:                  ${CXX} ${CXXFLAGS}])
LOC_MSG()
LOC_MSG([  Generate SWIG wrappers:        ${generate_swig}])
LOC_MSG([  Install documentation:         ${with_documentation}])
LOC_MSG([  Include AFIDS A configuration: ${with_afids_a}])
LOC_MSG([  Include MSPI:                  ${with_mspi}])
