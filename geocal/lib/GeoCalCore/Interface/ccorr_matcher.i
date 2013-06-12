// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ccorr_matcher.h"
%}
%base_import(image_matcher)
%geocal_shared_ptr(GeoCal::CcorrMatcher);
namespace GeoCal {

class CcorrMatcher: public ImageMatcher {
public:
  CcorrMatcher(int Target_nline = 37, int Target_nsamp = 37, int
	       Template_nline = 9, int Template_nsamp = 9, double Min_ccorr =
	       0.3, double Min_variance = 0);
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const;
  %python_attribute(min_correlation, double)
  %python_attribute(min_variance, double)
  %python_attribute(target_number_line, int)
  %python_attribute(target_number_sample, int)
  %python_attribute(template_number_line, int)
  %python_attribute(template_number_sample, int)
  %pickle_init(1, self.target_number_line, self.target_number_sample, 
	       self.template_number_line, self.template_number_sample,
	       self.min_correlation, self.min_variance)
};
}


