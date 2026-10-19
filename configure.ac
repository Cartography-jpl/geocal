dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([geocal],[2.0])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIRS([config/m4 config/m4_local])
AC_CONFIG_FILES([script/setup_afids_python.csh script/setup_afids_python.sh])
AC_CONFIG_FILES([script/setup_geocal.csh script/setup_geocal.sh])
AC_CONFIG_FILES([script/conda_wrapper.sh])
AC_CONFIG_FILES([script/setup_afids_env.csh script/setup_afids_env.sh script/vicarb])
AC_CONFIG_FILES([lib/GeoCalCore/Support/global_fixture_default.cc])
AC_CONFIG_FILES([geocal.pc])
AC_CONFIG_HEADERS([lib/GeoCalCore/Support/geocal_config.h lib/GeoCalCore/Support/geocal_external_config.h])
# We have the top directory for GeoCal as a variable. This allows the
# full AFIDS system to include this in a subdirectory. For this standalone
# build, this is just "." relative to srcdir.
geocal_topdir=.

AFIDS_COMMON
AX_WITH_CURSES

AC_ARG_VAR([THIRDPARTY], [
If you have a central location not already searched for third party
software, you can specify THIRDPARTY=<directory>. This gets added to
all the third party library checks.
])

#=================================================================
# Need to find VICAR RTL library
AC_VICAR_RTL(optional)

#=================================================================
# Need to have AFIDS data
AC_AFIDS_DATA(optional)

#=================================================================
# MSPI-Shared library. We can only use MSPI-Shared if we also have
# HDFEOS5 which is depends on.
AC_HDFEOS5(optional)

AC_MSPI_SHARED(optional)

#=================================================================
# Mensuration Service Program (MSP) from BAE. Used only for
# test validation.
AC_MSP(optional)

#=================================================================
# Various thirdparty libraries.

# There is a a series of bugs in older version of the boost serialization
# library. So we require at least 1.46. This is the version we currently
# distribute with afids.
AC_BOOST(required, 1.46.0)
AC_BLITZ(required)
AC_GSL(required)
# We'll make this optional at some point, but for now this is still required
AC_SPICE(required)
AC_HDF5(optional)
AC_DOXYGEN(optional)
AC_FFTW(optional)
# We might make this optional at some point, but right now a lot of
# unit tests depend on this. We could switch to having most tests use
# VicarLite files, but we won't do that yet.
AC_GDAL(required)
AC_VICAR_GDALPLUGIN(optional)
AC_GEOTIFF(required)

#=================================================================
# Have a little bit of code that depends on having both GDAL
# and VICAR available, so add a test for that

if test "$have_vicar_rtl" = "yes" -a "$have_gdal" = "yes"; then
   AC_DEFINE(HAVE_GDAL_VICAR_RTL,,[Defined if we have GDAL and VICAR RTL])
fi

#=================================================================
# We have some unit tests that make use of vicarb if it is available.
# It is fine if it isn't, we just skip these tests.
AC_AFIDS(optional)

#=================================================================
# Need to find carto library. (Need to have this after VICAR_RTL 
# and GSL, because it uses variables from those functions)
AC_CARTO(optional)

#=================================================================
# Build or find python

AFIDS_PYTHON(required)

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

have_swig=no
AX_PKG_SWIG([], [have_swig=yes], [AC_MSG_WARN([cannot find 'swig' program. You should look at http://www.swig.org])])
AM_CONDITIONAL([HAVE_SWIG], [test "$have_swig" = "yes"])
AX_SWIG_ENABLE_CXX
AX_SWIG_PYTHON
# swig_rules.am uses the older name for this variable
AC_SUBST([SWIG_PYTHON_OPT], [$AX_SWIG_PYTHON_OPT])

#=================================================================
# We have a small amount of code that gets different flags depending on
# if we are using g77 or gfortran, so pass this to the Makefile.
# NOTE: geocal itself doesn't use these (or AM_FFLAGS in common.am), but
# the shared autotools files are used by other repositories that do have
# Fortran code. Keep them.
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
LT_LIB_M
LIBS="$LIBS $LIBM"
AC_CHECK_FUNCS(sincos)

#=================================================================
# Build does a few extra things to support a conda install -
# add a conda_wrapper.sh script, modify shebangs so vicar procs are
# callable without activating conda.

AC_ARG_WITH([conda-install-support],
	     [AS_HELP_STRING([--with-conda-install-support],
	     [extra build support for doing a conda install])],
	     [with_conda_install_support=yes],
	     [with_conda_install_support=no])
AM_CONDITIONAL([WITH_CONDA_INSTALL_SUPPORT], [ test "$with_conda_install_support" = "yes" ])
	     
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
AM_CONDITIONAL([HAVE_BOOST_SERIALIZATION], [ test "$with_boost_serialization" = "yes" ])

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

#=================================================================
# Location of ISIS. 
# This option here is used to add a specification to the setup files to
# set these to particular values.
AC_ARG_WITH([isis-root],
AS_HELP_STRING([--with-isis-root@<:@=DIR@:>@], [give directory where ISIS executables can be found]), [ ac_isis_root="$withval" ], [ ac_isis_root="/opt/isis"] )
AC_SUBST([isisroot], ["$ac_isis_root"])

AC_ARG_WITH([isis-data],
AS_HELP_STRING([--with-isis-data@<:@=DIR@:>@], [give directory where ISIS data can be found]), [ ac_isis_data="$withval" ], [ ac_isis_data="/opt/afids/data/isisdata"] )
AC_SUBST([isisdata], ["$ac_isis_data"])

#=================================================================
# Location of Copernicus data

AC_ARG_WITH([copernicus-dem],
AS_HELP_STRING([--with-copernicus-dem@<:@=DIR@:>@], [give directory where Copernicus DEM can be found]), [ ac_copernicus_dem="$withval" ], [ ac_copernicus_dem="/bigdata/tllogan30/copernicusDEM/copdem_v1_L2_tif" ] )
AC_SUBST([copernicusdem], ["$ac_copernicus_dem"])

AC_ARG_WITH([copernicus-lwm],
AS_HELP_STRING([--with-copernicus-lwm@<:@=DIR@:>@], [give directory where Copernicus LWM can be found]), [ ac_copernicus_lwm="$withval" ], [ ac_copernicus_lwm="/bigdata/tllogan30/copernicusDEM/coplwm_v1_L2_tif" ] )
AC_SUBST([copernicuslwm], ["$ac_copernicus_lwm"])

#=================================================================
# Put in setup_afids_env.csh/setup_afids_env.sh and vicarb that
# uses the newer version of things in geocal

AM_CONDITIONAL([GEOCAL_OVERRIDE_AFIDS], [test x$have_afids = xyes -a $AFIDS_PREFIX != $prefix])

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
LOC_MSG([  Conda install support:         ${with_conda_install_support}])
LOC_MSG([  Include boost serialization:   ${with_boost_serialization}])
LOC_MSG([  Have AFIDS Data:               ${have_afids_data}])
LOC_MSG([  Have AFIDS:                    ${have_afids}])
LOC_MSG([  Have Carto library:            ${have_carto}])
LOC_MSG([  Have FFTW:                     ${have_fftw}])
LOC_MSG([  Have HDF5:                     ${have_hdf5}])
LOC_MSG([  Have HDFEOS5:                  ${have_hdfeos5}])
LOC_MSG([  Have MSPI-Shared:              ${have_mspi_shared}])
LOC_MSG([  Have MSP:                      ${have_msp}])
LOC_MSG([  Have SPICE:                    ${have_spice}])
LOC_MSG([  Have VICAR GDAL Plugin:        ${have_vicar_gdalplugin}])
LOC_MSG([  Have VICAR Run Time Library:   ${have_vicar_rtl}])
