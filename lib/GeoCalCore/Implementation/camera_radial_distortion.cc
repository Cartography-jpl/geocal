#include "camera_radial_distortion.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CameraRadialDistortion::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
  ar & GEOCAL_NVP_(k_distort);
}


GEOCAL_IMPLEMENT(CameraRadialDistortion);
#endif

void CameraRadialDistortion::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "CameraRadialDistortion:\n"
     << "   Number line:     " << number_line(0) << "\n"
     << "   Number sample:   " << number_sample(0) << "\n"
     << "   Focal length:    " << focal_length() << " mm\n"
     << "   Line pitch:      " << line_pitch() << " mm\n"
     << "   Sample pitch:    " << sample_pitch() << " mm\n"
     << "   Principal point: " << principal_point(0) << "\n"
     << "   Frame convention: " << (frame_convention() == LINE_IS_X ?
				    "LINE_IS_X\n" : "LINE_IS_Y\n")
     << "   Frame to spacecraft: " << frame_to_sc() << "\n"
     << "   K_distort:\n";
  opad << k_distort() << "\n";
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// Calculate dr_over_r from the undistorted x and y
//-----------------------------------------------------------------------

double CameraRadialDistortion::dr_over_r_calc_from_x_y
(double x, double y) const
{
  // Pulled this code from ISIS CameraRadialDistortionMap. Not sure if
  // we can come up with a closed form solution, or something better
  // here. But this is good for now.
  double r2 = (x * x + y * y);
  double r = sqrt(r2);
  double dr_over_r = dr_over_r_calc(r2);
  // Estimate rp
  double rp = (1 + dr_over_r) * r;
  double rp_prev;
  double tol = std::min(line_pitch(), sample_pitch()) / 100.0;
  int iteration = 0;
  do {
    // Don't get in an end-less loop.  This algorithm should
    // converge quickly.  If not then we are probably way outside
    // of the focal plane.  Just set the distorted position to the
    // undistorted position. Also, make sure the focal plane is less
    // than 1km, it is unreasonable for it to grow larger than that.
    if (iteration >= 15 || r > 1E9) {
      dr_over_r = 0.0;
      break;
    }
    rp_prev = rp;
    double rp2 = rp * rp;
    dr_over_r = dr_over_r_calc(rp2);
    rp = r + (dr_over_r * rp_prev);  // Compute new estimate of r
    iteration++;
  } while (fabs(rp - rp_prev) > tol);
  return dr_over_r;
}

//-----------------------------------------------------------------------
/// Determine the maximum radius that covers the actual camera, we
/// truncate this as the maximum distortion we apply, so values well
/// outside of the camera don't have extremely large distortions
/// (the polynomial extracts really pretty badly).
//-----------------------------------------------------------------------

void CameraRadialDistortion::fill_in_max() const
{
  if(max_r2_filled_in)
    return;
  double xfp_max = 0, yfp_max = 0;
  max_rp2 = 0;
  for(int ln_i = 0; ln_i < 2; ++ln_i)
    for(int smp_i = 0; smp_i < 2; ++smp_i) {
      FrameCoordinate fc(ln_i * number_line(0),
			 smp_i * number_sample(0));
      double xfp, yfp;
      fc_to_focal_plane(fc, 0, xfp, yfp);
      double rp2 = xfp*xfp + yfp*yfp;
      if(rp2 > max_rp2) {
	max_rp2 = rp2;
	xfp_max = xfp;
	yfp_max	= yfp;
      }
    }
  max_dr_over_r = dr_over_r_calc(max_rp2);
  double x = xfp_max * (1 - max_dr_over_r);
  double y = yfp_max * (1 - max_dr_over_r);
  max_r2 = x*x + y*y;
}

void CameraRadialDistortion::dcs_to_focal_plane
(int Band, const boost::math::quaternion<double>& Dcs,
 double& Xfp, double& Yfp) const
{
  fill_in_max();
  double x, y;
  QuaternionCamera::dcs_to_focal_plane(Band, Dcs, x, y);
  double dr_over_r;
  if(x*x+y*y > max_r2)
    dr_over_r = max_dr_over_r;
  else
    dr_over_r = dr_over_r_calc_from_x_y(x, y);
  Xfp = x / (1 - dr_over_r);
  Yfp = y / (1 - dr_over_r);
  if(false)
    std::cerr << "dcs_to_focal_plane:\n"
	      << "  x: " << x << "\n"
	      << "  y: " << y << "\n"
	      << "  dr_over_r: " << dr_over_r << "\n"
	      << "  Xfp: " << Xfp << "\n"
	      << "  Yfp: " << Yfp << "\n";
}

void CameraRadialDistortion::dcs_to_focal_plane
(int Band, const boost::math::quaternion<AutoDerivative<double> >& Dcs,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  fill_in_max();
  AutoDerivative<double> x, y;
  QuaternionCamera::dcs_to_focal_plane(Band, Dcs, x, y);
  double dr_over_r;
  if(x.value()*x.value()+y.value()*y.value() > max_r2)
    dr_over_r = max_dr_over_r;
  else
    dr_over_r = dr_over_r_calc_from_x_y(x.value(), y.value());
  Xfp = x / (1 - dr_over_r);
  Yfp = y / (1 - dr_over_r);
}

boost::math::quaternion<double> CameraRadialDistortion::focal_plane_to_dcs
(int Band, double Xfp, double Yfp) const
{
  fill_in_max();
  double rp2 = (Xfp *Xfp + Yfp * Yfp);
  double dr_over_r;
  if(rp2 > max_rp2)
    dr_over_r = max_dr_over_r;
  else
    dr_over_r = dr_over_r_calc(rp2);
  double x = Xfp * (1-dr_over_r);
  double y = Yfp * (1-dr_over_r);
  if(false)
    std::cerr << "focal_plane_to_dcs:\n"
	      << "  Xfp: " << Xfp << "\n"
	      << "  Yfp: " << Yfp << "\n"
	      << "  dr_over_r: " << dr_over_r << "\n"
	      << "  x: " << x << "\n"
	      << "  y: " << y << "\n";
  return QuaternionCamera::focal_plane_to_dcs(Band, x, y);
}

boost::math::quaternion<AutoDerivative<double> >
CameraRadialDistortion::focal_plane_to_dcs
(int Band, const AutoDerivative<double>& Xfp, 
 const AutoDerivative<double>& Yfp) const
{
  fill_in_max();
  double rp2 = (Xfp.value() *Xfp.value() + Yfp.value() * Yfp.value());
  double dr_over_r;
  if(rp2 > max_rp2)
    dr_over_r = max_dr_over_r;
  else
    dr_over_r = dr_over_r_calc(rp2);
  AutoDerivative<double> x = Xfp * (1-dr_over_r);
  AutoDerivative<double> y = Yfp * (1-dr_over_r);
  return QuaternionCamera::focal_plane_to_dcs(Band, x, y);
}



