// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "ccorr_lsm_matcher.h"
%}

%geocal_shared_ptr(CcorrLsmMatcher);
namespace GeoCal {

class CcorrLsmMatcher: public ImageMatcher {
public:
  CcorrLsmMatcher(bool Accept_ccorr_only = false);
  CcorrLsmMatcher(const boost::shared_ptr<CcorrMatcher> Ccorr,
		  const boost::shared_ptr<LsmMatcher> LsmMatcher,
		  bool Accept_ccorr_only = false);
  virtual void match(const RasterImage& Ref, const RasterImage&
		     New, const ImageCoordinate& Ref_loc, const
		     ImageCoordinate& New_guess, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const;  
  %python_attribute2(ccorr_matcher, ccorr_matcher_ptr, 
		     boost::shared_ptr<CcorrMatcher>)
  %python_attribute2(lsm_matcher, lsm_matcher_ptr, 
		     boost::shared_ptr<LsmMatcher>)
  %python_attribute(accept_ccorr_only, bool)
  %pickle_init(self.ccorr_matcher, self.lsm_matcher, 
	       self.accept_ccorr_only)
};
}


