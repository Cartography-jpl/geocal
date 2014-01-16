Summary: This is the AFIDS python
Name: afids-python
Version: 1.05
Release: 1
License: Copyright 2013 California Institute of Technology ALL RIGHTS RESERVED
Group: Applications/Engineering
Vendor: California Institute of Technology
URL: http://www-mipl.jpl.nasa.gov/cartlab/cartlab.html
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: python27 numpy27 scipy27 matplotlib27 nose27 h5py27 sphinx27 afids-data gdal19 vicar-gdalplugin opencv27 ipython27
Prefix: /opt/afids

%description

This is the AFIDS python bindings. This contains GeoCal along with a number
of python modules.

%prep
%setup -q

%build
./configure --prefix=/opt/afids --with-afids-a
make %_smp_mflags 

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/opt/afids/lib/libgeocal.la
rm -f $RPM_BUILD_ROOT/opt/afids/share/doc/afids/python/.doctrees/environment.pickle

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc
/opt/afids/lib/libgeocal.a
/opt/afids/lib/libgeocal.so
/opt/afids/lib/libgeocal.so.0
/opt/afids/lib/libgeocal.so.0.0.0
/opt/afids/lib/python2.7/site-packages/afids/*
/opt/afids/lib/python2.7/site-packages/geocal/*
/opt/afids/afids/vdev/call_sample_vicar_python.pdf
/opt/afids/afids/vdev/pair_ortho.pdf
/opt/afids/afids/vdev/pair_ortho2.pdf
/opt/afids/afids/vdev/sample_vicar_python
/opt/afids/afids/vdev/sample_vicar_python.pdf
/opt/afids/bin/abachd
/opt/afids/bin/convert_reflectance
/opt/afids/bin/dem_generate
/opt/afids/bin/doughnut_diff
/opt/afids/bin/gdal_to_erdas
/opt/afids/bin/gdal_to_nitf
/opt/afids/bin/gen_geneplot_cmd.py
/opt/afids/bin/gen_geneplot_cmd.pyc
/opt/afids/bin/gen_geneplot_cmd.pyo
/opt/afids/bin/height_to_color
/opt/afids/bin/igc_project
/opt/afids/bin/sba
/opt/afids/bin/shelve_airmspi_igc
/opt/afids/bin/shelve_dem
/opt/afids/bin/shelve_dir
/opt/afids/bin/shelve_igccol
/opt/afids/bin/shelve_image
/opt/afids/bin/shelve_job_run
/opt/afids/bin/shelve_show
/opt/afids/bin/tiepoint_to_error
/opt/afids/bin/tp_collect
/opt/afids/bin/tp_show
/opt/afids/bin/write_image
/opt/afids/include/geocal/*
/opt/afids/include/sdbx/*
/opt/afids/include/libraw*.h
/opt/afids/share/doc/afids/*
/opt/afids/share/doc/geocal/*
/opt/afids/share/doc/libsdbx/*
/opt/afids/setup_afids_python.csh
/opt/afids/setup_afids_python.sh

%changelog
* Fri Oct 11 2013 Mike M Smyth <smyth@pistol> - 1.05-1
- ABAChD alpha delivery. Numerous changes, but largest is adding
  abachd script along with support programs.

* Tue Sep 17 2013 Mike M Smyth <smyth@pistol> - 1.04-1
- Add pair_ortho2 for Tom, which is like pair_ortho but also generates a DEM.

* Fri Aug  25 2013 Mike M Smyth <smyth@pistol> - 1.03-1
- Proper handling of RPC A format in gdal_to_nitf, and better handling of 
  interpolation in DEM generation.

* Thu Jul 11 2013 Mike M Smyth <smyth@pistol> - 1.02-1
- Add pair_ortho proc for Tom and Rich

* Tue Dec 18 2012 Mike M Smyth <smyth@pistol> - python-1
- Initial build.

