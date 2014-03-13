#include "argus_camera.h"
#ifdef HAVE_GDAL
#include "gdal_raster_image.h"
#endif
#include <sstream>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Create a ARGUS camera. We read one of the image jpeg files to get
/// camera metadata used to describe the camera. Yaw, Pitch and Roll
/// are in degrees, Focal_length is in mm.
//-----------------------------------------------------------------------

ArgusCamera::ArgusCamera(double Yaw, double Pitch, double Roll, 
			 double Focal_length)
: roll_(Roll),
    pitch_(Pitch),
    yaw_(Yaw)
{
  focal_length_ = Focal_length;
  nline_ = 3744;
  nsamp_ = 5616;
  frame_convention_ = QuaternionCamera::LINE_IS_X;
  
  // For now, hardwire this to fit a sensor size of 36.0 x 24.0 mm.
  // This is the actual image sensor size according to canon, and may
  // be more accurate than what we are reading from the jpeg. We'll
  // want to examine this more carefully.

  line_pitch_ = 24.0 / number_line(0);
  sample_pitch_ = 36.0 / number_sample(0);

  frame_to_sc_ = quat_rot("ZYX", Yaw * Constant::deg_to_rad, 
			  Pitch * Constant::deg_to_rad,
			 Roll * Constant::deg_to_rad);
}

//-----------------------------------------------------------------------
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

FrameCoordinate ArgusCamera::frame_coordinate(const ScLookVector& Sl, 
					      int Band) const
{
  range_check(Band, 0, 1);

  // Just reverse of sc_look_vector.
  boost::math::quaternion<double> fv = conj(frame_to_sc_) * Sl.look_quaternion() * frame_to_sc_;
  FrameCoordinate fc;
  fc.line = number_line(0) / 2.0 -
    focal_length() * (fv.R_component_2() / fv.R_component_4()) / line_pitch();
  fc.sample = number_sample(0) / 2.0 +
    focal_length() * (fv.R_component_3() / fv.R_component_4()) / sample_pitch();
  return fc;
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

ScLookVector ArgusCamera::sc_look_vector(const FrameCoordinate& F, 
					 int Band) const
{
  range_check(Band, 0, 1);

  // We assume a principal point at the center of the camera. Also,
  // these are actually body coordinates, but we use the ScLookVector
  // because this is essentially the same thing.

  ScLookVector sl(-(F.line - number_line(0) / 2.0) * line_pitch(),
		  (F.sample - number_sample(0) / 2.0) * sample_pitch(),
		  focal_length());
  sl.look_quaternion(frame_to_sc_ * sl.look_quaternion() * conj(frame_to_sc_));
  return sl;
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void ArgusCamera::print(std::ostream& Os) const
{
  Os << "ArgusCamera:\n"
     << "   Number line:     " << number_line(0) << "\n"
     << "   Number sample:   " << number_sample(0) << "\n"
     << "   Focal length:    " << focal_length() << " mm\n"
     << "   Line pitch:      " << line_pitch() << " mm\n"
     << "   Sample pitch:    " << sample_pitch() << " mm\n"
     << "   Yaw:             " << yaw() << " deg\n"
     << "   Pitch:           " << pitch() << " deg\n"
     << "   Roll:            " << roll() << " deg\n";
}
