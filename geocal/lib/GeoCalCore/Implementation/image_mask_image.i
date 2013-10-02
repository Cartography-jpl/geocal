// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "image_mask_image.h"
%}
%base_import(image_mask)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::ImageMaskImage);
namespace GeoCal {
class ImageMaskImage : public ImageMask {
public:
  ImageMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0);
  virtual bool mask(int Line, int Sample) const;
  void unmasked_bounding_box(int &OUTPUT, int &OUTPUT, int &OUTPUT,
			     int &OUTPUT) const;
  %python_attribute2(raster_image, raster_image_ptr, 
		     boost::shared_ptr<RasterImage>)
  %python_attribute(masked_value, int)
  %pickle_init(1, self.raster_image, self.masked_value)
};
}
