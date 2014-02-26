#include "ccorr_matcher.h"
#include <blitz/array.h>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
// Constructor. Default values were tuned for MISR imagery, but work
// well for other imagery.
//-----------------------------------------------------------------------

CcorrMatcher::CcorrMatcher(int Target_nline, int Target_nsamp, int
	     Template_nline, int Template_nsamp, double Min_ccorr, 
	     double Min_variance)
: min_correlation_(Min_ccorr), min_variance_(Min_variance), 
  target_number_line_(Target_nline), target_number_sample_(Target_nsamp),
  template_number_line_(Template_nline), 
  template_number_sample_(Template_nsamp)
{
  range_min_check(Min_ccorr, 0.0);
  range_min_check(Min_variance, 0.0);
  range_min_check(Target_nline, 0);
  range_min_check(Target_nsamp, 0);
  range_min_check(Template_nline, 0);
  range_min_check(Template_nsamp, 0);
  if(Target_nline % 2 != 1 ||
     Target_nsamp % 2 != 1 ||
     Template_nline % 2 != 1 ||
     Template_nsamp % 2 != 1)
    throw Exception("Target and template window size needs to be and odd number of pixels");
  templ.resize(template_number_line(), template_number_sample());
  target.resize(target_number_line(), target_number_sample());
}

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
/// parameters of a LsmMatcher).
//-----------------------------------------------------------------------

void CcorrMatcher::match_mask
(const RasterImage& Ref, 
 const ImageMask& Ref_mask,			 
 const RasterImage& New, 
 const ImageMask& New_mask,
 const ImageCoordinate& Ref_loc, 
 const ImageCoordinate& New_guess, 
 ImageCoordinate& New_res,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  Success = false;		// We'll change this once we succeed.
  int ref_line = int(Ref_loc.line) - (template_number_line() - 1) / 2;
  int ref_sample = int(Ref_loc.sample) - (template_number_sample() - 1) / 2;
  int new_line = int(New_guess.line) - (target_number_line() - 1) / 2;
  int new_sample = int(New_guess.sample) - (target_number_sample() - 1) / 2;

//-----------------------------------------------------------------------
// If we don't have enough data to fill the template or target window,
// then we can't do image matching. This is not an error on the part
// of the caller of this function, since they should not need to know
// the template or target window size in order to know if Ref_loc or
// New_guess is too close to the edge. Set Success flag to false,
// and return.
//-----------------------------------------------------------------------

  if(ref_line                              < 0                   ||
     ref_line + template_number_line()     >=Ref.number_line()   ||
     ref_sample                            < 0                   ||
     ref_sample + template_number_sample() >=Ref.number_sample() ||
     new_line                              < 0                   ||
     new_line + target_number_line()       >=New.number_line()   ||
     new_sample                            < 0                   ||
     new_sample + target_number_sample()   >=New.number_sample()) {
    if(Diagnostic)
      *Diagnostic = TOO_CLOSE_TO_IMAGE_EDGE;
    return;
  }

//-----------------------------------------------------------------------
// Check if any of the date we want to use is masked.
//-----------------------------------------------------------------------

  if(Ref_mask.area_any_masked(ref_line, ref_sample, template_number_line(),
			      template_number_sample()) ||
     New_mask.area_any_masked(new_line, new_sample, target_number_line(),
			      target_number_sample())) {
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }

//-----------------------------------------------------------------------
// Otherwise, extract the template and target from the input imagery 
//-----------------------------------------------------------------------

  Ref.read_ptr(ref_line, ref_sample, template_number_line(), 
	       template_number_sample(), templ.data());
  New.read_ptr(new_line, new_sample, target_number_line(), 
	       target_number_sample(), target.data());

//-----------------------------------------------------------------------
// Get mean and variance, and check threshold.
//-----------------------------------------------------------------------

  double template_mean = mean(templ);
  double template_var = 
    (sum(sqr(templ)) - templ.size() * template_mean * template_mean) / 
    (templ.size() - 1);
  if(template_var < min_variance_) {
    if(Diagnostic)
      *Diagnostic = VARIANCE_TOO_LOW;
    return;
  }
  int imax = -1, jmax = -1;
  double correlation_max = 0.0;
  for(int i = 0; i < target.extent(0) - templ.extent(0); ++i)
    for(int j = 0; j < target.extent(1) - templ.extent(1); ++j) {
      Range r1(i, i + templ.extent(0) - 1);
      Range r2(j, j + templ.extent(1) - 1);
      Array<int, 2> tsub(target(r1, r2));
      double tmean = mean(tsub);
      double tvar = 
	(sum(sqr(tsub)) - tsub.size() * tmean * tmean) / (tsub.size() - 1);
      if(tvar < min_variance_)
	continue;
      double ccorr_covariance = 
	(sum(tsub * templ) - tsub.size() * tmean * template_mean) / 
	(tsub.size() - 1);
      double correlation = fabs(ccorr_covariance) * ccorr_covariance /
	(tvar * template_var);
      if(correlation > correlation_max) {
	correlation_max = correlation;
	imax = i;
	jmax = j;
      }
    }
  if(correlation_max < min_correlation_) {
    if(Diagnostic)
      *Diagnostic = CORRELATION_TO_LOW;
    return;
  }
  New_res.line = new_line + imax + (template_number_line() - 1) / 2;
  New_res.sample = new_sample + jmax + (template_number_sample() - 1) / 2;
  Line_sigma = 0.5;
  Sample_sigma = 0.5;
  if(Diagnostic)
    *Diagnostic = NO_FAIL;
  Success = true;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void CcorrMatcher::print(std::ostream& Os) const
{
  Os << "Cross correlation matcher:\n"
     << "  Minimum correlation: " << min_correlation_ << "\n"
     << "  Minimum variance:    " << min_variance_ << "\n"
     << "  Target:              " << target_number_line_ << " X " 
     << target_number_sample_ << "\n"
     << "  Template:            " << template_number_line_ << " X " 
     << template_number_sample_ << "\n";
}
