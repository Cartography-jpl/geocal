#include "quaternion_camera.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Go from a look vector in the detector coordinate system to X and Y
/// coordinates in the focal plane.  X and Y should be given in
/// millimeters.
///
/// Note that the look vector is not necessarily normalized (since
/// some implementation don't depend on this being normalized). If you
/// need it normalized, you need to do that yourself.
///
/// The default implementation is a pinhole camera, derived classed
/// can override this to add any non-linearity correction.
//-----------------------------------------------------------------------

void QuaternionCamera::dcs_to_focal_plane
(int Band, const boost::math::quaternion<double>& Dcs,
 double& Xfp, double& Yfp) const
{
  Xfp = focal_length() * (Dcs.R_component_2() / Dcs.R_component_4());
  Yfp = focal_length() * (Dcs.R_component_3() / Dcs.R_component_4());
}

//-----------------------------------------------------------------------
/// Go from X and Y coordinates in the focal plane to a look vector in
/// the detector coordinate system to.  X and Y are given in
/// millimeters.
///
/// The default implementation is a pinhole camera, derived classed
/// can override this to add any non-linearity correction.
//-----------------------------------------------------------------------

boost::math::quaternion<double> 
QuaternionCamera::focal_plane_to_dcs(int Band, double& Xfp, double& Yfp) const
{
  return boost::math::quaternion<double>(0, Xfp, Yfp, focal_length());
}

//-----------------------------------------------------------------------
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

FrameCoordinate QuaternionCamera::frame_coordinate(const ScLookVector& Sl, 
						   int Band) const
{
  range_check(Band, 0, number_band());

  // Just reverse of sc_look_vector.
  boost::math::quaternion<double> dcs = 
    conj(frame_to_sc_) * Sl.look_quaternion() * frame_to_sc_;
  double xfp, yfp;
  dcs_to_focal_plane(Band, dcs, xfp, yfp);
  FrameCoordinate fc;
  if(frame_convention_ == LINE_IS_Y) {
    fc.sample = principal_point(Band).sample + 
      xfp / sample_pitch() * samp_dir();
    fc.line = principal_point(Band).line +
      yfp / line_pitch() * line_dir();
  } else {
    fc.sample = principal_point(Band).sample +
      yfp / sample_pitch() * samp_dir();
    fc.line = principal_point(Band).line +
      xfp / line_pitch() * line_dir();
  }
  return fc;
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to DcsLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

DcsLookVector QuaternionCamera::dcs_look_vector(const FrameCoordinate& F, 
						int Band) const
{
  range_check(Band, 0, number_band());
  double xfp, yfp;
  if(frame_convention_ == LINE_IS_Y) {
    xfp = (F.sample - principal_point(Band).sample) * sample_pitch() * 
      samp_dir();
    yfp = (F.line - principal_point(Band).line) * line_pitch()
      * line_dir();
  } else {
    yfp = (F.sample - principal_point(Band).sample) * sample_pitch() * 
      samp_dir();
    xfp = (F.line - principal_point(Band).line) * line_pitch()
      * line_dir();
  }
  return DcsLookVector(focal_plane_to_dcs(Band, xfp, yfp));
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

ScLookVector QuaternionCamera::sc_look_vector(const FrameCoordinate& F, 
					      int Band) const
{
  return ScLookVector(frame_to_sc_ * 
		      dcs_look_vector(F, Band).look_quaternion() *
		      conj(frame_to_sc_));
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuaternionCamera::print(std::ostream& Os) const
{
  Os << "QuaternionCamera:\n"
     << "   Number line:     " << number_line(0) << "\n"
     << "   Number sample:   " << number_sample(0) << "\n"
     << "   Focal length:    " << focal_length() << " mm\n"
     << "   Line pitch:      " << line_pitch() << " mm\n"
     << "   Sample pitch:    " << sample_pitch() << " mm\n"
     << "   Principal point: " << principal_point(0) << "\n"
     << "   Frame convention: " << (frame_convention() == LINE_IS_X ?
				    "LINE_IS_X\n" : "LINE_IS_Y\n")
     << "   Frame to spacecraft: " << frame_to_sc() << "\n";
}

