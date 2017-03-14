# GeoCal
This provides software for map projection, geometric calibration, 
simultaneous bundle adjustment, etc.

The software is made up of a core C++ library, python wrappers that allows 
the library to be used through python, and a number of support executables. 
Most programs are written in python.

## Documentation

When you do a

    make install

we create doxygen documentation for the C++ code.
This will appear in the directory \<install prefix\>/share/doc/geocal.

The documentation can also be viewed from a version pushed into
[Github Pages](https://github.jpl.nasa.gov/pages/Cartography/geocal).
If you work on documentation, you can rebuild the version in Github using
the command

    make blah_blah_need_to_implement

We also generate python documentation for all of the python code. This
is available in for example in ipython using the normal "help(class)".

We will generate html documentation of geocal python "real soon now".

## Building

This uses the standard autotools build (the configure/make cycle common
to many software packages). Generic instructions can be be found in
the [INSTALL](INSTALL) file.

There are a number of dependencies for geocal (described below). As an
example, explicit directions for building on a bare Centos 7 can be found
in [Centos 7 Build](README_centos7_build.md)

A note on the build. We have unit tests run with the standard GNU target
"make check". There is also a standard GNU target "installcheck" that can
be used to run unit tests that require the install step first. However this
is less commonly known, so I wanted to point this out. The various python 
unit tests can only be tested after an install. So a full test cycle would
be:

    make -j 20 all
    make -j 20 check
    make install
    make -j 20 installcheck

Replace -j 20 with whatever is reasonable on your system. Also note that
the install step can't be done in parallel, again this is pretty standard
GNU build.

## Dependencies

The GeoCal library has a few required library dependencies, and a number
of optional libraries. The configure step will search for the optional
libraries, and in if found the code that depends on them will be included.
If not found, then that portion of the library will not be built. 

A summary of what will be built will be displayed at the end of the configure
step. For example:

```
pistol% ../geocal-repo/configure
  <lots of checking messages>

GeoCal is now configured

  Installation directory:        /home/smyth/Local/geocal-build/install
  Build debug version:           no
  Fortran compiler:               
  C compiler:                    gcc -O2 -g -march=native -Wall
  CXX compiler:                  g++ -O2 -g -march=native -Wall

  Generate SWIG wrappers:        yes
  Install documentation:         yes
  Include boost serialization:   yes
  Have AFIDS Data:               yes
  Have AFIDS:                    yes
  Have Carto library:            yes
  Have FFTW:                     yes
  Have HDF5:                     yes
  Have HDFEOS5:                  yes
  Have MSPI-Shared:              yes
  Have Python OGR Module:        yes
  Have SPICE:                    yes
  Have VICAR GDAL Plugin:        yes
  Have VICAR Run Time Library:   yes
```

The required libraries are 

* [Boost](http://www.boost.org) (need version >= 1.46) 
* [Blitz](http://sourceforge.net/projects/blitz/) (need >=0.9) 
* [GSL](http://www.gnu.org/software/gsl)
* [CSPICE](http://naif.jpl.nasa.gov/naif/toolkit.html)
* [GDAL](http://www.gdal.org), [python](https://www.python.org/)

The normal distribution of CSPICE is a static library. We require a shared
version of this library. A shared build of this is available in the package
[spice](https://github.jpl.nasa.gov/Cartography/spice), this is just the
normal JPL CSPICE but with build scripts added for making a shared library.

In addition, CSPICE depends on a number of spice kernels to actually work.
Again, we have this at [AFIDS](https://github.jpl.nasa.gov/Cartography/afids),
but contact Mike.M.Smyth@jpl.nasa.gov to give you just the data files. 

When using the CSPICE library, you'll need to set the environment variable
SPICEDATA to point to the location of the data, e.g.,:

    export SPICEDATA=/data/linux_ops/AIRMSPI/tools_03232015/install_fedora20/data/cspice

(This is not needed to build or run the unit tests).

The python version should be 2.7, or python >= 3.5. In addition to
python, you should have the libraries listed in requirements.txt. I
often install these in a virtual environment, which you can do if you
don't want to install these at the system level.

## Optional dependencies

There are other libraries that will be used if available.

* The [Doxygen](http://www.stack.nl/~dimitri/doxygen) tools will be used to
  make HTML documentation of the C++ library. This documentation is also
  available in python.

* The [SWIG](http://www.swig.org/) tool is used to create python wrappers.
  This is only needed if you are doing code development, if you just want to
   build the existing python wrappers SWIG is not necessary.

* The library 
  [VICAR GDAL Plugin](https://github.jpl.nasa.gov/Cartography/vicar_gdalplugin)
   adds support to GDAL for the VICAR file format.

* The library
  [VICAR RTL](https://github.jpl.nasa.gov/Cartography/vicar_rtl) gives support
  for reading and writing VICAR format files. Even without this library GeoCal
  has some basic support for reading VICAR file format.

* The AFIDS data directories can be used to supply various data sets such
   as the SRTM DEM. Contact Mike.M.Smyth@jpl.nasa.gov for details about this.

* The [HDF5](http://www.hdfgroup.org/HDF5) library can be used to directly
  read and write HDF 5. There is also already support in GDAL for using HDF 5
  (depending on the build options used for GDAL).

* The [FFTW](http://www.fftw.org) library can be used to do phase correlation
  image matching.

* The carto library is used for a few cartography lab classes. These are 
  primarily used for backwards testing, most people will not be interested
  in this code.

* The python OGR library is an optional installation of 
  [GDAL](http://www.gdal.org). If available, this can be used by code that
  works with for example ESRI Shape files.

* The [MSPI-Shared](https://github.jpl.nasa.gov/MSPI/MSPI-Shared) library
  can be used to support MSPI file format read and write classes.

