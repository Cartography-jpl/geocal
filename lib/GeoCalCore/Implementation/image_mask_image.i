// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "image_mask_image.h"
%}
%base_import(image_mask)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::ImageMaskImage);
namespace GeoCal {
class ImageMaskImage : public ImageMask {
public:
  enum OutsideHandling { OUTSIDE_MASKED, OUTSIDE_NOT_MASKED, OUTSIDE_ERROR };
  ImageMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0, 
		 OutsideHandling Handling = OUTSIDE_MASKED);
  virtual bool mask(int Line, int Sample) const;
  void unmasked_bounding_box(int &OUTPUT, int &OUTPUT, int &OUTPUT,
			     int &OUTPUT) const;
  %python_attribute2(raster_image, raster_image_ptr, 
		     boost::shared_ptr<RasterImage>)
  %python_attribute(masked_value, int)
  %python_attribute(outside_handling, OutsideHandling)
  %pickle_init(2, self.raster_image, self.masked_value, self.outside_handling)
};
}
