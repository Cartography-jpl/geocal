#include "camera_radial_distortion_and_transform.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CameraRadialDistortionAndTransform::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CameraRadialDistortion);
  ar & GEOCAL_NVP_(ccd_off) & GEOCAL_NVP_(ccd_cen)
    & GEOCAL_NVP_(t_off) & GEOCAL_NVP_(tinv_off)
    & GEOCAL_NVP_(t_m) & GEOCAL_NVP_(tinv_m);
}


GEOCAL_IMPLEMENT(CameraRadialDistortionAndTransform);
#endif

void CameraRadialDistortionAndTransform::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "CameraRadialDistortionAndTransform:\n"
     << "   Number line:     " << number_line(0) << "\n"
     << "   Number sample:   " << number_sample(0) << "\n"
     << "   Focal length:    " << focal_length() << " mm\n"
     << "   Line pitch:      " << line_pitch() << " mm\n"
     << "   Sample pitch:    " << sample_pitch() << " mm\n"
     << "   Principal point: " << principal_point(0) << "\n"
     << "   Frame convention: " << (frame_convention() == LINE_IS_X ?
				    "LINE_IS_X\n" : "LINE_IS_Y\n")
     << "   Frame to spacecraft: " << frame_to_sc() << "\n"
     << "   Bin mode:         " << bin_mode() << "\n"
     << "   K_distort:\n";
  opad << k_distort() << "\n";
  opad.strict_sync();
  Os << "   Ccd_off:\n";
  opad << ccd_off() << "\n";
  opad.strict_sync();
  Os << "   Ccd_cen:\n";
  opad << ccd_cen() << "\n";
  opad.strict_sync();
  Os << "   T_off:\n";
  opad << t_off() << "\n";
  opad.strict_sync();
  Os << "   T_m:\n";
  opad << t_m() << "\n";
  opad.strict_sync();
  Os << "   Tinv_off:\n";
  opad << tinv_off() << "\n";
  opad.strict_sync();
  Os << "   Tinv_m:\n";
  opad << tinv_m() << "\n";
  opad.strict_sync();
}

FrameCoordinate
CameraRadialDistortionAndTransform::focal_plane_to_fc(int Band, double Xfp,
						      double Yfp) const
{
  range_check(Band, 0, number_band());
  double ccd_sample = tinv_off_(0) + Xfp * tinv_m_(0,0) + Yfp * tinv_m_(0,1);
  double ccd_line = tinv_off_(1) + Xfp * tinv_m_(1,0) + Yfp * tinv_m_(1,1);
  FrameCoordinate fc;
  fc.line = ccd_line - (ccd_off_(1) - ccd_cen_(1) - (bin_mode_ / 2.0 - 0.5));
  fc.sample = (ccd_sample - (ccd_off_(0) - ccd_cen_(0))) / bin_mode_;
  return fc;
}

FrameCoordinateWithDerivative CameraRadialDistortionAndTransform::focal_plane_to_fc
(int Band, const AutoDerivative<double>& Xfp,
 const AutoDerivative<double>& Yfp) const
{
  range_check(Band, 0, number_band());
  AutoDerivative<double> ccd_sample =
    tinv_off_(0) + Xfp * tinv_m_(0,0) + Yfp * tinv_m_(0,1);
  AutoDerivative<double> ccd_line  =
    tinv_off_(1) + Xfp * tinv_m_(1,0) + Yfp * tinv_m_(1,1);
  FrameCoordinateWithDerivative fc;
  fc.line = ccd_line - (ccd_off_(1) - ccd_cen_(1) - (bin_mode_ / 2.0 - 0.5));
  fc.sample = (ccd_sample - (ccd_off_(0) - ccd_cen_(0))) / bin_mode_;
  return fc;
}
  
void CameraRadialDistortionAndTransform::fc_to_focal_plane(const FrameCoordinate& Fc, int Band,
					 double& Xfp, double& Yfp) const
{
  range_check(Band, 0, number_band());
  double ccd_line = Fc.line + ccd_off_(1) - ccd_cen_(1) - (bin_mode_ / 2.0 - 0.5);
  double ccd_sample = Fc.sample * bin_mode_ + ccd_off_(0) - ccd_cen_(0);
  Xfp = t_off_(0) + ccd_sample * t_m_(0,0) + ccd_line * t_m_(0,1);
  Yfp = t_off_(1) + ccd_sample * t_m_(1,0) + ccd_line * t_m_(1,1);
}

void CameraRadialDistortionAndTransform::fc_to_focal_plane
(const FrameCoordinateWithDerivative& Fc, int Band,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  range_check(Band, 0, number_band());
  AutoDerivative<double> ccd_line =
    Fc.line + ccd_off_(1) - ccd_cen_(1) - (bin_mode_ / 2.0 - 0.5);
  AutoDerivative<double> ccd_sample =
    Fc.sample * bin_mode_ + ccd_off_(0) - ccd_cen_(0);
  Xfp = t_off_(0) + ccd_sample * t_m_(0,0) + ccd_line * t_m_(0,1);
  Yfp = t_off_(1) + ccd_sample * t_m_(1,0) + ccd_line * t_m_(1,1);
}

