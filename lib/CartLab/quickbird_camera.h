#ifndef QUICKBIRD_CAMERA_H
#define QUICKBIRD_CAMERA_H
#include "quaternion_camera.h"

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

class QuickBirdCamera : public QuaternionCamera {
public:
  QuickBirdCamera();

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~QuickBirdCamera() {}

  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(QuickBirdCamera);
#endif
