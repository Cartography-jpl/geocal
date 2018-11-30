// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "pyramid_image_matcher.h"
#include "image_ground_connection.h"
%}
%base_import(image_matcher)
%geocal_shared_ptr(GeoCal::PyramidImageMatcher);
namespace GeoCal {
class PyramidImageMatcher : public ImageMatcher {
public:
  PyramidImageMatcher(const boost::shared_ptr<ImageMatcher>& Im,
		      int start_level,
		      bool Only_accept_finest_resolution = false);
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const;
  %python_attribute(start_level, int)
  %python_attribute(only_accept_finest_resolution, bool)
  %python_attribute(underlying_matcher, boost::shared_ptr<ImageMatcher>)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("PyramidImageMatcher")

