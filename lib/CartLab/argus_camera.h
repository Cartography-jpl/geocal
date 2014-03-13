#ifndef ARGUS_CAMERA_H
#define ARGUS_CAMERA_H
#include "printable.h"
#include "quaternion_camera.h"
#include "geocal_exception.h"

namespace GeoCal {
/****************************************************************//**
  A model of the ARGUS camera. Right now, this doesn't account for
  any lens distortion, we model this as a pinhole camera. We may need
  to revisit this.
*******************************************************************/

class ArgusCamera : public QuaternionCamera {
public:
  ArgusCamera(double Yaw = 0, double Pitch = 0, double Roll = 0, 
	      double Focal_length = 400);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ArgusCamera() {}

//-----------------------------------------------------------------------
/// Roll angle, in degrees.
//-----------------------------------------------------------------------

  double roll() const {return roll_;}

//-----------------------------------------------------------------------
/// Pitch angle, in degrees.
//-----------------------------------------------------------------------

  double pitch() const {return pitch_;}

//-----------------------------------------------------------------------
/// Yaw angle, in degrees.
//-----------------------------------------------------------------------

  double yaw() const {return yaw_;}

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
private:
  double roll_; 		// Roll angle, in degrees.
  double pitch_; 		// Pitch angle, in degrees.
  double yaw_; 	        	// Yaw angle, in degrees.
};
}
#endif

