// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "phase_correlation_matcher.h"
%}
%base_import(image_matcher)
%geocal_shared_ptr(GeoCal::PhaseCorrelationMatcher);
namespace GeoCal {

class PhaseCorrelationMatcher: public ImageMatcher {
public:
  PhaseCorrelationMatcher(int Template_size = 32, int Search_size = 32);
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const;
  %python_attribute(template_size, int)
  %python_attribute(search_size, int)
  %pickle_serialization();
};
}


