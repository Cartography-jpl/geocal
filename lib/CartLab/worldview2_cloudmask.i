// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "worldview2_cloudmask.h"
#include "raster_image_multi_band_variable.h"
%}
%base_import(calc_raster)
%import "raster_image_multi_band.i"
%geocal_shared_ptr(GeoCal::WorldView2CloudMask);

namespace GeoCal {
class WorldView2CloudMask : public CalcRaster {
public:
  enum {CLEAR = 0, SHADOW = 1, CLOUDY = 2};
  WorldView2CloudMask(const RasterImageMultiBand&
		      Mul_spec_data, int Min_cloud_size,
		      double Cloud_threshold, double Shadow_threshold);
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
