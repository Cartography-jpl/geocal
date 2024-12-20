// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ccorr_lsm_matcher.h"
#include "image_ground_connection.h"
%}
%base_import(image_matcher)
%import "ccorr_matcher.i"
%import "lsm_matcher.i"
%geocal_shared_ptr(GeoCal::CcorrLsmMatcher);
namespace GeoCal {

class CcorrLsmMatcher: public ImageMatcher {
public:
  CcorrLsmMatcher(bool Accept_ccorr_only = false);
  CcorrLsmMatcher(const boost::shared_ptr<ImageMatcher> Ccorr,
		  const boost::shared_ptr<ImageMatcher> LsmMatcher,
		  bool Accept_ccorr_only = false);
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const;
  %python_attribute2(ccorr_matcher, ccorr_matcher_ptr, 
		     boost::shared_ptr<ImageMatcher>)
  %python_attribute2(lsm_matcher, lsm_matcher_ptr, 
		     boost::shared_ptr<ImageMatcher>)
  %python_attribute(accept_ccorr_only, bool)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("CcorrLsmMatcher")

