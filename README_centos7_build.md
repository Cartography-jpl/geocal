# Centos 7 Build

This gives explicit directions on building on a bare Centos 7 system.

## Get geocal

You can get a copy of geocal from github. You can either use the git/ssh
protocol or https:

    git clone https://github.jpl.nasa.gov/Cartography/geocal.git geocal

## Dependencies from packages

There are various dependencies that can be loaded from standard yum packages:

    sudo yum install boost-devel wget gsl-devel gdal gdal-devel doxygen suitesparse-devel swig zlib-devel bzip2-devel sqlite sqlite-devel openssl-devel fftw-devel

Note gdal-devel fails to install a bunch of other development packages, so
we need to install these. Not sure why it does this, normally dependencies
are installed. Perhaps a future gdal-devel will fix this. But for now, follow
up with:

    sudo yum install armadillo-devel poppler-devel libxml2-devel libgeotiff-devel mariadb-devel freexl-devel geos-devel libwebp-devel xerces-c-devel openjpeg2-devel jasper-devel netcdf-devel ogdi-devel giflib-devel libgta-devel cfitsio-devel postgresql-devel libdap-devel CharLS-devel hdf-devel hdf5-devel g2clib-devel

## Build blitz

We need to build blitz directly, it doesn't have an easy to find package. Get
it by:

    wget https://sourceforge.net/projects/blitz/files/latest/download -O blitz-0.10.tar.gz

Or you can also just download directly from web site. Then build by:

   tar -xf blitz-0.10.tar.gz
   cd blitz-0.10
   ./configure --prefix=`pwd`/../install --enable-shared
   make -j 20 all && make install
   cd ..

Note you should replace -j 20 with something making sense on your machine.

## Build SPICE

We have a copy of JPL CSPICE library with build scripts added to create a
shared library. You can get this by:

   git clone https://github.jpl.nasa.gov/Cartography/spice.git spice

Or you can also just grab tar file from github if you prefer to cloning. Then
build with:

   cd spice
   ./configure --prefix=`pwd`/../install
   make -j 20 all && make install
   cd ..

## Build python 3.5

Python 3.5 is too new to have a package for centos7, so we can build directly

    wget https://www.python.org/ftp/python/3.5.2/Python-3.5.2.tgz
    tar -xf Python-3.5.2.tgz
    ./configure --prefix=`pwd`/../install
    make -j 20 all && make install
    cd ..

Then add the new python to you path:

   export PATH=`pwd`/install/bin:${PATH}
   export LD_LIBRARY_PATH=`pwd`/install/lib:${LD_LIBRARY_PATH}
   cd install/bin
   ln -s ./python3 python
   ln -s ./python3-config python-config
   cd ../..


## Install python packages

Install python packages needed

    pip3 install -r geocal/requirements.txt


## Build VICAR support

Not strictly required, but not a lot of effort to add VICAR support and a
number of unit tests use VICAR format. There are two libraries, the VICAR
runtime library and a separate VICAR GDAL plugin to give GDAL VICAR support:

    git clone https://github.jpl.nasa.gov/Cartography/vicar_rtl.git vicar_rtl
    cd vicar_rtl/
    ./configure --prefix=`pwd`/../install
    make -j 20 all && make install
    cd ..
    git clone https://github.jpl.nasa.gov/Cartography/vicar_gdalplugin.git vicar_gdalplugin
    ./configure --prefix=`pwd`/../install
    make -j 20 all && make install
    cd ..

## Build geocal

We can now build geocal. I prefer building this in it own build directory
separate from the source. Not strictly required, but this is nice for
development.

    mkdir geocal-build
    cd geocal-build
    ../geocal/configure --prefix=`pwd`/../install
    make -j 20 all && make -j 20 check && make install && make -j 20 installcheck
