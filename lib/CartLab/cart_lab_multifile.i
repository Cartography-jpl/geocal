// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "cart_lab_multifile.h"
%}

%base_import(raster_multifile)

%geocal_shared_ptr(GeoCal::CartLabMultifile);
%geocal_shared_ptr(GeoCal::GdalCartLabMultifile);
%geocal_shared_ptr(GeoCal::VicarCartLabMultifile);
namespace GeoCal {
%nodefaultctor CartLabMultifile;
class CartLabMultifile: public RasterMultifile {
public:
  void create_subset_file
  (const std::string& Oname, const std::string& Driver,
   const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt, 
   const std::string& Options = "",   
   int boundary = 0) const;
  %python_attribute(directory_base, std::string);
  %pickle_serialization();
};

%nodefaultctor GdalCartLabMultifile;
class GdalCartLabMultifile: public CartLabMultifile {
public:
  %pickle_serialization();
};

%nodefaultctor VicarCartLabMultifile;
class VicarCartLabMultifile: public CartLabMultifile {
public:
  %pickle_serialization();
};

}
