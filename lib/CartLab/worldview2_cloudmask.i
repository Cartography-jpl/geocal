// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "worldview2_cloudmask.h"
#include "raster_image_multi_band_variable.h"
#include "image_ground_connection.h"
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
  %pickle_serialization();
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
// List of things "import *" will include
%python_export("WorldView2CloudMask")
