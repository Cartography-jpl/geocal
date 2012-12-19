Summary: This is the AFIDS python
Name: afids-python
Version: r1262
Release: 1
License: Copyright 2012 California Institute of Technology ALL RIGHTS RESERVED
Group: Applications/Engineering
Vendor: California Institute of Technology
URL: http://www-mipl.jpl.nasa.gov/cartlab/cartlab.html
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: python27 numpy27 scipy27 matplotlib27 nose27 h5py27 sphinx27 afids-data
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
/opt/afids/afids/vdev/sample_vicar_python
/opt/afids/afids/vdev/sample_vicar_python.pdf
/opt/afids/afids/bin/create_gdal_igc
/opt/afids/afids/bin/gdal_to_erdas
/opt/afids/afids/bin/gdal_to_nitf
/opt/afids/afids/bin/gen_geneplot_cmd.py
/opt/afids/afids/bin/igc_project
/opt/afids/afids/bin/sba
/opt/afids/afids/bin/setup_dem_job
/opt/afids/afids/bin/shelve_dir
/opt/afids/afids/bin/shelve_job_run
/opt/afids/afids/bin/tp_collect
/opt/afids/afids/include/geocal/*
/opt/afids/afids/include/sdbx/*
/opt/afids/afids/include/libraw*.h
/opt/afids/share/doc/afids/*
/opt/afids/share/doc/geocal/*
/opt/afids/share/doc/libsdbx/*

%changelog
* Tue Dec 18 2012 Mike M Smyth <smyth@pistol> - python-1
- Initial build.

