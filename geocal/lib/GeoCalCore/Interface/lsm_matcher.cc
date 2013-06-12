#include "lsm_matcher.h"
#include <blitz/array.h>
#include <iostream>
#include "gsl/gsl_linalg.h"
#include "geocal_gsl_matrix.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Wrapper around DGESV, which solves the linear system y = A * x. On
/// exit, y has been filled in with the solution x, and A has been
/// destroyed. 
//-----------------------------------------------------------------------

void LsmMatcher::solve(Array<double, 2>& A, Array<double, 2>& y) const
{
  GslMatrix am(A);
  gsl_permutation * p = gsl_permutation_alloc(A.extent(0));
  int s;
  int status = gsl_linalg_LU_decomp(am.gsl(), p, &s);
  gsl_check(status);
  for(int i = 0; i < y.extent(0); ++i) {
    Array<double,1> yr(y(i, Range::all()));
    GslVector yrv(yr);
    status = gsl_linalg_LU_svx(am.gsl(), p, yrv.gsl());
    gsl_check(status);
  }
  gsl_permutation_free(p);
}

//-----------------------------------------------------------------------
// Stencil used as a low pass filter
//-----------------------------------------------------------------------

BZ_DECLARE_STENCIL2(lowpass, A, B)
A = (B(-1, -1) +       B(-1, 0) + B(-1, 1)
  + B(0, -1)  + 2.0 * B(0, 0)  + B(0, 1)
  + B(1, -1)  +       B(1, 0)  + B(1, 1)) / 10.0;
BZ_END_STENCIL

//-----------------------------------------------------------------------
/// Constructor. Default values were tuned for MISR imagery, but work
/// well for other imagery.
//-----------------------------------------------------------------------

LsmMatcher::LsmMatcher(int Number_line, int Number_sample, int
     Border_size, double Precision_goal, double Precision_requirement, 
     double Max_sigma, double Rad_uncertainty_factor,
     double Precision_min_geo_goal, double
     Precision_min_rad_goal)
: number_line_(Number_line), number_sample_(Number_sample), 
  border_size_(Border_size), precision_goal_ (Precision_goal),
  precision_requirement_(Precision_requirement), max_sigma_(Max_sigma),
  rad_uncertainty_factor_(Rad_uncertainty_factor),
  precision_min_geo_goal_(Precision_min_geo_goal), 
  precision_min_rad_goal_(Precision_min_rad_goal),
  target_res(Number_line, Number_sample),
  grad_x(Number_line, Number_sample),
  grad_y(Number_line, Number_sample), v(8)
{
  range_min_check(Number_line, 0);
  range_min_check(Number_sample, 0);
  range_min_check(Border_size, 0);
  range_min_check(Precision_goal, 0.0);
  range_min_check(Precision_requirement, 0.0);
  range_min_check(Max_sigma, 0.0);
  range_min_check(Rad_uncertainty_factor, 0.0);
  range_min_check(Precision_min_geo_goal, 0.0);
  range_min_check(Precision_min_rad_goal, 0.0);
  if(Number_line % 2 != 1 ||
     Number_sample % 2 != 1)
    throw Exception("Target and template window size needs to be and odd number of pixels");

// Line_offset and sample_offset give index into template with center
// set at 0, 0.

  line_offset = (number_line() - 1) / 2;
  sample_offset = (number_sample() - 1) / 2; 
}

//-----------------------------------------------------------------------
/// Copy constructor. 
//-----------------------------------------------------------------------

LsmMatcher::LsmMatcher(const LsmMatcher& M)
: number_line_(M.number_line()), number_sample_(M.number_sample()), 
  border_size_(M.border_size()), precision_goal_(M.precision_goal()),
  precision_requirement_(M.precision_requirement()), max_sigma_(M.max_sigma()),
  rad_uncertainty_factor_(M.rad_uncertainty_factor()),
  precision_min_geo_goal_(M.precision_min_geo_goal()), 
  precision_min_rad_goal_(M.precision_min_rad_goal()),
  line_offset(M.line_offset), sample_offset(M.sample_offset), 
  target_res(M.number_line(), M.number_sample()),
  grad_x(M.number_line(), M.number_sample()),
  grad_y(M.number_line(), M.number_sample()), v(8)
{
}

//-----------------------------------------------------------------------
/// Resample target geometrically, but not radiometricly.
/// Returns true if we could resample, or false if this takes us
/// outside of target.
//-----------------------------------------------------------------------

bool LsmMatcher::resample_geometrically() const {
  for(int i = 0; i < target_res.extent(0); ++i)
    for(int j = 0; j < target_res.extent(1); ++j) {
 // This is index into target. Extra border_size() term is because
 // target has a border around it to allow us to move more than a
 // pixel while doing matching.
      double l = v(2) + v(3) * (i - line_offset) + v(4) * (j - sample_offset) +
	line_offset + border_size();
      double s = v(5) + v(6) * (i - line_offset) + v(7) * (j - sample_offset) +
	sample_offset + border_size();
      int li = (int) floor(l);
      int si = (int) floor(s);
      if(li < 0 || li + 1 >= target.extent(0) || 
	 si < 0 || si + 1 >= target.extent(1))
	 return false;		// We have moved outside of target, so
				// give up.
      double d1 = target(li + 1, si + 1) + target(li, si) -
	target(li, si + 1) - target(li + 1, si);
      double d2 = target(li, si + 1) - target(li, si);
      double d3 = target(li + 1, si) - target(li, si);
      target_res(i, j) = d1 * (s - si) * (l - li) + d2 * (s-si)
	+ d3 * (l - li) + target(li, si);
      grad_x(i, j) = d3 + d1 * (s - si);
      grad_y(i, j) = d2 + d1 * (l - li);
    }
  return true;
}

//-----------------------------------------------------------------------
/// Calculate jacobian.
//-----------------------------------------------------------------------

Array<double, 3> LsmMatcher::jacobian() const 
{  
  firstIndex i1;
  secondIndex i2;
  thirdIndex i3;
  fourthIndex i4;
  Array<double, 3> jac(target_res.extent(0), target_res.extent(1), 
		       v.extent(0));
  // derivative wrt h0.
  jac(Range::all(), Range::all(), 0) = 1; 
  // derivative wrt h1.
  jac(Range::all(), Range::all(), 1) = target_res;
  // derivative wrt a0.
  jac(Range::all(), Range::all(), 2) = v(1) * grad_x;
  // derivative wrt a1.
  jac(Range::all(), Range::all(), 3) = v(1) * grad_x * (i1 - line_offset);
  // derivative wrt a2.
  jac(Range::all(), Range::all(), 4) = v(1) * grad_x * (i2 - sample_offset);
  // derivative wrt b0.
  jac(Range::all(), Range::all(), 5) = v(1) * grad_y;
  // derivative wrt b1.
  jac(Range::all(), Range::all(), 6) = v(1) * grad_y * (i1 - line_offset);
  // derivative wrt b2.
  jac(Range::all(), Range::all(), 7) = v(1) * grad_y * (i2 - sample_offset);
  return jac;
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

void LsmMatcher::match_mask
(const RasterImage& Ref, 
 const ImageMask& Ref_mask,
 const RasterImage& New, 
 const ImageMask& New_mask,
 const ImageCoordinate& Ref_loc, const
 ImageCoordinate& New_guess, 
 ImageCoordinate& New_res,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  Success = false;		   // We'll change this once we
				   // succeed. .
// Extra border we need to do low pass kernel.
  const int kadjust = (3 - 1) / 2; 
  double ref_line = Ref_loc.line - (number_line() - 1) / 2 - kadjust;
  double ref_sample = Ref_loc.sample - (number_sample() - 1) / 2 - kadjust;
  double new_line = New_guess.line - (number_line() - 1) / 2 - 
    kadjust - border_size();
  double new_sample = New_guess.sample - (number_sample() - 1) / 2 -
    kadjust - border_size();
    

//-----------------------------------------------------------------------
// If we don't have enough data to fill the template or target window,
// then we can't do image matching. This is not an error on the part
// of the caller of this function, since they should not need to know
// the template or target window size in order to know if Ref_loc or
// New_guess is too close to the edge. Set Success flag to false,
// and return.
//-----------------------------------------------------------------------

  if(ref_line                              < 0                   ||
     ref_line + number_line() + 2 * kadjust
                                  >=Ref.number_line()            ||
     ref_sample                            < 0                   ||
     ref_sample + number_sample() + 2 * kadjust
                                  >=Ref.number_sample()          ||
     new_line                              < 0                   ||
     new_line + number_line() + 2 * kadjust + 2 * border_size()
                                  >=New.number_line()            ||
     new_sample                            < 0                   ||
     new_sample + number_sample() + 2 * kadjust + 2 * border_size()
     >=New.number_sample()) {
    if(Diagnostic)
      *Diagnostic = TOO_CLOSE_TO_IMAGE_EDGE;
    return;
  }

//-----------------------------------------------------------------------
// Check if any of the date we want to use is masked.
//-----------------------------------------------------------------------

  if(Ref_mask.area_any_masked((int) ref_line, (int) ref_sample, 
			      number_line() + 2 * kadjust + 1,
			      number_sample() + 2 * kadjust + 1) ||
     New_mask.area_any_masked((int) new_line, (int) new_sample, 
			      number_line() + 2 * kadjust + 
			      2 * border_size() + 1,
			      number_sample() + 2 * kadjust + 
			      2 * border_size() + 1)) {
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }

//-----------------------------------------------------------------------
// Otherwise, extract the template and target from the input imagery.
// We resample the data as we extract it.
//-----------------------------------------------------------------------

  Array<double, 2> templd = Ref.interpolate(ref_line, ref_sample, 
				       number_line() + 2 * kadjust,
				       number_sample() + 2 * kadjust);
  Array<double, 2> targetd = New.interpolate(new_line, new_sample,
		number_line() + 2 * kadjust + 2 * border_size(),
		number_sample() + 2 * kadjust + 2 * border_size());

//-----------------------------------------------------------------------
// Apply low pass filter to target and template. This gives us the
// target and template we will use for matching.
//-----------------------------------------------------------------------

  Array<double, 2> templl(templd.shape());
  Array<double, 2> targetl(targetd.shape());
  applyStencil(lowpass(), templl, templd);
  applyStencil(lowpass(), targetl, targetd);
  // Trim edges that are beyond what the lowpass stencil is applied
  // to.
  templ.reference(templl(Range(1, number_line()), 
				Range(1, number_sample())));
  target.reference(targetl(Range(1, number_line() + 2 * border_size()), 
			   Range(1, number_sample() + 2 * border_size())));

//-----------------------------------------------------------------------
// Calculate mean and variance of target and template in order to do 
// histogram equalization.
//-----------------------------------------------------------------------

  double template_mean = mean(templ);
  double template_var = 
    (sum(sqr(templ)) - templ.size() * template_mean * template_mean) / 
    (templ.size() - 1);
  double target_mean = mean(target);
  double target_var = 
    (sum(sqr(target)) - target.size() * target_mean * target_mean) / 
    (target.size() - 1);
  double min_rad_var = (target_var < template_var ? target_var : template_var);
  double max_rad_var = min_rad_var * min_rad_var * rad_uncertainty_factor();

//-----------------------------------------------------------------------
// Set geometric parameters to the identity transform, and radiometric
// to equalize the histograms.
// 
// The parameters in v are h0, h1, a0, a1, a2, b0, b1 and b2. 
//-----------------------------------------------------------------------

  v = template_mean - target_mean, template_var / target_var,
    0, 1, 0, 0, 0, 1;

//-----------------------------------------------------------------------
// Resample geometrically, and also calculate grad_x and grad_y
//-----------------------------------------------------------------------

  if(!resample_geometrically())	{
    if(Diagnostic)
      *Diagnostic = MOVE_PAST_TARGET;
    return;			// If we go outside image, give up on
				// matching. 
  }

//-----------------------------------------------------------------------
// Start iteration.
//-----------------------------------------------------------------------

  for(int i = 0; i < max_iteration; ++i) {

//-----------------------------------------------------------------------
// Want to solve (template - target) = jac * delta_v in least squares
// sense. 
//-----------------------------------------------------------------------

    firstIndex i1;
    secondIndex i2;
    thirdIndex i3;
    fourthIndex i4;
    Array<double, 3> jac = jacobian();

    Array<double, 1> delta_y(v.extent(0));
    Array<double, 2> design(v.extent(0), v.extent(0));
    for(int j = 0; j < delta_y.extent(0); ++j)
      delta_y(j) = sum(jac(Range::all(), Range::all(), j) *
		    (templ - (v(0) + v(1) * target_res)));
    design = sum(sum(jac(i3, i4, i1) * jac(i3, i4, i2), i4), i3);

    // We tack on some vectors to help calculate various pieces of 
    // (A^T * A) ^ -1 w/o doing full inversion. Note this is in reverse
    // order because Fortran uses col major rather than C row major
    Array<double, 2> yside(4, v.extent(0));
    yside = 0;
    yside(0, Range::all()) = delta_y;
    yside(1, 2) = 1;		// a0 term
    yside(2, 5) = 1;		// b0 term
    yside(3, 0) = 1;		// h0 term
    try {
      solve(design, yside);	// Solution now in yside.
    } catch(const Exception& E) {
      if(Diagnostic)
	*Diagnostic = SOLVE_FAILED; // This is probably is from image
				  // with no contrast, such as water
				  // or a black edge in the image.
      return;
    }
    Array<double, 1> delta_v(yside(0, Range::all()));
    v += delta_v;

//-----------------------------------------------------------------------
// Resample geometrically, and also calculate grad_x and grad_y
//-----------------------------------------------------------------------

    if(!resample_geometrically()) {
      if(Diagnostic)
	*Diagnostic = MOVE_PAST_TARGET;
      return;			// If we go outside image, give up on
				// matching. 
    }

//-----------------------------------------------------------------------
// Calculate variance, and if we have exceeded thresholds give up.
//-----------------------------------------------------------------------

    double diff_var = sum(sqr(templ - (v(0) + v(1) * target_res))) / 
      (templ.size() - v.size());
    double a0_var = yside(1, 2) * diff_var;
    double b0_var = yside(2, 5) * diff_var;
    double h0_var = yside(3, 0) * diff_var;
    if(a0_var > max_sigma() * max_sigma() ||
       b0_var > max_sigma() * max_sigma()) {
      if(Diagnostic)
	*Diagnostic = EXCEED_MAX_SIGMA;
      return;
    }
    if(h0_var > max_rad_var) {
      if(Diagnostic)
	*Diagnostic = EXCEED_MAX_RADIANCE_VARIANCE;
      return;
    }

//-----------------------------------------------------------------------
// If we have reached our precision goal, then return success.
//-----------------------------------------------------------------------

    if((fabs(delta_v(2)) <= precision_goal() &&
	fabs(delta_v(5)) <= precision_goal()) ||
       ((i == max_iteration - 1) &&
	fabs(delta_v(2)) < precision_min_geo_goal() &&
	fabs(delta_v(5)) < precision_min_geo_goal() &&
	fabs(delta_v(0)) < precision_min_rad_goal() * min_rad_var)) {
      New_res.line = New_guess.line + v(2);
      New_res.sample = New_guess.sample + v(5);
      Line_sigma = std::max(sqrt(a0_var), precision_goal());
      Sample_sigma = std::max(sqrt(b0_var), precision_goal());
      if(Line_sigma <= precision_requirement() &&
	 Sample_sigma <= precision_requirement()) {
	if(Diagnostic)
	  *Diagnostic = NO_FAIL;
	Success = true;
      } else {
	if(Diagnostic)
	  *Diagnostic = EXCEED_PRECISION_REQUIREMENT;
      }
      return;
    }
  }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void LsmMatcher::print(std::ostream& Os) const
{
  Os << "Least squares matcher:\n"
     << "  Matching area:        " << number_line_ << " X " 
     << number_sample_ << "\n"
     << "  Border size:           " << border_size_ << "\n"
     << "  Precision goal:        " << precision_goal_ << "\n"
     << "  Precision requirement: " << precision_requirement_ << "\n"
     << "  Maximum sigma:         " << max_sigma_ << "\n"
     << "  Radiometric uncertainty factor: " << rad_uncertainty_factor_ << "\n"
     << "  Precision minimum geometric goal: " << precision_min_geo_goal_ 
     << "\n"
     << "  Precision minimum radiometric goal: " << precision_min_rad_goal_
     << "\n";
}
