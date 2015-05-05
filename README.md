# GeoCal
This provides software for map projection, geometric calibration, 
simultaneous bundle adjustment, etc.

The software is made up of a core C++ library, python wrappers that allows 
the library to be used through python, and a number of support executables. 
Most programs are written in python.

## Building

This uses the standard autotools build (the configure/make cycle common
to many software packages). Generic instructions can be be found in
the [INSTALL](INSTALL) file.

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