#ifndef LSM_MATCHER_H
#define LSM_MATCHER_H
#include "image_matcher.h"
#include <boost/utility.hpp>

namespace GeoCal {
/****************************************************************//**
  This class performs image matching. This does a nonlinear least
  squares match. It also calculates an estimate of the error in its
  matching.

  We attempt to reach a goal of precision_goal in determining a0 and
  b0. If at any iteration, the uncertainty in determining a0 and b0 is
  greater than max_sigma, then we give up on doing LSM. If the final
  uncertainty in determining a0 and b0 is larger than
  precision_requirement, then we give up on doing LSM. We now also
  check radiometric uncertainty at each iteration as described in MISR
  SDS DFM-0245-I. The maximum allowed radiometric uncertainty is
  calculated by multiplying the factor radiometric_uncertainty_factor
  by the minimum of the template and target window sigmas. After each
  iteration in the lsm the uncertainty in the calculation of h0 is
  greater than this threshold we stop doing LSM and return
  unsuccessful.

  The minimum uncertainty that will be returned is precision_goal
  (i.e., if the estimated uncertainty is smaller than this value,
  then precision_goal is returned instead).

  The model used is to resample the target using an affine
  transformation combined with a linear correction to the radiometry:

  g'(i, j) = h0 + h1 * g(a0 + a1 * i + a2 * j, b0 + b1 * i + b2 * j)
  
  Where g is the target, g' is the template.

  As a convention, internal to this class we index things so g'(0, 0)
  is the center of the template. 

  We solve for the parameters giving the best fit against the
  template. 

*******************************************************************/

class LsmMatcher : public ImageMatcher {
public:
//-----------------------------------------------------------------------
/// Diagnostic codes indicates why a match failed.
//-----------------------------------------------------------------------

  enum Diagnostic {NO_FAIL = 0, IMAGE_MASKED = 1, TOO_CLOSE_TO_IMAGE_EDGE = 2,
		   EXCEED_MAX_SIGMA = 5, EXCEED_MAX_RADIANCE_VARIANCE = 6,
		   EXCEED_PRECISION_REQUIREMENT = 7, MOVE_PAST_TARGET = 8,
                   SOLVE_FAILED = 9, EXCEED_MAX_ITERATION=10};
  LsmMatcher(int Number_line = 21, int Number_sample = 21, int
     Border_size = 3, double Precision_goal = 0.0625, 
     double Precision_requirement = 0.2, 
     double Max_sigma = 0.5, double Rad_uncertainty_factor = 2.0,
     double Precision_min_geo_goal = 0.15, double
     Precision_min_rad_goal = 1);
  LsmMatcher(const LsmMatcher& M);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~LsmMatcher() {}
  virtual void match_mask(const RasterImage& Ref, const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, const
			  ImageCoordinate& New_guess, 
			  ImageCoordinate& New_res,
			  double& Line_sigma, double& Sample_sigma,
			  bool& Success,
			  int* Diagnostic = 0) const;

//-----------------------------------------------------------------------
/// Number of lines in target and template.
//-----------------------------------------------------------------------

  int number_line() const { return number_line_; }

//-----------------------------------------------------------------------
/// Number of samples in target and template.
//-----------------------------------------------------------------------

  int number_sample() const { return number_sample_; }

//-----------------------------------------------------------------------
/// Border added to target to allow for template/target shift.
//-----------------------------------------------------------------------

  int border_size() const { return border_size_; }

//-----------------------------------------------------------------------
/// Goal in doing solving LSM.
//-----------------------------------------------------------------------

  double precision_goal() const { return precision_goal_; }

//-----------------------------------------------------------------------
/// Requirement for LSM success.
//-----------------------------------------------------------------------

  double precision_requirement() const { return precision_requirement_; }

//-----------------------------------------------------------------------
/// If at any stagee in solving LSM uncertainty is greater than this
/// value, then give up doing image matching. 
//-----------------------------------------------------------------------

  double max_sigma() const { return max_sigma_; }

//-----------------------------------------------------------------------
/// This factor is used to scale the maximum allowable radiometric
/// uncertainty while solving LSM. 
//-----------------------------------------------------------------------

  double rad_uncertainty_factor() const { return rad_uncertainty_factor_; }

//-----------------------------------------------------------------------
/// Minimum geometric goal for LSM to success.
//-----------------------------------------------------------------------

  double precision_min_geo_goal() const { return precision_min_geo_goal_; }

//-----------------------------------------------------------------------
/// Minimum radiometric goal for LSM to success.
//-----------------------------------------------------------------------

  double precision_min_rad_goal() const { return precision_min_rad_goal_; }

  virtual void print(std::ostream& Os) const;
private:
  int number_line_;
  int number_sample_;
  int border_size_;
  double precision_goal_;
  double precision_requirement_;
  double max_sigma_;
  double rad_uncertainty_factor_;
  double precision_min_geo_goal_;
  double precision_min_rad_goal_;

//-----------------------------------------------------------------------
// These are scratch variables. Rather than passing them around, we
// put them here and access them from various routines.
//-----------------------------------------------------------------------

// Line_offset and sample_offset give index into template with center
// set at 0, 0.

  int line_offset;
  int sample_offset;

// Target and template.

  mutable blitz::Array<double, 2> templ;
  mutable blitz::Array<double, 2> target;

// Target resampled geometrically (but not radiometrically), along
// with gradient in x and y direction.

  mutable blitz::Array<double, 2> target_res;
  mutable blitz::Array<double, 2> grad_x;
  mutable blitz::Array<double, 2> grad_y;

// Parameters to take target to template. This is h0, h1, a0, a1, a2,
// b0, b1, b2.

  mutable blitz::Array<double, 1> v;

  bool resample_geometrically() const;
  void solve(blitz::Array<double, 2>& A, blitz::Array<double, 2>& y) const;
  blitz::Array<double, 3> jacobian() const;

// Maximum number of iterations before giving up.
  static const int max_iteration = 6;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(LsmMatcher);

#endif

