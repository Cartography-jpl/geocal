// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "worldview2_cloudmask.h"
%}

%geocal_shared_ptr(WorldView2CloudMask);

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
