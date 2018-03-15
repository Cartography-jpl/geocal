// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  %rename(_v_create_subset_file) create_subset_file;
  void create_subset_file
  (const std::string& Oname, const std::string& Driver,
   const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
   const boost::shared_ptr<MapInfo>& Desired_map_info,
   const std::string& Translate_arg = "",
   const std::string& Options = "",   
   int boundary = 0) const;
  %python_attribute(directory_base, std::string);
  %pickle_serialization();
%pythoncode {
  def create_subset_file(self, Oname, Driver, Pt_list = [], Desired_map_info = None, Translate_arg = "", Options = "", Boundary = 0):
    if(isinstance(Pt_list, geocal_swig.Vector_GroundCoordinate)):
        t = Pt_list
    else:
        t = geocal_swig.Vector_GroundCoordinate()
        for p in Pt_list:
            t.push_back(p)
    return self._v_create_subset_file(Oname, Driver, t, Desired_map_info, Translate_arg,Options, Boundary)
}
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

// List of things "import *" will include
%python_export("CartLabMultifile", "GdalCartLabMultifile", "VicarCartLabMultifile")
