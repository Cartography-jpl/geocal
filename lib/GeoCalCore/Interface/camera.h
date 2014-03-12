#ifndef CAMERA_H
#define CAMERA_H
#include "constant.h"
#include "frame_coordinate.h"
#include "look_vector.h"
#include <blitz/array.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class models a frame camera. It is used to convert ScLookVector
  to FrameCoordinate and vice versa.
*******************************************************************/

class Camera : public Printable<Camera> {
public:
//-----------------------------------------------------------------------
/// Direction camera is pointing.
//-----------------------------------------------------------------------

  enum Direction {FORWARD, AFTWARD};

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Camera() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Camera() {}

//-----------------------------------------------------------------------
/// This gives the camera direction. This is intended for use with
/// steep camera angles (e.g., MISR AF camera). For cameras that are
/// near nadir looking, we can just arbitrarily pick a direction for
/// it. 
///
/// The default version returns FORWARD.
//-----------------------------------------------------------------------

  virtual Direction direction() const { return FORWARD; }

//-----------------------------------------------------------------------
/// This is the integration time in seconds.
///
/// The integration time is how long it takes the camera to collect
/// data. It is important to model this, so that we can include
/// movement of the spacecraft while the camera is in the middle of
/// collecting data. If the integration time is very short, or if a
/// particular derived class doesn't wish to model this, then the
/// integration time can be set to 0 seconds.
///
/// The default version returns 0.0.
//-----------------------------------------------------------------------

  virtual double integration_time(int Band) const { return 0.0; }

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------

  virtual int number_band() const = 0;

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const = 0;

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const = 0;

//-----------------------------------------------------------------------
/// A camera model might depend on a set of parameters, which can by
/// modified (e.g., during a simultaneous bundle adjustment). This
/// returns those parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 1> parameter() const
  { // Default is no parameters.
    return blitz::Array<double, 1>(0); 
  }

//-----------------------------------------------------------------------
/// Set the value of the parameters.
//-----------------------------------------------------------------------

  virtual void parameter(const blitz::Array<double, 1>& Parm)
  {
    // Default is do nothing
  }

//-----------------------------------------------------------------------
/// Descriptive name of each parameter.
//-----------------------------------------------------------------------

  virtual std::vector<std::string> parameter_name() const
  {
    std::vector<std::string> res;
    return res;
  }

//-----------------------------------------------------------------------
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const = 0;

//-----------------------------------------------------------------------
/// This is similar to frame_coordinate, except it only calculates the
/// line coordinate. For some camera models, it is more expensive to
/// calculate the sample number than the line (e.g., MISR camera needs
/// to use the pixel-theta measurements). For functions that only need
/// the line information, it is wasteful to do the full calculation. 
///
/// The default version of this function just calls frame_coordinate
/// and returns the line portion, but a derived class can override
/// this default with a faster way of calculation the line
/// information. 
//-----------------------------------------------------------------------

  virtual double frame_line_coordinate(const ScLookVector& Sl, int Band) 
    const { return frame_coordinate(Sl, Band).line; }

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const = 0;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};

/****************************************************************//**
  This is a camera specialized to 1 line per band.
*******************************************************************/

class PushBroomCamera : public Camera {
public:
  virtual ~PushBroomCamera() {}

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const { return 1; }
};

/****************************************************************//**
  This is a simple PushBroomCamera. It is not intended as a realistic
  camera model, but rather as supplying a simple class to use for
  testing. The defaults are for the nominal MISR DF camera, red band.
*******************************************************************/

class SimplePushBroomCamera : public PushBroomCamera {
public:
  SimplePushBroomCamera(double Beta=58*Constant::deg_to_rad, double
     Delta=-2.7*Constant::deg_to_rad, double Epsilon=0, 
     double Focal=123.8e-3, double
     line_pitch=18e-6, double sample_pitch=21e-6, int Number_sample=1504);
  virtual ~SimplePushBroomCamera() {}

//-----------------------------------------------------------------------
/// This gives the camera direction. 
//-----------------------------------------------------------------------

  virtual Camera::Direction direction() const 
  { return (beta_ >= 0 ? Camera::FORWARD : Camera::AFTWARD); }

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------

  virtual int number_band() const {return 1;}

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const {return nsample;}
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
  double beta() const {return beta_;}
  double delta() const {return delta_;}
  double epsilon() const {return epsilon_;}
  double focal_length() const {return focal_;}
  double line_pitch() const {return line_pitch_;}
  double sample_pitch() const {return sample_pitch_;}
private:
  double beta_, delta_, epsilon_, focal_, line_pitch_, sample_pitch_;
  int nsample;
  double r[3][3];		///< Rotation matrix.
};

}
#endif
