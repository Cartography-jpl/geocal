// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "calc_map_projected.h"
%}

%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::CalcMapProjected);

namespace GeoCal {
%nodefaultctor CalcMapProjected;

class CalcMapProjected : public RasterImageVariable {
public:
  void write_image(RasterImage& Out, int Grid_spacing = 1) const;
  void write_multiple(const 
     std::vector<boost::shared_ptr<RasterImage> >& Out) const;
  void write_multiple(const 
    std::vector<boost::shared_ptr<RasterImage> >& Out, int Grid_spacing)
    const;
};
}
