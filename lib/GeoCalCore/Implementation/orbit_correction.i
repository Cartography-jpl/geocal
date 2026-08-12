// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_correction.h"
%}
%base_import(orbit)

%geocal_shared_ptr(GeoCal::OrbitCorrection);
namespace GeoCal {
%nodefaultctor OrbitCorrection;
// Allow this class to be derived from in Python.
%feature("director") OrbitCorrection;
  
class OrbitCorrection: public Orbit {
public:
  virtual std::string desc() const;
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
	   const Camera& C, int Band = 0,
	   const boost::shared_ptr<Refraction>&
	   Ref = boost::shared_ptr<Refraction>(),
           const boost::shared_ptr<VelocityAberration>&
	   Vabb = boost::shared_ptr<VelocityAberration>()) const;
  FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const TimeWithDerivative& T, 
           const GroundCoordinate& Gc, 
	   const Camera& C, int Band = 0,
	   const boost::shared_ptr<Refraction>&
	   Ref = boost::shared_ptr<Refraction>(),
           const boost::shared_ptr<VelocityAberration>&
	   Vabb = boost::shared_ptr<VelocityAberration>()) const;
				   
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
  virtual ScLookVector sc_look_vector(Time T, 
           const CartesianFixed& Pt,
	   const boost::shared_ptr<Refraction>&
	   Ref = boost::shared_ptr<Refraction>(),
           const boost::shared_ptr<VelocityAberration>&
	   Vabb = boost::shared_ptr<VelocityAberration>()) const;
  virtual boost::shared_ptr<CartesianInertial> position_ci(Time T) const;
  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const;
  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);
  %python_attribute(orbit_uncorrected, boost::shared_ptr<Orbit>);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization();
  OrbitCorrection(const boost::shared_ptr<Orbit> Orb_uncorr);
  OrbitCorrection();
  virtual boost::array<AutoDerivative<double>, 3 > 
  pcorr_with_derivative(const TimeWithDerivative& Tm, 
			const CartesianFixed& Pos_uncorr) const = 0;
  virtual boost::array<double, 3 > 
  pcorr(const Time& Tm,
	const CartesianFixed& Pos_uncorr) const = 0;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  acorr_with_derivative(const TimeWithDerivative& T) const = 0;
  virtual boost::math::quaternion<double> 
  acorr(const Time& T) const = 0;
  %python_attribute(pos_corr_is_cf, bool);
};
}

// Extra code for handling boost serialization/python pickle of
// director classes
%geocal_director_serialization(orbit_correction, OrbitCorrection)

// List of things "import *" will include
%python_export("OrbitCorrection")
