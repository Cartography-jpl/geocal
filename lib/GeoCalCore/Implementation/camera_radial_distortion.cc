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

void CameraRadialDistortion::dcs_to_focal_plane
(int Band, const boost::math::quaternion<double>& Dcs,
 double& Xfp, double& Yfp) const
{
  double x, y;
  QuaternionCamera::dcs_to_focal_plane(Band, Dcs, x, y);
  double r2 = (x *x + y * y);
  double dr_over_r = k_distort_(0) + r2 * (k_distort_(1) + r2 * k_distort_(2));
  // The radial polynomial really extrapolates badly. If we are a ways
  // outside of the image, switch to a pinhole camera.
  FrameCoordinate fc_approx = focal_plane_to_fc(Band, x, y);
  const int border = 100;
  if(fc_approx.line < -border || fc_approx.line > number_line(Band) + border ||
     fc_approx.sample < -border ||
     fc_approx.sample > number_sample(Band) + border)
    dr_over_r = 0;
  Xfp = x * (1-dr_over_r);
  Yfp = y * (1-dr_over_r);
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
  AutoDerivative<double> x, y;
  QuaternionCamera::dcs_to_focal_plane(Band, Dcs, x, y);
  AutoDerivative<double> r2 = (x *x + y * y);
  AutoDerivative<double> dr_over_r =
    k_distort_(0) + r2 * (k_distort_(1) + r2 * k_distort_(2));
  // The radial polynomial really extrapolates badly. If we are a ways
  // outside of the image, switch to a pinhole camera.
  FrameCoordinate fc_approx = focal_plane_to_fc(Band, x.value(), y.value());
  const int border = 100;
  if(fc_approx.line < -border || fc_approx.line > number_line(Band) + border ||
     fc_approx.sample < -border ||
     fc_approx.sample > number_sample(Band) + border)
    dr_over_r = 0;
  Xfp = x * (1-dr_over_r);
  Yfp = y * (1-dr_over_r);
}

boost::math::quaternion<double> CameraRadialDistortion::focal_plane_to_dcs
(int Band, double Xfp, double Yfp) const
{
  double dr_over_r = 0;
  // The radial polynomial really extrapolates badly. If we are a ways
  // outside of the image, switch to a pinhole camera.
  FrameCoordinate fc_approx = focal_plane_to_fc(Band, Xfp, Yfp);
  const int border = 100;
  if(fc_approx.line < -border || fc_approx.line > number_line(Band) + border ||
     fc_approx.sample < -border ||
     fc_approx.sample > number_sample(Band) + border)
    dr_over_r = 0;
  else {
    // Pulled this code from ISIS CameraRadialDistortionMap. Not sure if
    // we can come up with a closed form solution, or something better
    // here. But this is good for now.
    double rp2 = (Xfp * Xfp + Yfp * Yfp);
    double rp = sqrt(rp2);
    dr_over_r = k_distort_(0) + rp2 * (k_distort_(1) + rp2 * k_distort_(2));
  
    // Estimate r
    double r = (1 + dr_over_r) * rp;
    double r_prev;
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

      r_prev = r;
      double r2 = r * r;
      dr_over_r = k_distort_(0) + r2 * (k_distort_(1) + r2 * k_distort_(2));
    
      r = rp + (dr_over_r * r_prev);  // Compute new estimate of r
      iteration++;
    } while (fabs(r - r_prev) > tol);
  }
  double x = Xfp / (1 - dr_over_r);
  double y = Yfp / (1 - dr_over_r);
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
  throw Exception("Not implemented yet");
}



