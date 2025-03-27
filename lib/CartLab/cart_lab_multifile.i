// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "cart_lab_multifile.h"
#include "image_ground_connection.h"
%}

%base_import(raster_multifile)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::CartLabMultifile);
%geocal_shared_ptr(GeoCal::GdalCartLabMultifile);
%geocal_shared_ptr(GeoCal::VicarCartLabMultifile);
%geocal_shared_ptr(GeoCal::VicarCartLabMultifileSetup);
namespace GeoCal {
%nodefaultctor CartLabMultifile;
class CartLabMultifile: public RasterMultifile {
public:
  void create_subset_file
  (const std::string& Oname, const std::string& Driver,
   const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
   const boost::shared_ptr<MapInfo>& Desired_map_info=
   boost::shared_ptr<MapInfo>(),
   const std::string& Translate_arg = "",
   const std::string& Options = "",   
   int boundary = 0, bool Verbose = false) const;
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

class VicarCartLabMultifileSetup: public CartLabMultifile {
public:
  VicarCartLabMultifileSetup(const boost::shared_ptr<MapInfo> Minfo,
			     int Number_line_per_tile = 100,
			     int Number_sample_per_tile = -1, 
			     int Number_tile_each_file = 4,
			     int Number_tile = 4,
			     bool Favor_memory_mapped = true,
			     bool No_coverage_is_error = true, 
			     int No_coverage_fill_value = -1,
			     bool Force_area_pixel = false);
  void file_add(int x1, int y1, int x2, int y2, const std::string& fname);
  %pickle_serialization();
};
  
}

// List of things "import *" will include
%python_export("CartLabMultifile", "GdalCartLabMultifile", "VicarCartLabMultifile",
	       "VicarCartLabMultifileSetup")
