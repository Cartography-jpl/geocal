#include "quaternion_camera.h"
using namespace GeoCal;

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
  range_check(Band, 0, 1);

  // Just reverse of sc_look_vector.
  boost::math::quaternion<double> fv = conj(frame_to_sc_) * Sl.look_quaternion() * frame_to_sc_;
  FrameCoordinate fc;
  fc.sample = pp_.sample +
    sample_scale() * 
    focal_length() * (fv.R_component_2() / fv.R_component_4()) / sample_pitch();
  fc.line = pp_.line +
    line_scale() * 
    focal_length() * (fv.R_component_3() / fv.R_component_4()) / line_pitch();
  return fc;
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
  range_check(Band, 0, 1);
  ScLookVector sl((F.sample - pp_.sample) * sample_pitch() / sample_scale(),
		  (F.line - pp_.line) * line_pitch() / line_scale(),
		  focal_length());
  sl.look_quaternion(frame_to_sc_ * sl.look_quaternion() * conj(frame_to_sc_));
  return sl;
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
     << "   Line scale:      " << line_scale() << "\n"
     << "   Sample scale:    " << sample_scale() << "\n"
     << "   Principal point: " << principal_point() << "\n"
     << "   Frame to spacecraft: " << frame_to_sc() << "\n";
}

