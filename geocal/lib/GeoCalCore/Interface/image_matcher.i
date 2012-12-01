// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "image_matcher.h"
%}

%geocal_shared_ptr(ImageMatcher);
namespace GeoCal {
class ImageMatcher {
public:
  virtual void match(const RasterImage& Ref, const RasterImage&
		     New, const ImageCoordinate& Ref_loc, const
		     ImageCoordinate& New_guess, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const = 0;  
  std::string print_to_string() const;
};
}


