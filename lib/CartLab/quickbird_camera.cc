#include "quickbird_camera.h"
#include "geocal_exception.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using boost::math::quaternion;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void QuickBirdCamera::serialize(Archive & ar, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
}

GEOCAL_IMPLEMENT(QuickBirdCamera);
#endif

//-----------------------------------------------------------------------
/// Constructor. At this point, all of the camera coefficients are
/// hardwired in this class. We can change this in the future if
/// needed. 
//-----------------------------------------------------------------------

QuickBirdCamera::QuickBirdCamera() 
{
  // These values come from the model generated
  // 2003-05-23T10:53:15.000000Z. This was read from a GEO text file
  // (I used 07NOV24034231-P1BS-005695183010_01_P001.GEO, but all the
  // GEO file we have are same.

  // The full model allows for rotation of the detector, a nonlinear
  // correction of the optics, and moving the center of perspective.
  // None of this is present in the GEO file we are using as input, so
  // we haven't implemented this.

  focal_length_ = 8836.202;
  nband_ = 1;
  nline_ = 1;
  nsamp_ = 27552;
  line_pitch_ = 0.0119139600000000;
  sample_pitch_ = line_pitch_;
  double det_origin_x = 9.5468399999999995;
  double det_origin_y = 164.0277299999999900;
  principal_point_.push_back(FrameCoordinate(-det_origin_x / line_pitch(),
					     +det_origin_y / sample_pitch()));
  frame_convention_ = QuaternionCamera::LINE_IS_X;
  line_direction_ = QuaternionCamera::INCREASE_IS_POSITIVE;
  sample_direction_ = QuaternionCamera::INCREASE_IS_NEGATIVE;

  // Note that there are a few different conventions about the
  // ordering of the quaternion coefficients. The boost library places
  // the real part at the front, so we have a + b i + c j + d k and
  // the quaternion is 4-tuple (a, b, c, d). The convention used by
  // quickbird data  is q1 i + q2 j + q3 k + q4 with the 4-tuple is
  // (q1, q2, q3, q4). That means when we bring this over to the boost
  // library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

  frame_to_sc(quaternion<double>(0.9999806843019142,
				 -0.0037369299279122,
				 -0.0046602381421084,
				 0.0017171366422757));
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuickBirdCamera::print(std::ostream& Os) const
{
  Os << "QuickBird Panchromatic camera model\n   (model generation time 2003-05-23T10:53:15.000000Z)\n";
}

