// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ground_mask_image.h"
%}
%base_import(ground_mask)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::GroundMaskImage);

namespace GeoCal {
class GroundMaskImage : public GroundMask {
public:
  GroundMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0);
  %python_attribute2(raster_image, raster_image_ptr, 
		     boost::shared_ptr<RasterImage>)
  %python_attribute(masked_value, int)
  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;
  %pickle_init(1, self.raster_image, self.masked_value)
};
}