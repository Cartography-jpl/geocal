dnl Process this file with autoconf to produce a configure script.

AC_INIT(afids-python, 1.05)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_FILES([script/setup_afids_python.csh script/setup_afids_python.sh])
AC_CONFIG_FILES([lib/GeoCalCore/Support/global_fixture_default.cc])
# We have the top directory for GeoCal as a variable. This allows the
# full AFIDS system to include this in a subdirectory. For this standalone
# build, this is just "." relative to srcdir.
geocal_topdir=.

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
AC_AFIDS_DATA(optional, cannot_build, default_search)

#=================================================================
# Various thirdparty libraries.

AC_BOOST(required, cannot_build, default_search)
AC_BLITZ(required, cannot_build, default_search)
AC_GSL(required, cannot_build, default_search)
# We'll make this optional at some point, but for now this is still required
AC_SPICE(required, cannot_build, default_search)
AC_HDF5(optional, cannot_build, default_search)
AC_DOXYGEN(optional, cannot_build, default_search)
AC_FFTW(optional, cannot_build, default_search)
# We might make this optional at some point, but right now a lot of
# unit tests depend on this. We could switch to having most tests use
# VicarLite files, but we won't do that yet.
AC_GDAL(required, cannot_build, default_search)
AC_VICAR_GDALPLUGIN(optional, cannot_build, default_search)

#=================================================================
# We have some unit tests that make use of vicarb if it is available.
# It is fine if it isn't, we just skip these tests.
AC_AFIDS(optional, cannot_build, default_search)

#=================================================================
# Need to find carto library. (Need to have this after VICAR_RTL 
# and GSL, because it uses variables from those functions)
AC_CARTO(optional, cannot_build, default_search)

#=================================================================
# Build or find python

AFIDS_PYTHON(required, can_build, default_search)

#=================================================================
# Location of all the source code directories
GEOCAL_SOURCE_DIRECTORY

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
# Option to bypass running doxygen to update documentation. This is useful
# for a build environment that is used for development, but you'll want all
# of your "real" build environments to also update the documentation.
AC_ARG_WITH([documentation],
           [AS_HELP_STRING([--without-documentation],
             [Bypass running doxygen to update documentation. This is useful for a build environment that is used for development, but you'll want all of your "real" build environments to also update the documentation.])],
           [],
           [with_documentation=yes])
if test "$have_doxygen" != "yes"; then
   with_documentation=no
fi
AM_CONDITIONAL([WITH_DOCUMENTATION], [test x$with_documentation = xyes])

#=================================================================
# Option to include "AFIDS configuration A" stuff.
AFIDS_A_OPTION
if test x$with_afids_a = xyes; then
  AC_DEFINE(INCLUDE_AFIDS_A)
fi

if test "$have_swig" = "yes" -a "$USE_MAINTAINER_MODE" != "no"; then
  generate_swig="yes"
else
  generate_swig="no" 
fi

if test "$generate_swig" = "yes" -a "$have_doxygen" != "yes"; then
     AC_MSG_ERROR([
GeoCal requires doxygen if you are also generating SWIG python bindings.
You can try specifying the location using --with-doxygen if configure
couldn't find this. Or you can turn off the use of swig using 
--without-swig.])
fi
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
LOC_MSG([  Have AFIDS Data:               ${have_afids_data}])
LOC_MSG([  Have AFIDS:                    ${have_afids}])
LOC_MSG([  Have Carto library:            ${have_carto}])
LOC_MSG([  Have FFTW:                     ${have_fftw}])
LOC_MSG([  Have HDF5:                     ${have_hdf5}])
LOC_MSG([  Have SPICE:                    ${have_spice}])
LOC_MSG([  Have VICAR GDAL Plugin:        ${have_vicar_gdalplugin}])
LOC_MSG([  Have VICAR Run Time Library:   ${have_vicar_rtl}])
LOC_MSG([  Include AFIDS A configuration: ${with_afids_a}])
