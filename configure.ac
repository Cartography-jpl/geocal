dnl Process this file with autoconf to produce a configure script.

AC_INIT(geocal, 1.18)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_FILES([script/setup_afids_python.csh script/setup_afids_python.sh])
AC_CONFIG_FILES([lib/GeoCalCore/Support/global_fixture_default.cc])
AC_CONFIG_HEADERS([lib/GeoCalCore/Support/geocal_config.h lib/GeoCalCore/Support/geocal_internal_config.h])
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
AC_VICAR_RTL(optional, cannot_build, default_search)

#=================================================================
# Need to have AFIDS data
AC_AFIDS_DATA(optional, cannot_build, default_search)

#=================================================================
# MSPI-Shared library. We can only use MSPI-Shared if we also have
# HDFEOS5 which is depends on.
AC_HDFEOS5(optional, cannot_build, default_search)

AC_MSPI_SHARED(optional, cannot_build, default_search)

#=================================================================
# Various thirdparty libraries.

# There is a a series of bugs in older version of the boost serialization
# library. So we require at least 1.46. This is the version we currently
# distribute with afids.
AC_BOOST(required, can_build, default_search, 1.46.0)
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
# Have a little bit of code that depends on having both GDAL
# and VICAR available, so add a test for that

AM_CONDITIONAL([HAVE_GDAL_VICAR_RTL], [test "$have_vicar_rtl" = "yes" -a "$have_gdal" = "yes"])


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

AC_PROG_SWIG(3.0.7)
SWIG_ENABLE_CXX
SWIG_PYTHON

#=================================================================
# We have a small amount of code that gets different flags depending on
# if we are using g77 or gfortran, so pass this to the Makefile.
#=================================================================

AM_CONDITIONAL([HAVE_G77], [test `expr "${F77}" : '.*g77'` != "0"])
AM_CONDITIONAL([HAVE_GFORTRAN], [test `expr "${F77}" : '.*gfortran'` != "0"])

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
# Turn off use of boost serialization of desired. Don't have a use
# case for this yet, but it is easier to put in and not use than
# to not have it and find we need it.

AC_ARG_WITH([boost-serialization],
            [AS_HELP_STRING([--without-boost-serialization],
	      [Do not include code to support doing boost serialization with GeoCal objects])],
	      [with_boost_serialization=no],
              [with_boost_serialization=yes])
if test "$with_boost_serialization" = "yes"; then
    AC_DEFINE(GEOCAL_HAVE_BOOST_SERIALIZATION,,[Defined if we have boost serialization])
fi

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

AM_CONDITIONAL([BUILD_AFIDS], [false])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

#======================================================================
# Give a nice log message describing what has been configured, and which
# optional libraries are going to be used.
#======================================================================

LOC_MSG()
LOC_MSG([GeoCal is now configured])
LOC_MSG()
LOC_MSG([  Installation directory:        ${prefix}])
LOC_MSG([  Build debug version:           ${enable_debug}])
LOC_MSG([  Fortran compiler:              ${F77} ${FFLAGS}])
LOC_MSG([  C compiler:                    ${CC} ${CFLAGS}])
LOC_MSG([  CXX compiler:                  ${CXX} ${CXXFLAGS}])
LOC_MSG()
LOC_MSG([  Generate SWIG wrappers:        ${generate_swig}])
LOC_MSG([  Install documentation:         ${with_documentation}])
LOC_MSG([  Include boost serialization:   ${with_boost_serialization}])
LOC_MSG([  Have AFIDS Data:               ${have_afids_data}])
LOC_MSG([  Have AFIDS:                    ${have_afids}])
LOC_MSG([  Have Carto library:            ${have_carto}])
LOC_MSG([  Have FFTW:                     ${have_fftw}])
LOC_MSG([  Have HDF5:                     ${have_hdf5}])
LOC_MSG([  Have HDFEOS5:                  ${have_hdfeos5}])
LOC_MSG([  Have MSPI-Shared:              ${have_mspi_shared}])
LOC_MSG([  Have SPICE:                    ${have_spice}])
LOC_MSG([  Have VICAR GDAL Plugin:        ${have_vicar_gdalplugin}])
LOC_MSG([  Have VICAR Run Time Library:   ${have_vicar_rtl}])
