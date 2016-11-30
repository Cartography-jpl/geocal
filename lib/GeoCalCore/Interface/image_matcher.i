// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "image_matcher.h"
%}
%base_import(generic_object)
%import "raster_image.i"
%import "image_coordinate.i"
%import "image_mask.i"
%geocal_shared_ptr(GeoCal::ImageMatcher);
namespace GeoCal {
class ImageMatcher : public GenericObject {
public:
  virtual void match(const RasterImage& Ref, const RasterImage&
		     New, const ImageCoordinate& Ref_loc, const
		     ImageCoordinate& New_guess, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const;  
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const = 0;
  std::string print_to_string() const;
};
}


