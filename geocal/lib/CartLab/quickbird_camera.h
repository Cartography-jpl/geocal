#ifndef QUICKBIRD_CAMERA_H
#define QUICKBIRD_CAMERA_H
#include "camera.h"
#include <boost/math/quaternion.hpp>

namespace GeoCal {
/****************************************************************//**
  This class models a the QuickBird camera. It is used to convert
  ScLookVector to FrameCoordinate and vice versa.

  The QuickBird camera is described by metadata supplied with the
  QuickBird data, a ".GEO" file. This is potentially dynamic, and
  potentially accounts for nonlinearities in the optics. However, in
  practice all of the GEO file we have are exactly the same, and all
  the nonlinear terms are set to 0. I'm not sure if this is because
  the camera really is very stable and the optics very linear, or just
  that DigitalGlobe didn't do a very good job at geometric camera
  calibration. In any case, rather than reading in this file we just
  hardwire the coefficients into this class. This is something we can
  revisit in the future if needed.

  This model is for the Panchromatic band only. We could easily extend
  this to other bands, but right now we only work with the
  Panchromatic data.
*******************************************************************/

class QuickBirdCamera : public PushBroomCamera {
public:
  QuickBirdCamera();

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~QuickBirdCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------

  virtual int number_band() const { return 1; }

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const { return 27552; }
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
private:
  double det_pitch;		///< Detector pitch in mm
  double det_origin_x;		///< Detector origin in mm
  double det_origin_y;		///< Detector origin in mm
  double focal_length;		///< Detector focal length in mm
  boost::math::quaternion<double> cam_to_sc;
				/// Rotate from camera
				/// coordinates to spacecraft coordinates.
};

}
#endif
