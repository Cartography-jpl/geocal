// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "orbit.h"
%}
%base_import(generic_object)
%base_import(observer)
%base_import(with_parameter)
%import "camera.i"
%import "ground_coordinate.i"
%import "look_vector.i"
%import "dem.i"
%import "auto_derivative.i"
%import "array_ad.i"
%geocal_shared_ptr(GeoCal::OrbitData);
%geocal_shared_ptr(GeoCal::QuaternionOrbitData);
%geocal_shared_ptr(GeoCal::Orbit);
%geocal_shared_ptr(GeoCal::KeplerOrbit);

namespace GeoCal {
  class Orbit;
}

%geocal_shared_ptr(GeoCal::Observable<GeoCal::Orbit>);
%geocal_shared_ptr(GeoCal::Observer<GeoCal::Orbit>);

namespace GeoCal {
class OrbitData : public GenericObject {
public:
  double resolution_meter(const Camera& C, int Band = 0) const;
  double OrbitData::resolution_meter(const Camera& C, const FrameCoordinate& Fc,
				     int Band) const;
  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const = 0;
  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const ScLookVectorWithDerivative& Sl) 
    const = 0;
  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const = 0;
  virtual CartesianFixedLookVectorWithDerivative
  cf_look_vector(const ScLookVectorWithDerivative& Sl) 
    const = 0;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const = 0;
  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianInertialLookVectorWithDerivative& Ci) const = 0;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const = 0;
  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianFixedLookVectorWithDerivative& Cf) const = 0;
  FrameCoordinate frame_coordinate(const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  boost::shared_ptr<CartesianFixed> 
  reference_surface_intersect_approximate(const Camera& C, 
		  const FrameCoordinate& Fc, int Band = 0, 
                  double Height_reference_surface = 0.0) const;
  %python_attribute(position_ci, virtual boost::shared_ptr<CartesianInertial>)
  %python_attribute(position_cf, virtual boost::shared_ptr<CartesianFixed>)
  std::vector<boost::shared_ptr<GroundCoordinate> >
  footprint(const Camera& C, const Dem& D, 
	    double Resolution = 30,
	    int Band = 0, double Max_height = 9000) const;
  boost::shared_ptr<CartesianFixed>
  surface_intersect(const Camera& C, 
		    const FrameCoordinate& Fc,
		    const Dem& D,
		    double Resolution = 30,
		    int Band = 0, double Max_height = 9000) const;
  %extend {
    blitz::Array<double, 1> _velocity_ci() const {
      blitz::Array<double, 1> res(3);
      boost::array<double, 3> v = $self->velocity_ci();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return res;
    }
  }
  %extend {
    GeoCal::ArrayAd<double, 1> _velocity_ci_with_derivative() const {
      blitz::Array<GeoCal::AutoDerivative<double>, 1> res(3);
      boost::array<GeoCal::AutoDerivative<double>, 3> v = $self->velocity_ci_with_derivative();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return GeoCal::ArrayAd<double, 1>(res);
    }
    GeoCal::ArrayAd<double, 1> _position_ci_with_derivative() const {
      blitz::Array<GeoCal::AutoDerivative<double>, 1> res(3);
      boost::array<GeoCal::AutoDerivative<double>, 3> v = $self->position_ci_with_derivative();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return GeoCal::ArrayAd<double, 1>(res);
    }
    GeoCal::ArrayAd<double, 1> _position_cf_with_derivative() const {
      blitz::Array<GeoCal::AutoDerivative<double>, 1> res(3);
      boost::array<GeoCal::AutoDerivative<double>, 3> v = $self->position_cf_with_derivative();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return GeoCal::ArrayAd<double, 1>(res);
    }
  }
  %pythoncode {
@property
def velocity_ci(self):
    return self._velocity_ci()

@property
def velocity_ci_with_derivative(self):
    return self._velocity_ci_with_derivative()

@property
def position_ci_with_derivative(self):
    return self._position_ci_with_derivative()

@property
def position_cf_with_derivative(self):
    return self._position_cf_with_derivative()
  }
  %python_attribute(time, virtual Time)
  %python_attribute(time_with_derivative, virtual TimeWithDerivative)
  std::string print_to_string() const;
};

class QuaternionOrbitData : public OrbitData {
public:
  QuaternionOrbitData(const QuaternionOrbitData& Start,
		      const boost::array<AutoDerivative<double>, 3>& Pos_off,
		      const boost::math::quaternion<AutoDerivative<double> >&
		      Sc_to_sc_corr);
  QuaternionOrbitData(Time Tm, const boost::shared_ptr<CartesianFixed>& pos_cf,
		      const boost::array<double, 3>& vel_fixed,
		      const boost::math::quaternion<double>& sc_to_cf_q);
  QuaternionOrbitData(const TimeWithDerivative& Tm, 
		      const boost::shared_ptr<CartesianFixed>& pos_cf,
		      const boost::array<AutoDerivative<double>, 3>&
		      pos_cf_with_der,
		      const boost::array<AutoDerivative<double>, 3>& vel_fixed,
		      const boost::math::quaternion<AutoDerivative<double> >& 
		      sc_to_cf_q);
  QuaternionOrbitData(Time Tm, 
		      const boost::shared_ptr<CartesianInertial>& pos_ci,
		      const boost::array<double, 3>& vel_inertial,
		      const boost::math::quaternion<double>& sc_to_ci_q);
  QuaternionOrbitData(const TimeWithDerivative& Tm, 
		      const boost::shared_ptr<CartesianInertial>& pos_ci,
		      const boost::array<AutoDerivative<double>, 3>&
		      pos_ci_with_der,
		      const boost::array<AutoDerivative<double>, 3>& 
		      vel_inertial,
		      const boost::math::quaternion<AutoDerivative<double> >& 
		      sc_to_ci_q);
  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const;
  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const ScLookVectorWithDerivative& Sl) const;
  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const;
  virtual CartesianFixedLookVectorWithDerivative 
  cf_look_vector(const ScLookVectorWithDerivative& Sl) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const;
  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianInertialLookVectorWithDerivative& Ci) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector(const CartesianFixedLookVectorWithDerivative& Cf) const;
  %python_attribute(sc_to_ci, boost::math::quaternion<double>)
  %python_attribute(sc_to_ci_with_derivative, 
		    boost::math::quaternion<AutoDerivative<double> >)
  %python_attribute_with_set(sc_to_cf, boost::math::quaternion<double>)
  %python_attribute_with_set(sc_to_cf_with_derivative, boost::math::quaternion<AutoDerivative<double> >)
  %python_attribute(from_cf, bool)
  %extend {
    blitz::Array<double, 1> _velocity_cf() const {
      blitz::Array<double, 1> res(3);
      boost::array<double, 3> v = $self->velocity_cf();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return res;
    }
    GeoCal::ArrayAd<double, 1> _velocity_cf_with_derivative() const {
      blitz::Array<GeoCal::AutoDerivative<double>, 1> res(3);
      boost::array<GeoCal::AutoDerivative<double>, 3> v = $self->velocity_cf_with_derivative();
      for(int i = 0; i < 3; ++i)
	res(i) = v[i];
      return GeoCal::ArrayAd<double, 1>(res);
    }
  }
  %pythoncode {
@property
def velocity_cf(self):
    return self._velocity_cf()

@property
def velocity_cf_with_derivative(self):
    return self._velocity_cf_with_derivative()
  }
%pythoncode {
@classmethod
def pickle_format_version(cls):
  return 1

def __reduce__(self):
  if(self.from_cf):
    return _new_from_init, (self.__class__, 1, self.time, self.position_cf, 
			    self.velocity_cf, self.sc_to_cf)
  else:
    return _new_from_init, (self.__class__, 1, self.time, self.position_ci, 
			    self.velocity_ci, self.sc_to_ci)

}
};


%template(ObservableOrbit) GeoCal::Observable<GeoCal::Orbit>;
%template(ObserverOrbit) GeoCal::Observer<GeoCal::Orbit>;

// Allow this class to be derived from in Python.
%feature("director") Orbit;

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

class Orbit : public Observable<Orbit>, public WithParameter {
public:
  Orbit(Time Min_time = Time::min_valid_time, 
	Time Max_time = Time::max_valid_time);
  virtual ~Orbit();
  virtual void add_observer(Observer<Orbit>& Obs);
  virtual void remove_observer(Observer<Orbit>& Obs);
    
  virtual CartesianInertialLookVector ci_look_vector(Time T, 
					     const ScLookVector& Sl) const;
  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const TimeWithDerivative& T, 
		 const ScLookVectorWithDerivative& Sl) const;
  virtual CartesianFixedLookVector cf_look_vector(Time T, 
					  const ScLookVector& Sl) const;
  virtual CartesianFixedLookVectorWithDerivative 
  cf_look_vector(const TimeWithDerivative& T, 
		 const ScLookVectorWithDerivative& Sl) const;
  FrameCoordinate frame_coordinate(Time T, const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(Time T, const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  boost::shared_ptr<CartesianFixed> 
  reference_surface_intersect_approximate(Time T, const Camera& C, 
			  const FrameCoordinate& Fc, int Band = 0,
                          double Height_reference_surface = 0.0) const;
  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianInertialLookVector& Ci) const;
  virtual ScLookVectorWithDerivative sc_look_vector
  (const TimeWithDerivative& T, 
   const CartesianInertialLookVectorWithDerivative& Ci) const;
  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianFixedLookVector& Cf) const;
  virtual ScLookVectorWithDerivative sc_look_vector
  (const TimeWithDerivative& T, 
   const CartesianFixedLookVectorWithDerivative& Cf) const;
  virtual boost::shared_ptr<CartesianInertial> position_ci(Time T) const;
  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const;
  %python_attribute(min_time, Time)
  %python_attribute(max_time, Time)
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const = 0;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const = 0;
  std::string print_to_string();
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
  void notify_update_do(const Orbit& Self);
  boost::math::quaternion<double> interpolate(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace) const;
  boost::math::quaternion<AutoDerivative<double> > interpolate(
              const boost::math::quaternion<AutoDerivative<double> >& Q1, 
              const boost::math::quaternion<AutoDerivative<double> >& Q2,
	      const AutoDerivative<double>& toffset, double tspace) const;
};

class KeplerOrbit : public Orbit {
public:
  KeplerOrbit(Time Min_time = Time::min_valid_time, 
	      Time Max_time = Time::max_valid_time,
	      // This is 1998-06-30T10:51:28.32Z, w/o needing to do parsing.
	      Time Epoch = Time::time_pgs(173357492.32),
	      double Semimajor_axis = 7086930, 
	      double Eccentricity = 0.001281620, double
	      Inclination = 98.199990, 
	      double Ra_ascending_node = 255.355971130,
	      double Ap_at_epoch = 69.086962170, 
	      double Mean_anomaly_at_epoch = 290.912925280);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  
  %python_attribute_with_set(epoch, Time)
  %python_attribute_with_set(semimajor_axis, double)
  %python_attribute_with_set(argument_of_perigee, double)
  %python_attribute_with_set(eccentricity, double)
  %python_attribute_with_set(mean_anomoly, double)
  %python_attribute_with_set(inclination, double)
  %python_attribute_with_set(right_ascension, double)
  %python_attribute(period, double)
  %pickle_init(1, self.min_time, self.max_time, self.epoch,
	       self.semimajor_axis, self.eccentricity,
	       self.inclination, self.right_ascension,
	       self.argument_of_perigee, self.mean_anomoly)
};
}

%template(Vector_QuaternionOrbitData) std::vector<boost::shared_ptr<GeoCal::QuaternionOrbitData> >;
%extend std::vector<boost::shared_ptr<GeoCal::QuaternionOrbitData> > {
  %pickle_vector()
};
