// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "camera.h"
%}
%base_import(generic_object)
%base_import(observer)
%base_import(with_parameter)
%import "frame_coordinate.i"
%import "look_vector.i"
%import "geocal_time.i"
%include "geocal_time_include.i"
%import "array_ad.i"
%geocal_shared_ptr(GeoCal::Camera);
%geocal_shared_ptr(GeoCal::SubCamera);
%geocal_shared_ptr(GeoCal::SimpleCamera);
namespace GeoCal {
  class Camera;
}

%geocal_shared_ptr(GeoCal::Observable<GeoCal::Camera>);
%geocal_shared_ptr(GeoCal::Observer<GeoCal::Camera>);

namespace GeoCal {
%template(ObservableCamera) GeoCal::Observable<GeoCal::Camera>;
%template(ObserverCamera) GeoCal::Observer<GeoCal::Camera>;

class Camera : public Observable<Camera>, public WithParameter {
public:
  Camera();
  virtual void add_observer(Observer<Camera>& Obs); 
  virtual void remove_observer(Observer<Camera>& Obs);
  virtual double integration_time(int Band) const;
  %python_attribute(number_band, virtual int)
  virtual int number_line(int Band) const = 0;
  virtual int number_sample(int Band) const = 0;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const = 0;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
		   int Band) const = 0;
  virtual double frame_line_coordinate(const ScLookVector& Sl, int Band) 
    const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
    int Band) const = 0;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const = 0;
  std::string print_to_string() const;
};

class SubCamera : public Camera {
public:
  SubCamera(const boost::shared_ptr<Camera>& Cam,
	    int Start_line, int Start_sample, int Number_line,
	    int Number_sample);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
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
  %python_attribute(full_camera, boost::shared_ptr<Camera>);
  %python_attribute(start_line, int);
  %python_attribute(start_sample, int);
};
  
class SimpleCamera : public Camera {
public:
  // swig 3.0.7 doesn't seem to support expressions in the
  // default argument value (this use to work in the older swig 2.x series).
  // As an easy workaround, just have multiple versions of this w/o
  // the default values.
  // SimpleCamera(double Beta=58*Constant::deg_to_rad, double
  //    Delta=-2.7*Constant::deg_to_rad, double Epsilon=0, 
  //    double Focal=123.8e-3, double
  // 	       Pitch_line=18e-6, double Pitch_sample=21e-6, int Number_line = 1,
  // 	       int Number_sample=1504);
  SimpleCamera();
  SimpleCamera(double Beta);
  SimpleCamera(double Beta, double Delta, double Epsilon=0, 
	       double Focal=123.8e-3, double
	       Pitch_line=18e-6, double Pitch_sample=21e-6, int Number_line = 1,
	       int Number_sample=1504);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
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
  %python_attribute(beta, double)
  %python_attribute(delta, double)
  %python_attribute(epsilon, double)
  %python_attribute(focal_length, double)
  %python_attribute(line_pitch, double)
  %python_attribute(sample_pitch, double)
  %pickle_serialization();
};
}
%template(Vector_Camera) std::vector<boost::shared_ptr<GeoCal::Camera> >;

// List of things "import *" will include
%python_export("ObserverCamera", "ObservableCamera", "Camera", "SimpleCamera", "Vector_Camera",
	       "SubCamera")
