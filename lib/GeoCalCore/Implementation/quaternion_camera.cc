#include "quaternion_camera.h"
#include "geocal_serialize_support.h"
#include <boost/lexical_cast.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  template<class Archive>
void QuaternionCamera::save(Archive& Ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void QuaternionCamera::load(Archive& Ar, const unsigned int version)
{
  frame_to_sc_nd_ = value(frame_to_sc_);
}

template<class Archive>
void QuaternionCamera::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Camera);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Camera, WithParameter);
  GEOCAL_BASE(QuaternionCamera, Camera);
  ar & GEOCAL_NVP_(focal_length)
    & GEOCAL_NVP2("number_band", nband_)
    & GEOCAL_NVP2("number_line", nline_)
    & GEOCAL_NVP2("number_sample", nsamp_)
    & GEOCAL_NVP_(line_pitch)
    & GEOCAL_NVP_(sample_pitch)
    & GEOCAL_NVP_(principal_point)
    & GEOCAL_NVP_(frame_to_sc)
    & GEOCAL_NVP_(frame_convention)
    & GEOCAL_NVP_(line_direction)
    & GEOCAL_NVP_(sample_direction)
    & GEOCAL_NVP_(parameter_mask);
  // Older version didn't have optical_axis_direction
  if(version > 0) {
    ar & GEOCAL_NVP_(optical_axis_direction);
  }
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(QuaternionCamera);
#endif

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

void QuaternionCamera::dcs_to_focal_plane
(int Band, const boost::math::quaternion<AutoDerivative<double> >& Dcs,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  Xfp = focal_length_with_derivative() * 
    (Dcs.R_component_2() / Dcs.R_component_4());
  Yfp = focal_length_with_derivative() * 
    (Dcs.R_component_3() / Dcs.R_component_4());
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
QuaternionCamera::focal_plane_to_dcs(int Band, double Xfp, double Yfp) const
{
  return boost::math::quaternion<double>(0, Xfp, Yfp,
			 optical_axis_dir() * focal_length());
}

boost::math::quaternion<AutoDerivative<double>  >
QuaternionCamera::focal_plane_to_dcs
(int Band,const AutoDerivative<double>& Xfp, 
 const AutoDerivative<double>& Yfp) const
{
  return boost::math::quaternion<AutoDerivative<double> >(0, Xfp, Yfp, 
		  optical_axis_dir() * focal_length_with_derivative());
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
    conj(frame_to_sc()) * Sl.look_quaternion() * frame_to_sc();
  double xfp, yfp;
  dcs_to_focal_plane(Band, dcs, xfp, yfp);
  return focal_plane_to_fc(Band, xfp, yfp);
}

FrameCoordinateWithDerivative QuaternionCamera::frame_coordinate_with_derivative
(const ScLookVectorWithDerivative& Sl, 
 int Band) const
{
  range_check(Band, 0, number_band());

  // Just reverse of sc_look_vector.
  boost::math::quaternion<AutoDerivative<double> > dcs = 
    conj(frame_to_sc_with_derivative()) * Sl.look_quaternion() * 
    frame_to_sc_with_derivative();
  AutoDerivative<double> xfp, yfp;
  dcs_to_focal_plane(Band, dcs, xfp, yfp);
  return focal_plane_to_fc(Band, xfp, yfp);
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
  fc_to_focal_plane(F, Band, xfp, yfp);
  return DcsLookVector(focal_plane_to_dcs(Band, xfp, yfp));
}

DcsLookVectorWithDerivative 
QuaternionCamera::dcs_look_vector(const FrameCoordinateWithDerivative& F, 
				  int Band) const
{
  range_check(Band, 0, number_band());
  AutoDerivative<double> xfp, yfp;
  fc_to_focal_plane(F, Band, xfp, yfp);
  return DcsLookVectorWithDerivative(focal_plane_to_dcs(Band, xfp, yfp));
}

//-----------------------------------------------------------------------
/// Convert focal plane coordinates to FrameCoordinate
//-----------------------------------------------------------------------

FrameCoordinate QuaternionCamera::focal_plane_to_fc(int Band, double Xfp,
						    double Yfp) const
{
  range_check(Band, 0, number_band());
  FrameCoordinate fc;
  if(frame_convention_ == LINE_IS_Y) {
    fc.sample = principal_point(Band).sample + 
      Xfp / sample_pitch() * samp_dir();
    fc.line = principal_point(Band).line +
      Yfp / line_pitch() * line_dir();
  } else {
    fc.sample = principal_point(Band).sample +
      Yfp / sample_pitch() * samp_dir();
    fc.line = principal_point(Band).line +
      Xfp / line_pitch() * line_dir();
  }
  return fc;
}

//-----------------------------------------------------------------------
/// Convert focal plane coordinates to FrameCoordinateWithDerivative
//-----------------------------------------------------------------------

FrameCoordinateWithDerivative QuaternionCamera::focal_plane_to_fc
(int Band, const AutoDerivative<double>& Xfp,
 const AutoDerivative<double>& Yfp) const
{
  range_check(Band, 0, number_band());
  FrameCoordinateWithDerivative fc;
  if(frame_convention_ == LINE_IS_Y) {
    fc.sample = principal_point_with_derivative(Band).sample + 
      Xfp / sample_pitch_with_derivative() * samp_dir();
    fc.line = principal_point_with_derivative(Band).line +
      Yfp / line_pitch_with_derivative() * line_dir();
  } else {
    fc.sample = principal_point_with_derivative(Band).sample +
      Yfp / sample_pitch_with_derivative() * samp_dir();
    fc.line = principal_point_with_derivative(Band).line +
      Xfp / line_pitch_with_derivative() * line_dir();
  }
  return fc;
}
  
//-----------------------------------------------------------------------
/// Convert FrameCoordinate to focal plane coordinates.
//-----------------------------------------------------------------------

void QuaternionCamera::fc_to_focal_plane(const FrameCoordinate& Fc, int Band,
					 double& Xfp, double& Yfp) const
{
  range_check(Band, 0, number_band());
  if(frame_convention_ == LINE_IS_Y) {
    Xfp = (Fc.sample - principal_point(Band).sample) * sample_pitch() * 
      samp_dir();
    Yfp = (Fc.line - principal_point(Band).line) * line_pitch()
      * line_dir();
  } else {
    Yfp = (Fc.sample - principal_point(Band).sample) * sample_pitch() * 
      samp_dir();
    Xfp = (Fc.line - principal_point(Band).line) * line_pitch()
      * line_dir();
  }
}

//-----------------------------------------------------------------------
/// Convert FrameCoordinateWithDerivative to focal plane coordinates.
//-----------------------------------------------------------------------

void QuaternionCamera::fc_to_focal_plane
(const FrameCoordinateWithDerivative& Fc, int Band,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  range_check(Band, 0, number_band());
  if(frame_convention_ == LINE_IS_Y) {
    Xfp = (Fc.sample - principal_point_with_derivative(Band).sample) * 
      sample_pitch_with_derivative() * samp_dir();
    Yfp = (Fc.line - principal_point_with_derivative(Band).line) * 
      line_pitch_with_derivative() * line_dir();
  } else {
    Yfp = (Fc.sample - principal_point_with_derivative(Band).sample) * 
      sample_pitch_with_derivative() * samp_dir();
    Xfp = (Fc.line - principal_point_with_derivative(Band).line) * 
      line_pitch_with_derivative() * line_dir();
  }
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
  return ScLookVector(frame_to_sc() * 
		      dcs_look_vector(F, Band).look_quaternion() *
		      conj(frame_to_sc()));
}

ScLookVectorWithDerivative QuaternionCamera::sc_look_vector_with_derivative
(const FrameCoordinateWithDerivative& F, 
 int Band) const
{
  return ScLookVectorWithDerivative(frame_to_sc_with_derivative() * 
		      dcs_look_vector(F, Band).look_quaternion() *
		      conj(frame_to_sc_with_derivative()));
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
     << "   Line direction:   " << (line_direction() == INCREASE_IS_POSITIVE ?
				    "INCREASE_IS_POSITIVE\n" :
				    "INCREASE_IS_NEGATIVE\n")
     << "   Sample direction:  " << (sample_direction() == INCREASE_IS_POSITIVE ?
				    "INCREASE_IS_POSITIVE\n" :
				    "INCREASE_IS_NEGATIVE\n")
     << "   Optical axis dir:  " << (optical_axis_direction() ==
				     OPTICAL_AXIS_IS_POSITIVE ?
				     "OPTICAL_AXIS_IS_POSITIVE\n" :
				     "OPTICAL_AXIS_IS_NEGATIVE\n")
     << "   Frame to spacecraft: " << frame_to_sc() << "\n";
}

//-----------------------------------------------------------------------
/// Set parameter. Right now this is Euler epsilon, beta, delta, line
/// pitch, sample pitch. We may well want to play with this in the
/// future, turning individual things on an off. But for now we'll just
/// have the set of values we've found useful to fit for.
//-----------------------------------------------------------------------

void QuaternionCamera::parameter(const blitz::Array<double, 1>& Parm)
{
  if(Parm.rows() != 6 + 2 * number_band())
    throw Exception("Wrong sized parameter passed.");
  line_pitch_ = Parm(3);
  sample_pitch_ = Parm(4);
  focal_length_ = Parm(5);
  for(int b = 0; b < number_band(); ++b)
    principal_point(b, FrameCoordinate(Parm(6 + b * 2), Parm(7 + b * 2)));
  // euler calls notify_update(), so we don't need to do that.
  euler(Parm(blitz::Range(0,2)));
}

void QuaternionCamera::parameter_with_derivative(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != 6 + 2 * number_band())
    throw Exception("Wrong sized parameter passed.");
  line_pitch_ = Parm(3);
  sample_pitch_ = Parm(4);
  focal_length_ = Parm(5);
  for(int b = 0; b < number_band(); ++b)
    principal_point_with_derivative(b, FrameCoordinateWithDerivative(Parm(6 + b * 2), Parm(7 + b * 2)));
  // euler calls notify_update(), so we don't need to do that.
  euler_with_derivative(Parm(blitz::Range(0,2)));
}

blitz::Array<double, 1> QuaternionCamera::parameter() const
{
  blitz::Array<double, 1> res(6 + 2 * number_band());
  res(blitz::Range(0,2)) = euler();
  res(3) = line_pitch();
  res(4) = sample_pitch();
  res(5) = focal_length();
  for(int b = 0; b < number_band(); ++b) {
    res(6 + b * 2) = principal_point(b).line;
    res(7 + b * 2) = principal_point(b).sample;
  }
  return res;
}

ArrayAd<double, 1> QuaternionCamera::parameter_with_derivative() const
{
  blitz::Array<AutoDerivative<double>, 1> res(6 + 2 * number_band());
  res(blitz::Range(0,2)) = euler_with_derivative();
  res(3) = line_pitch_with_derivative();
  res(4) = sample_pitch_with_derivative();
  res(5) = focal_length_with_derivative();
  for(int b = 0; b < number_band(); ++b) {
    res(6 + b * 2) = principal_point_with_derivative(b).line;
    res(7 + b * 2) = principal_point_with_derivative(b).sample;
  }
  return ArrayAd<double, 1>(res);
}

std::vector<std::string> QuaternionCamera::parameter_name() const
{
  std::vector<std::string> res;
  res.push_back("Camera Euler Epsilon");
  res.push_back("Camera Euler Beta");
  res.push_back("Camera Euler Delta");
  res.push_back("Camera line pitch");
  res.push_back("Camera sample pitch");
  res.push_back("Camera focal length");
  for(int b = 0; b < number_band(); ++b) {
    res.push_back("Camera band " + boost::lexical_cast<std::string>(b) +
		  " principal point line");
    res.push_back("Camera band " + boost::lexical_cast<std::string>(b) +
		  " principal point sample");
  }
  return res;
}

