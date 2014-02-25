# This is a simple makefile. It builds the picmtch4 software, which
# includes a small part of AFIDS/VICAR and the actual picmtch4 code.
# There is a unit test, run by "unit_tests"
#
# Note that parallel makes should work fine, so you can build with
# a "-j <number cpu>" to speed things up

# This makefile needs to be altered to link to OpenCV once it is installed on
# sreca or else it will not build as it has dependencies in that form
F77 = gfortran
CC = g++

# Pretty colors for output
red = $$(tput setaf 1)
green = $$(tput setaf 2)
default = $$(tput setaf 9)
HDFEOS5INC=/data/svn_workspace/mikeb/sreca/mspi/hdfeos5/hdfeos5/include
HDFEOS5LIB=/data/svn_workspace/mikeb/sreca/mspi/hdfeos5/hdfeos5/hdfeos5/lib
INHOUSE =/data/svn_workspace/plummer/mspi/MSPI-Ground/refactPicmtch4/InHouse/
HEADERS=picmtch4.h MspiPcorrMatcher.h HDFEOS5Image.h ImageVis.h ImageOps.h AffineWarp.h

all: libpicmtch4.so runpicmtch4

# We copy the test data to the working directory because it gets updated
# by the unit test

clean:
	-rm runpicmtch4
	-rm autothresh tstimg1 tstimg2 tstimg2_rotate tstimg3 tstimg4 xxa
	-rm test.out
	-rm libpicmtch4.so
	-rm picmtch4.o
	-rm MspiPcorrMatcher.o
	-rm hdfeos5image.o
	-rm ImageOps.o
	-rm ImageVis.o
	-rm AffineWarp.o
	-rm unit_tests

really_clean: clean
	-rm -r afids_install

unit_tests: unit_tests.cpp libpicmtch4.so
	libtool --mode=link $(CC) -I afids_install/include $< -o $@ \
	 -I $(INHOUSE) \
	 -I $(HDFEOS5INC) \
         -Wl,-rpath -Wl,afids_install/lib \
         -Lafids_install/lib \
	 -lpicmtch4 -lboost_unit_test_framework \
	 -L$(HDFEOS5LIB) $(HDFEOS5LIB)/libhe5_hdfeos.la -lGctp -lhdf5
	 -rm *.hdf &> /dev/null
	 cp test_data/*.hdf .	
	 ./unit_tests
	 -rm  tstimg1.hdf tstimg2.hdf tstimg3.hdf tstimg4.hdf tstimg2_rotate.hdf

libpicmtch4.so: MspiPcorrMatcher.o picmtch4.o hdfeos5image.o ImageVis.o ImageOps.o AffineWarp.o afids_install/bin/vicar	
	 $(CC) -shared MspiPcorrMatcher.o picmtch4.o hdfeos5image.o ImageVis.o -o ImageOps.o AffineWarp.o $@ \
	 -Wl,-rpath -Wl,afids_install/lib \
	 -Lafids_install/lib \
	 -lfftw3 -lcarto
	 -cp $@ afids_install/lib

picmtch4.o: MspiPcorrMatcher.cpp
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

hdfeos5image.o: HDFEOS5Image.cpp
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

MspiPcorrMatcher.o: picmtch4.cpp ImageVis.o ImageOps.o AffineWarp.o
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

ImageVis.o: ImageOps.cpp
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

ImageOps.o: ImageVis.cpp
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

AffineWarp.o: ImageVis.o ImageOps.o
	$(CC) -I $(HDFEOS5INC) -I $(INHOUSE) -fpic -c -I afids_install/include $< -o $@

runpicmtch4: runImageMatcher.cpp libpicmtch4.so
	libtool --mode=link $(CC) -I afids_install/include \
	   -I $(HDFEOS5INC) \
	   -I $(INHOUSE) \
	   -I /usr/include/python2.7 \
	   $< -o $@ \
           -Wl,-rpath -Wl,afids_install/lib \
           -Lafids_install/lib \
	   -lpicmtch4 \
	   -lboost_python -lboost_filesystem -lboost_system\
	   -lpython2.7 \
	   -L$(HDFEOS5LIB) $(HDFEOS5LIB)/libhe5_hdfeos.la -lGctp -lhdf5

afids_install/bin/vicar: afids_matching-1.0.tar.gz
	-rm -r afids_matching-1.0
	-rm -r afids_install
	tar -xf $<
	pre=`pwd` && cd afids_matching-1.0 && \
          export F77=$(F77) && \
          ./configure --prefix=$${pre}/afids_install && \
          $(MAKE) all && $(MAKE) -j 1 install
	-rm -r afids_matching-1.0

.PHONY: all
.PHONY: check
.PHONY: clean
.PHONY: really_clean
.PHONY: unit_tests