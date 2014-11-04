#include "galileo_camera.h"
#include "geocal_gsl_root.h"
using namespace GeoCal;

// Note that the various constants come from the spice kernel
// gll36001.ti.

const int gll_num_line = 800;
const int gll_num_sample = 800;
const double gll_twist_angle = -0.0022797905; // In radians
// Pixel size is the same in the line and sample direction.
const double gll_pixel_size = 15.24e-3;	      // In mm
const double gll_focal_length = 1501.039;     // In mm
// Note the "-1" is because the spice kernel defines this in terms of
// line and sample starting with 1, but FrameCoordinate starts with 0.
const FrameCoordinate gll_fov_center(400 - 1, 400 - 1);
// Nonlinearity term
const double gll_nonlinear_a = 6.58e-9;

//-----------------------------------------------------------------------
/// Constructor. Right now we have everything hardcoded for the camera.
//-----------------------------------------------------------------------

GalileoCamera::GalileoCamera()
// We have an extra "-" with the twist angle because the twist angle
// goes from spacecraft to frame, and camera constructor wants to go
// the other way.
  :QuaternionCamera(quat_rot("z", -gll_twist_angle),
		    gll_num_line, gll_num_sample, gll_pixel_size, 
		    gll_pixel_size, gll_focal_length,
		    gll_fov_center,
		    QuaternionCamera::LINE_IS_Y)		    
{
}

//-----------------------------------------------------------------------
// Model the nonlinearity of the galileo camera model. This is
// described in the spice kernel, and is a simple cubic correction.
//-----------------------------------------------------------------------

class F: public DFunctor {
public:
  F(double R) : R_(R) {}
  virtual double operator()(const double& r) const
  { return r + gll_nonlinear_a * r * r *r - R_; }
private:
  double R_;
};

// See base class for description of this function.
void GalileoCamera::dcs_to_focal_plane
(int Band, 
 const boost::math::quaternion<double>& Dcs,
 double& Xfp, double& Yfp) const
{
  double xfp_prime = 
    focal_length() * (Dcs.R_component_2() / Dcs.R_component_4());
  double yfp_prime = 
    focal_length() * (Dcs.R_component_3() / Dcs.R_component_4());
  // Actual image distance from cneter of the field of view, in
  // pixels.
  double R = sqrt(xfp_prime * xfp_prime + yfp_prime * yfp_prime) / 
    gll_pixel_size;
  // Special handling if R is very small, to avoid divide by zero
  // error
  if(R < 0.01) {
    Xfp = xfp_prime;
    Yfp = yfp_prime;
  } else {
    F f(R);
    // We will always have 0 <= r <= R
    double r = gsl_root(f, 0, R);
    Xfp = xfp_prime * r / R;
    Yfp = yfp_prime * r / R;
  }
}

void GalileoCamera::dcs_to_focal_plane
(int Band, 
 const boost::math::quaternion<AutoDerivative<double> >& Dcs,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  AutoDerivative<double> xfp_prime = 
    focal_length_with_derivative() * 
    (Dcs.R_component_2() / Dcs.R_component_4());
  AutoDerivative<double> yfp_prime = 
    focal_length_with_derivative() * 
    (Dcs.R_component_3() / Dcs.R_component_4());
  // Actual image distance from cneter of the field of view, in
  // pixels.
  AutoDerivative<double> R = 
    std::sqrt(xfp_prime * xfp_prime + yfp_prime * yfp_prime) / 
    gll_pixel_size;
  // Special handling if R is very small, to avoid divide by zero
  // error
  if(R.value() < 0.01) {
    Xfp = xfp_prime;
    Yfp = yfp_prime;
  } else {
    F f(R.value());
    // We will always have 0 <= r <= R
    double r = gsl_root(f, 0, R.value());
    Xfp = xfp_prime * r / R;
    Yfp = yfp_prime * r / R;
  }
}

// See base class for description of this function.
boost::math::quaternion<double> GalileoCamera::focal_plane_to_dcs
(int Band, double Xfp, double Yfp) const
{
  // Ideal image distance from center of the field of view, in pixels;
  double r = sqrt(Xfp * Xfp + Yfp * Yfp) / gll_pixel_size;
  // Actual image distance from cneter of the field of view, in pixels.
  double R = r + gll_nonlinear_a * r * r *r;
  // Special handling to avoid divide by zero id.
  if(r < 0.01)
    return boost::math::quaternion<double>(0, Xfp, Yfp, 
					   focal_length());
  else 
    return boost::math::quaternion<double>(0, Xfp * R / r, Yfp * R / r, 
					   focal_length());
}

// See base class for description of this function.
boost::math::quaternion<AutoDerivative<double> > 
GalileoCamera::focal_plane_to_dcs
(int Band, const AutoDerivative<double>& Xfp, 
 const AutoDerivative<double>& Yfp) const
{
  // Ideal image distance from center of the field of view, in pixels;
  AutoDerivative<double> r = std::sqrt(Xfp * Xfp + Yfp * Yfp) / gll_pixel_size;
  // Actual image distance from cneter of the field of view, in pixels.
  AutoDerivative<double> R = r + gll_nonlinear_a * r * r *r;
  // Special handling to avoid divide by zero id.
  if(r.value() < 0.01)
    return boost::math::quaternion<AutoDerivative<double> >(0, Xfp, Yfp, 
					   focal_length_with_derivative());
  else 
    return boost::math::quaternion<AutoDerivative<double> >(0, Xfp * R / r, Yfp * R / r, 
					   focal_length_with_derivative());
}


