#ifndef CAMERA_H
#define CAMERA_H
#include "constant.h"
#include "frame_coordinate.h"
#include "look_vector.h"
#include "observer.h"
#include "array_ad.h"
#include "with_parameter.h"
#include "geocal_quaternion.h"
#include <blitz/array.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class models a frame camera. It is used to convert ScLookVector
  to FrameCoordinate and vice versa.

  Other objects may depend on the Camera, and should be updated
  when the Camera is updated. To facilitate that, this class in
  an Oberverable, and objects can add themselves as Observers to be
  notified when the Camera is updated.
*******************************************************************/

class Camera : public Printable<Camera>, 
	       public Observable<Camera>,
	       public WithParameter {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Camera() {}

  virtual void add_observer(Observer<Camera>& Obs) 
  { add_observer_do(Obs, *this);}
  virtual void remove_observer(Observer<Camera>& Obs) 
  { remove_observer_do(Obs, *this);}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Camera() {}

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

  virtual double integration_time(int UNUSED(Band)) const { return 0.0; }

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
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const = 0;

//-----------------------------------------------------------------------
/// Variation of frame_coordinate that both propagate derivative
/// information in the ScLookVector and adds in any derivatives from
/// the parameters.
//-----------------------------------------------------------------------

  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
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
/// Variation of sc_look_vector that both propagate derivative
/// information in the FrameCoordinate and adds in any derivatives from
/// the parameters.
//-----------------------------------------------------------------------

  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const = 0;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a simple Camera. It is not intended as a realistic
  camera model, but rather as supplying a simple class to use for
  testing. The defaults are for the nominal MISR DF camera, red band.
*******************************************************************/

class SimpleCamera : public Camera {
public:
  SimpleCamera(double Beta=58*Constant::deg_to_rad, double
     Delta=-2.7*Constant::deg_to_rad, double Epsilon=0, 
     double Focal=123.8e-3, double
     line_pitch=18e-6, double sample_pitch=21e-6, 
     int Number_line = 1, 
     int Number_sample=1504);
  virtual ~SimpleCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------

  virtual int number_band() const {return 1;}

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int UNUSED(Band)) const {return nline;}

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int UNUSED(Band)) const {return nsample;}
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
				   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
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
  int nline,nsample;
  boost::math::quaternion<double> frame_to_sc;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Camera);
GEOCAL_EXPORT_KEY(SimpleCamera);
#endif
