#include "glas_gfm_camera.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GlasGfmCamera::save(Archive& Ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void GlasGfmCamera::load(Archive& Ar, const unsigned int version)
{
  frame_to_sc_nd_ = value(frame_to_sc_);
}

template<class Archive>
void GlasGfmCamera::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Camera);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Camera, WithParameter);
  GEOCAL_BASE(GlasGfmCamera, Camera);
  ar & GEOCAL_NVP_(focal_length)
    & GEOCAL_NVP2("number_line", nline_)
    & GEOCAL_NVP2("number_sample", nsamp_)
    & GEOCAL_NVP_(frame_to_sc)
    & GEOCAL_NVP_(band_type) & GEOCAL_NVP_(band_wavelength)
    & GEOCAL_NVP_(band_index) & GEOCAL_NVP_(irepband)
    & GEOCAL_NVP_(isubcat) & GEOCAL_NVP_(sample_number_first)
    & GEOCAL_NVP_(delta_sample_pair) & GEOCAL_NVP_(field_alignment);
  boost::serialization::split_member(ar, *this, version);
}


GEOCAL_IMPLEMENT(GlasGfmCamera);
#endif

  
GlasGfmCamera::GlasGfmCamera()
{
  // These default values come from the RIP sample NITF file for Hyperion.
  focal_length_ = 1.41009182;
  nline_ = 1;
  nsamp_ = 256;
  frame_to_sc_ = boost::math::quaternion<double>(1,0,0,0);
  frame_to_sc_nd_ = value(frame_to_sc_);
  field_alignment_.resize(1, 4);
  field_alignment_ = -0.00765, 0, 0.00765, 0;
  band_type_ = "N";
  band_wavelength_ = 1.45;
  // Leave these as empty
  // band_index_, irepband_, isubcat_.
  focal_length_time_ = Time::parse_time("2005-04-07T00:00:00Z") +
    26649.88899999857;
  sample_number_first_ = 0;
  delta_sample_pair_ = 256;
}

FrameCoordinate GlasGfmCamera::frame_coordinate(const ScLookVector& Sl, 
						int Band) const
{
  range_check(Band, 0, number_band());

  // Just reverse of sc_look_vector.
  boost::math::quaternion<double> dcs = 
    conj(frame_to_sc()) * Sl.look_quaternion() * frame_to_sc();
  double xfp = focal_length() * (-dcs.R_component_3() / dcs.R_component_4());
  double yfp = focal_length() * (dcs.R_component_2() / dcs.R_component_4());
  FrameCoordinate res;
  res.line = xfp / (0.00765 / 128) + 0;;
  res.sample = yfp / (0.00765 / 128) + 128;;
  return res;
}

FrameCoordinateWithDerivative GlasGfmCamera::frame_coordinate_with_derivative
(const ScLookVectorWithDerivative& Sl, int Band) const
{
  range_check(Band, 0, number_band());

  boost::math::quaternion<AutoDerivative<double> > dcs = 
    conj(frame_to_sc_with_derivative()) * Sl.look_quaternion() * 
    frame_to_sc_with_derivative();
  AutoDerivative<double> xfp = focal_length_with_derivative() *
    (-dcs.R_component_3() / dcs.R_component_4());
  AutoDerivative<double> yfp = focal_length_with_derivative() *
    (dcs.R_component_2() / dcs.R_component_4());
  FrameCoordinateWithDerivative res;
  res.line = xfp / (0.00765 / 128) + 0;;
  res.sample = yfp / (0.00765 / 128) + 128;;
  return res;
}

ScLookVector GlasGfmCamera::sc_look_vector
(const FrameCoordinate& F, int Band) const
{
  range_check(Band, 0, number_band());
  double xfp = (F.line - 0) * 0.00765 / 128;
  double yfp = (F.sample - 128) * 0.00765 / 128;
  // Coordinate system is rotated relative to GlasGfm
  boost::math::quaternion<double> lv(0, yfp, -xfp, focal_length());  
  return ScLookVector(frame_to_sc() * lv * conj(frame_to_sc()));
}

ScLookVectorWithDerivative GlasGfmCamera::sc_look_vector_with_derivative
(const FrameCoordinateWithDerivative& F, int Band) const
{
  range_check(Band, 0, number_band());
  AutoDerivative<double> xfp = (F.line - 0) * 0.00765 / 128;
  AutoDerivative<double> yfp = (F.sample - 128) * 0.00765 / 128;
  boost::math::quaternion<AutoDerivative<double> >
    lv(0, yfp, -xfp, focal_length_with_derivative());  
  return ScLookVectorWithDerivative(frame_to_sc_with_derivative() * lv *
				    conj(frame_to_sc_with_derivative()));
}

//-----------------------------------------------------------------------
/// Angular sensor frame offset. This is in radians, and is the order
/// "xyz".
//-----------------------------------------------------------------------

blitz::Array<double, 1> GlasGfmCamera::angoff() const
{
  blitz::Array<double, 1> res(3);
  quat_to_euler(frame_to_sc(), res(2), res(1), res(0));
  return res;
}

void GlasGfmCamera::angoff(const blitz::Array<double, 1>& V)
{
  if(V.rows() != 3)
    throw Exception("angoff must be size 3");
  frame_to_sc(quat_rot("zyx", V(2), V(1), V(0)));
  notify_update();
}
