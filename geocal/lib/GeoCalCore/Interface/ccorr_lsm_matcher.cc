#include "ccorr_lsm_matcher.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Match a point found in the reference image with a point in the new
/// image. A initial guess in the new image is supplied. We return an
/// improved image in the new image along with the uncertainty if
/// Success is true, otherwise the match failed.
///
/// The Diagnostic argument is used to indicate why a match failed.
/// For many purposes you can ignore this, because you need to know
/// detailed information about the specific ImageMatcher that a user passed
/// to a function and what the diagnostic codes mean. However, for
/// some purposes the failure reason can be useful (e.g., tuning
/// parameters of a LsmMatcher). This class just returns the
/// Diagnostic returned by the CcorrMatcher or the LsmMatcher.
//-----------------------------------------------------------------------

void CcorrLsmMatcher::match(const RasterImage& Ref, const RasterImage&
		       New, const ImageCoordinate& Ref_loc, const
		       ImageCoordinate& New_guess, 
		       ImageCoordinate& New_res,
		       double& Line_sigma, double& Sample_sigma,
			    bool& Success, int* Diagnostic) const
{
  ImageCoordinate t;
  double ls, ss;
  ccorr_matcher().match(Ref, New, Ref_loc, New_guess, t, ls, ss, Success, 
			Diagnostic);
  if(Success) {
    lsm_matcher().match(Ref, New, Ref_loc, t, New_res, Line_sigma, 
			Sample_sigma, Success, Diagnostic);
    if(!Success && accept_ccorr_only_) {
      New_res = t;
      Line_sigma = ls;
      Sample_sigma = ss;
      Success = true;
    }
  }  
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void CcorrLsmMatcher::print(std::ostream& Os) const
{
  Os << "Cross correlation + least squares matcher:\n"
     << "  Accept Ccorr only: " << (accept_ccorr_only_ ? "true" : "false") 
     << "\n"
     << "  Cross correlation matcher: \n" << *ccorr_matcher_
     << "  Least squares matcher:\n" << *lsm_matcher_;
}
