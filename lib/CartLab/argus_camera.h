#ifndef ARGUS_CAMERA_H
#define ARGUS_CAMERA_H
#include "printable.h"
#include "camera.h"
#include "geocal_exception.h"

namespace GeoCal {
/****************************************************************//**
  A model of the ARGUS camera. Right now, this doesn't account for
  any lens distortion, we model this as a pinhole camera. We may need
  to revisit this.
*******************************************************************/
class ArgusCamera : public Camera {
public:
  ArgusCamera(double Yaw = 0, double Pitch = 0, double Roll = 0, 
	      double Focal_length = 400);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ArgusCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera. We don't treat the bands separately (at
/// least for now), so this is set to 1.
//-----------------------------------------------------------------------

  virtual int number_band() const { return 1; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const
  { range_check(Band, 0, 1); return nline_; }

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const
  { range_check(Band, 0, 1); return nsamp_; }

//-----------------------------------------------------------------------
/// Focal length, in mm.
//-----------------------------------------------------------------------

  double focal_length() const {return focal_length_;}

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double line_pitch() const {return line_pitch_; }

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

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double sample_pitch() const {return sample_pitch_;}

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
private:
  double focal_length_;		// Focal length, in mm.
  int nline_;			// Number of lines in camera.
  int nsamp_;			// Number of samples in camera.
  double line_pitch_;		// CCD pitch, in mm
  double sample_pitch_;		// CCD pitch, in mm
  double roll_; 		// Roll angle, in degrees.
  double pitch_; 		// Pitch angle, in degrees.
  double yaw_; 	        	// Yaw angle, in degrees.
  boost::math::quaternion<double> frame_to_sc;
};
}
#endif

