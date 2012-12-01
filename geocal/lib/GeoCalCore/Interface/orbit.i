// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "orbit.h"
#include "argus_orbit.h"
#include "hdf_orbit.h"
#include "quickbird_orbit.h"
#include "spot_orbit.h"
#include "eci_tod.h"
namespace GeoCal {
  typedef HdfOrbit<EciTod, TimeAcsCreator> HdfOrbit_EciTod_TimeAcs;
}
%}

%geocal_shared_ptr(OrbitData);
%geocal_shared_ptr(QuaternionOrbitData);
%geocal_shared_ptr(Orbit);
%geocal_shared_ptr(KeplerOrbit);

%shared_ptr_dynamic_list(GeoCal::Orbit,
			 GeoCal::KeplerOrbit,
			 GeoCal::QuickBirdOrbit,
			 GeoCal::SpotOrbit,
			 GeoCal::ArgusOrbit,
			 GeoCal::PosExportOrbit,
			 GeoCal::HdfOrbit_EciTod_TimeAcs
			 );

%shared_ptr_dynamic_list(GeoCal::OrbitData,
			 GeoCal::ArgusOrbitData,
			 GeoCal::AircraftOrbitData,
			 GeoCal::QuaternionOrbitData);
namespace GeoCal {
class OrbitData {
public:
  double resolution_meter(const Camera& C, int Band = 0) const;
  double OrbitData::resolution_meter(const Camera& C, const FrameCoordinate& Fc,
				     int Band) const;
  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const = 0;
  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const = 0;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const = 0;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const = 0;
  FrameCoordinate frame_coordinate(const GroundCoordinate& Gc, 
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
  %python_attribute(velocity_ci, virtual boost::array<double, 3>)
  %python_attribute(time, virtual Time)
  std::string print_to_string() const;
};

class QuaternionOrbitData : public OrbitData {
public:
  QuaternionOrbitData(Time Tm, const boost::shared_ptr<CartesianFixed>& pos_cf,
		      const boost::array<double, 3>& vel_fixed,
		      const boost::math::quaternion<double>& sc_to_cf_q);
  QuaternionOrbitData(Time Tm, 
		      const boost::shared_ptr<CartesianInertial>& pos_ci,
		      const boost::array<double, 3>& vel_inertial,
		      const boost::math::quaternion<double>& sc_to_ci_q);
  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const;
  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const;
  %python_attribute(sc_to_ci, boost::math::quaternion<double>)
  %python_attribute(sc_to_cf, boost::math::quaternion<double>)
  %python_attribute(from_cf, bool)
  %python_attribute(velocity_cf, virtual boost::array<double, 3>)
%pythoncode {
def __reduce__(self):
  if(self.from_cf):
    return _new_from_init, (self.__class__, self.time, self.position_cf, 
			    self.velocity_cf, self.sc_to_cf)
  else:
    return _new_from_init, (self.__class__, self.time, self.position_ci, 
			    self.velocity_ci, self.sc_to_ci)

}
};

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

class Orbit {
public:
  Orbit(Time Min_time = Time::min_valid_time, 
	Time Max_time = Time::max_valid_time);
  virtual ~Orbit();
  virtual CartesianInertialLookVector ci_look_vector(Time T, 
					     const ScLookVector& Sl) const;
  virtual CartesianFixedLookVector cf_look_vector(Time T, 
					  const ScLookVector& Sl) const;
  FrameCoordinate frame_coordinate(Time T, const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  boost::shared_ptr<CartesianFixed> 
  reference_surface_intersect_approximate(Time T, const Camera& C, 
			  const FrameCoordinate& Fc, int Band = 0,
                          double Height_reference_surface = 0.0) const;
  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianInertialLookVector& Ci) const;
  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianFixedLookVector& Cf) const;
  virtual boost::shared_ptr<CartesianInertial> position_ci(Time T) const;
  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const;
  virtual boost::array<double, 3> velocity_ci(Time T) const;
  %python_attribute(min_time, Time)
  %python_attribute(max_time, Time)
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const = 0;
  std::string print_to_string();
protected:
  boost::math::quaternion<double> interpolate(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace) const;
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
  
  %python_attribute(epoch, Time)
  %python_attribute(semimajor_axis, double)
  %python_attribute(argument_of_perigee, double)
  %python_attribute(eccentricity, double)
  %python_attribute(mean_anomoly, double)
  %python_attribute(inclination, double)
  %python_attribute(right_ascension, double)
  %pickle_init(self.min_time, self.max_time, self.epoch,
	       self.semimajor_axis, self.eccentricity,
	       self.inclination, self.right_ascension,
	       self.argument_of_perigee, self.mean_anomoly)
};
}

%template(Vector_QuaternionOrbitData) std::vector<boost::shared_ptr<GeoCal::QuaternionOrbitData> >;
