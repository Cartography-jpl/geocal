// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "raster_image_multi_band_variable.h"
%}

%geocal_shared_ptr(RasterImageMultiBandVariable);

namespace GeoCal {
class RasterImageMultiBandVariable: public RasterImageMultiBand {
public:
  %rename(raster_image) raster_image_ptr;
  RasterImageMultiBandVariable();
  RasterImageMultiBandVariable
  (const std::vector<boost::shared_ptr<RasterImage> >& Raster_image);
  void add_raster_image(const boost::shared_ptr<RasterImage>& Ri);
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band);
protected:
  std::vector<boost::shared_ptr<RasterImage> > raster_image_;
};
}
