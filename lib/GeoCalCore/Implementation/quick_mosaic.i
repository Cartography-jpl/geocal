// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "quick_mosaic.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image)
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::QuickMosaic);
namespace GeoCal {
class QuickMosaic : public CalcRaster {
public:
  QuickMosaic(const MapInfo& Mi,
	      const std::vector<boost::shared_ptr<RasterImage> > &Img_list,
	      int Number_tile_line = -1,
	      int Number_tile_sample = -1,
	      double Fill_value = 0.0
	      );
  %python_attribute(fill_value, double)
  %python_attribute(image_list, std::vector<boost::shared_ptr<RasterImage> >)
  %pickle_serialization()
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
// List of things "import *" will include
%python_export("QuickMosaic")
