// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ground_mask_image.h"
#include "image_ground_connection.h"
%}
%base_import(ground_mask)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::GroundMaskImage);

namespace GeoCal {
class GroundMaskImage : public GroundMask {
public:
  GroundMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0,
		  bool Outside_is_masked = false);
  %python_attribute2(raster_image, raster_image_ptr, 
		     boost::shared_ptr<RasterImage>)
  %python_attribute(masked_value, int)
  %python_attribute(outside_is_masked, bool)
  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("GroundMaskImage")
