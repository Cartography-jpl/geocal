#include "quickbird_camera.h"
#include "geocal_exception.h"

using namespace GeoCal;
using boost::math::quaternion;

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

  focal_length = 8836.202;
  det_origin_x = 9.5468399999999995;
  det_origin_y = 164.0277299999999900;
  det_pitch = 0.0119139600000000;

  // Note that there are a few different conventions about the
  // ordering of the quaternion coefficients. The boost library places
  // the real part at the front, so we have a + b i + c j + d k and
  // the quaternion is 4-tuple (a, b, c, d). The convention used by
  // quickbird data  is q1 i + q2 j + q3 k + q4 with the 4-tuple is
  // (q1, q2, q3, q4). That means when we bring this over to the boost
  // library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

  cam_to_sc = quaternion<double>(0.9999806843019142,
				 -0.0037369299279122,
				 -0.0046602381421084,
				 0.0017171366422757);
}

//-----------------------------------------------------------------------
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

FrameCoordinate QuickBirdCamera::frame_coordinate(const ScLookVector& Sl, 
						  int Band) const
{
  range_check(Band, 0, number_band());

//-----------------------------------------------------------------------
// Convert from space craft coordinates to camera coordinates. 
//-----------------------------------------------------------------------

  quaternion<double> sc_coor = Sl.look_quaternion();

//-----------------------------------------------------------------------
// Rotate to camera coordinates.
//-----------------------------------------------------------------------

  quaternion<double> cam_coor = conj(cam_to_sc) * sc_coor * cam_to_sc;
  
//-----------------------------------------------------------------------
// Convert from camera coordinates to detector coordinates.
//-----------------------------------------------------------------------

  FrameCoordinate fc;
  fc.line = ((focal_length * cam_coor.R_component_2() / 
	      cam_coor.R_component_4()) - det_origin_x) / det_pitch;
  fc.sample = -((focal_length * cam_coor.R_component_3() / 
		 cam_coor.R_component_4()) - det_origin_y) / det_pitch;
  return fc;
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

ScLookVector QuickBirdCamera::sc_look_vector(const FrameCoordinate& F, 
					     int Band) const
{
  range_check(Band, 0, number_band());

//-----------------------------------------------------------------------
// First convert from FrameCoordinates (same thing as detector
// coordinates) to camera coordinates. Note that in general there can
// be a rotation, however this isn't implemented since it isn't used in
// the GEO files we have.
//
// We pack the vector into a quaternion, so we can rotate it in the
// next step.
//-----------------------------------------------------------------------

  quaternion<double> cam_coor(0,
			      F.line * det_pitch + det_origin_x,
			      -F.sample * det_pitch + det_origin_y,
			      focal_length);

//-----------------------------------------------------------------------
// This rotates cam_coor with the inverse of sc_to_cam.
//-----------------------------------------------------------------------

  quaternion<double> sc_coor = cam_to_sc * cam_coor * conj(cam_to_sc);

  ScLookVector res;
  res.look_quaternion(sc_coor);
  return res;
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuickBirdCamera::print(std::ostream& Os) const
{
  Os << "QuickBird Panchromatic camera model\n   (model generation time 2003-05-23T10:53:15.000000Z)\n";
}

