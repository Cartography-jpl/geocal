// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "cart_lab_multifile.h"
%}

%base_import(raster_multifile)

%geocal_shared_ptr(GeoCal::CartLabMultifile);
%geocal_shared_ptr(GeoCal::GdalCartLabMultifile);
namespace GeoCal {
%nodefaultctor CartLabMultifile;
class CartLabMultifile: public RasterMultifile {
public:
  %python_attribute(directory_base, std::string);
  %pickle_serialization();
};

%nodefaultctor GdalCartLabMultifile;
class GdalCartLabMultifile: public CartLabMultifile {
public:
  %pickle_serialization();
};

}
