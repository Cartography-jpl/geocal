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

// Allow this class to be derived from in Python.
%feature("director") Camera;

// Note, a class that is derived from in python needs to declare every
// virtual function that can be called on it, even if all that happens
// is the base class to a director is called. This is because this
// class is used to create the SwigDirector class, and this class
// needs each of the member functions to direct things properly. It is
// *not* necessary to add these function to the underlying C++, only
// that you declare them here.
//
// For this particular class, this isn't an issue since this is the
// base class. But I'll keep this note here in case we are cutting
// and pasting to make another class a director
  
class Camera : public Observable<Camera>, public WithParameter {
public:
  Camera();
  virtual void add_observer(Observer<Camera>& Obs); 
  virtual void remove_observer(Observer<Camera>& Obs);
  virtual double integration_time(int Band) const;
  %python_attribute(number_band, virtual int)
  virtual int number_line(int Band) const = 0;
  virtual int number_sample(int Band) const = 0;
  virtual int number_band() const;
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
  virtual std::string print_to_string() const;
  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);
protected:
  void notify_update_do(const Camera& Self);
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
