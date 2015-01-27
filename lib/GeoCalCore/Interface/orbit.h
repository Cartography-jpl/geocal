#ifndef ORBIT_H
#define ORBIT_H
#include "printable.h"
#include "look_vector.h"
#include "ground_coordinate.h"
#include "observer.h"
#include "camera.h"
#include "dem.h"
#include "geocal_autoderivative_quaternion.h"
#include <boost/math/quaternion.hpp>
#include <blitz/array.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class is used to convert ScLookVector,
  CartesianInertialLookVector and CartesianFixedLookVector to and 
  from each other at a given time.
*******************************************************************/

class OrbitData : public Printable<OrbitData> {
public:
  virtual ~OrbitData() {}

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianInertialLookVector.
//-----------------------------------------------------------------------

  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const = 0;

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianInertialLookVector.
//-----------------------------------------------------------------------

  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const ScLookVectorWithDerivative& Sl) 
    const = 0;

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianFixedLookVector.
//-----------------------------------------------------------------------

  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const = 0;

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianFixedLookVector.
//-----------------------------------------------------------------------

  virtual CartesianFixedLookVectorWithDerivative
  cf_look_vector(const ScLookVectorWithDerivative& Sl) 
    const = 0;

  FrameCoordinate frame_coordinate(const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
  FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const;
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
  boost::shared_ptr<CartesianFixed> 
  reference_surface_intersect_approximate(const Camera& C, 
		  const FrameCoordinate& Fc, int Band = 0,
		  double Height_reference_surface = 0.0) const;

  double resolution_meter(const Camera& C, int Band = 0) const;
  double resolution_meter(const Camera& C, const FrameCoordinate& Fc, 
			  int Band = 0) const;

//-----------------------------------------------------------------------
/// Convert from CartesianInertialLookVector to ScLookVector.
//-----------------------------------------------------------------------

  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const = 0;

//-----------------------------------------------------------------------
/// Convert from CartesianInertialLookVector to ScLookVector.
//-----------------------------------------------------------------------

  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianInertialLookVectorWithDerivative& Ci) const = 0;

//-----------------------------------------------------------------------
/// Convert from CartesianFixedLookVector to ScLookVector.
//-----------------------------------------------------------------------

  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const = 0;


//-----------------------------------------------------------------------
/// Convert from CartesianFixedLookVector to ScLookVector.
//-----------------------------------------------------------------------

  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianFixedLookVectorWithDerivative& Cf) const = 0;

//-----------------------------------------------------------------------
/// Return position as a pointer.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
  position_ci() const = 0;

//-----------------------------------------------------------------------
/// Return position as a pointer.
//-----------------------------------------------------------------------

  virtual boost::array<AutoDerivative<double>, 3>
  position_ci_with_derivative() const = 0;

//-----------------------------------------------------------------------
/// Return position as a pointer.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
  position_cf() const = 0;

//-----------------------------------------------------------------------
/// Return position as a pointer.
//-----------------------------------------------------------------------

  virtual boost::array<AutoDerivative<double>, 3>
  position_cf_with_derivative() const = 0;

//-----------------------------------------------------------------------
/// Return velocity. This is in meters per second, in same
/// CartesianInertial coordinate system as position (e.g., ECI).
//-----------------------------------------------------------------------

  virtual boost::array<double, 3> velocity_ci() const = 0;

//-----------------------------------------------------------------------
/// Return velocity. This is in meters per second, in same
/// CartesianInertial coordinate system as position (e.g., ECI).
//-----------------------------------------------------------------------

  virtual boost::array<AutoDerivative<double>, 3> 
  velocity_ci_with_derivative() const = 0;

//-----------------------------------------------------------------------
/// Return Time of OrbitData.
//-----------------------------------------------------------------------
  
  virtual Time time() const = 0;

//-----------------------------------------------------------------------
/// Return TimeWithDerivative of OrbitData.
//-----------------------------------------------------------------------

  virtual TimeWithDerivative time_with_derivative() const = 0;

  virtual void print(std::ostream& Os) const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This class implements the most common way of doing OrbitData
  conversions, which just uses fixed quaternions.
								 
  This accounts for both the orientation of the spacecraft relative to the
  planet, as well as the aberration of light (a small correction due
  to the movement of the spacecraft relative to the planet).

  This does *not* account for atmospheric refraction. Depending on the
  zenith angle, this can be somewhat important for satellites. From
  the approximate atmospheric model described in "Theoretical Basis of
  the SDP Toolkit Geolocation package for the ECS", Table 6-5 the
  linear displacement for a zenith angle of 10 is 0.549 meters, 20
  degrees is 1.223 meters, and 30 degrees is 2.221.

  We may want to add a atmospheric refraction correction in the
  future, but this hasn't been done yet.

  We need to have one of the toolkit available if we want to convert
  for the CartesianFixed coordinates used by this class to 
  CartesianInertial. If you stick to working with CartesianFixed only,
  you can avoid the need of using one of these toolkits.

  Note that we allow most pieces of this to be AutoDerivative, useful
  for propagating jacobians.   We do *not* support time being a
  AutoDerivative, so supporting things like time offset isn't
  currently in here. We probably could do this, we'd just need to
  think through how to support this.
*******************************************************************/

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
  virtual ~QuaternionOrbitData() {}

  virtual CartesianInertialLookVector 
  ci_look_vector(const ScLookVector& Sl) const;
  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const ScLookVectorWithDerivative& Sl) 
    const;
  virtual CartesianFixedLookVector 
  cf_look_vector(const ScLookVector& Sl) const;
  virtual CartesianFixedLookVectorWithDerivative
  cf_look_vector(const ScLookVectorWithDerivative& Sl) 
    const;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const;
  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianInertialLookVectorWithDerivative& Ci) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const;
  virtual ScLookVectorWithDerivative
  sc_look_vector(const CartesianFixedLookVectorWithDerivative& Cf) const;

//-----------------------------------------------------------------------
/// Return position as a ptr.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> position_ci() const
  { 
    fill_in_ci_to_cf();
    return pos_ci; 
  }

//-----------------------------------------------------------------------
/// Return position as a pointer.
//-----------------------------------------------------------------------

  virtual boost::array<AutoDerivative<double>, 3>
  position_ci_with_derivative() const
  {    
    fill_in_ci_to_cf();
    boost::array<AutoDerivative<double>, 3> res = 
      {{pos_ci_with_der.R_component_2(), 
	pos_ci_with_der.R_component_3(),
	pos_ci_with_der.R_component_4()}};
    return res;
  }

//-----------------------------------------------------------------------
/// Return position as a ptr.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> position_cf() const
  { return pos; }

//-----------------------------------------------------------------------
/// Return position as a ptr.
//-----------------------------------------------------------------------

  virtual boost::array<AutoDerivative<double>, 3>
  position_cf_with_derivative() const
  { 
    boost::array<AutoDerivative<double>, 3> res = 
      {{pos_with_der.R_component_2(), 
	pos_with_der.R_component_3(),
	pos_with_der.R_component_4()}};
    return res;
  }

  virtual boost::array<double, 3> velocity_ci() const;
  virtual boost::array<AutoDerivative<double>, 3> 
  velocity_ci_with_derivative() const;
  boost::array<double, 3> velocity_cf() const;
  virtual boost::array<AutoDerivative<double>, 3> 
  velocity_cf_with_derivative() const;

//-----------------------------------------------------------------------
/// Return Time of OrbitData.
//-----------------------------------------------------------------------
  
  virtual Time time() const {return tm.value();}
  virtual TimeWithDerivative time_with_derivative() const {return tm;}
  virtual void print(std::ostream& Os) const;
  friend boost::shared_ptr<QuaternionOrbitData>
  GeoCal::interpolate(const QuaternionOrbitData& t1, 
		      const QuaternionOrbitData& t2, 
		      const TimeWithDerivative& tm);

//-----------------------------------------------------------------------
/// Return the quaternion used to go from spacecraft to cartesian inertial
//-----------------------------------------------------------------------

  boost::math::quaternion<double> sc_to_ci() const 
  { return conj(ci_to_cf()) * sc_to_cf_; }
  
//-----------------------------------------------------------------------
/// Return the quaternion used to go from spacecraft to cartesian inertial
//-----------------------------------------------------------------------

  boost::math::quaternion<AutoDerivative<double> > sc_to_ci_with_derivative() const 
  { return conj(ci_to_cf_with_derivative()) * sc_to_cf_with_der; }
  
//-----------------------------------------------------------------------
/// Return the quaternion used to go from spacecraft to cartesian fixed.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> sc_to_cf() const { return sc_to_cf_; }

//-----------------------------------------------------------------------
/// Set the quaternion used to go from spacecraft to cartesian fixed.
//-----------------------------------------------------------------------

  void sc_to_cf(const boost::math::quaternion<double>& Sc_to_cf) 
  { sc_to_cf_ = Sc_to_cf; sc_to_cf_with_der = sc_to_cf_; }

//-----------------------------------------------------------------------
/// Return the quaternion used to go from spacecraft to cartesian fixed.
//-----------------------------------------------------------------------

  boost::math::quaternion<AutoDerivative<double> > sc_to_cf_with_derivative() 
    const { return sc_to_cf_with_der; }

  void sc_to_cf_with_derivative
  (const boost::math::quaternion<AutoDerivative<double> >& Sc_to_cf ) 
  { sc_to_cf_ = value(Sc_to_cf); sc_to_cf_with_der = Sc_to_cf; }

//-----------------------------------------------------------------------
/// Was this created form the cartesian fixed version of the
/// constructor? This is intended for use by python when we pickle
/// this object so we know which constructor to call. It probably
/// isn't of interest to anything else.
//-----------------------------------------------------------------------
  bool from_cf() const { return from_cf_; }
protected:
//-----------------------------------------------------------------------
/// Default constructor. Derived classes should call initialize before
/// finishing their constructor.
//-----------------------------------------------------------------------

  QuaternionOrbitData() {}

  void initialize(Time Tm, const boost::shared_ptr<CartesianFixed>& pos_cf,
    const boost::array<double, 3>& vel_fixed, const 
    boost::math::quaternion<double>& sc_to_cf_q);
  void initialize(const TimeWithDerivative& Tm, 
		  const boost::shared_ptr<CartesianFixed>& pos_cf,
    const boost::array<AutoDerivative<double>, 3>& pos_cf_with_der,
    const boost::array<AutoDerivative<double>, 3>& vel_fixed, const 
    boost::math::quaternion<AutoDerivative<double> >& sc_to_cf_q);
  void initialize(Time Tm, const boost::shared_ptr<CartesianInertial>& pos_ci,
    const boost::array<double, 3>& vel_inertial, const 
    boost::math::quaternion<double>& sc_to_ci_q);
  void initialize(const TimeWithDerivative& Tm, 
		  const boost::shared_ptr<CartesianInertial>& pos_ci,
    const boost::array<AutoDerivative<double>, 3>& pos_ci_with_der,
    const boost::array<AutoDerivative<double>, 3>& vel_inertial, const 
    boost::math::quaternion<AutoDerivative<double> >& sc_to_ci_q);


private:
  TimeWithDerivative tm;	///< Time of OrbitData.
  boost::shared_ptr<CartesianFixed> pos;
				///< Position
  boost::math::quaternion<AutoDerivative<double> > pos_with_der;
  boost::math::quaternion<double> vel_cf; ///< Velocity, in m/s
  boost::math::quaternion<AutoDerivative<double> > 
  vel_cf_with_der;			
  boost::math::quaternion<double> sc_to_cf_;
				///< Quaternion to go from
				///ScLookVector to
				///CartesianFixed.
  boost::math::quaternion<AutoDerivative<double> > sc_to_cf_with_der;

  bool from_cf_;

//-----------------------------------------------------------------------
/// We create ci_to_cf on demand. This means if we don't do any
/// conversion to CartesianInertial coordinates we don't generate
/// this. In particular this means we don't require the use of one of
/// the toolkits (such as SPICE or SDP).
//-----------------------------------------------------------------------

  boost::math::quaternion<double>& ci_to_cf() const
  {
    fill_in_ci_to_cf();
    return ci_to_cf_;
  }

  boost::math::quaternion<AutoDerivative<double> >& 
  ci_to_cf_with_derivative() const
  {
    fill_in_ci_to_cf();
    return ci_to_cf_der_;
  }
  void fill_in_ci_to_cf() const;
  mutable bool have_ci_to_cf;
  mutable boost::math::quaternion<double> ci_to_cf_;
  mutable boost::math::quaternion<AutoDerivative<double> > ci_to_cf_der_;
  mutable boost::shared_ptr<CartesianInertial> pos_ci;
				///< Position
  mutable boost::math::quaternion<AutoDerivative<double> > pos_ci_with_der;
  mutable boost::math::quaternion<double> vel_ci; 
  mutable boost::math::quaternion<AutoDerivative<double> > 
  vel_ci_with_der; 

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
   This class is used to model orbit data, allowing conversions from
   spacecraft coordinates to CartesianInertial and CartesianFixed 
   coordinates.

   This class is used to return orbit data at a given time. OrbitData
   is a class that is able to convert from spacecraft coordinates to
   CartesianInertial coordinates and vice-versa at a given time, as
   well as giving the platforms position.

   As an optimization, methods for direct conversion from 
   spacecraft coordinates to CartesianInertial and vice-versa, which
   don't use the intermediate OrbitData classes, are supplied.
   The default methods just use the OrbitData methods, but derived
   classes can supply more optimized versions of these methods.

   An orbit has a min_time() and a max_time() that orbit data is
   available for. Requesting data outside of this range will cause an
   exception to be thrown. For Time T, we must have 
   min_time() <= T < max_time().
*******************************************************************/

class Orbit : public Printable<Orbit>, 
	      public Observable<Orbit>, 
	      public WithParameter {
public:
//-----------------------------------------------------------------------
/// Constructor. The Orbit is valid for the given range of minimum to
/// maximum time.
//-----------------------------------------------------------------------

  Orbit(Time Min_time = Time::min_valid_time, 
	Time Max_time = Time::max_valid_time)
    : min_tm(Min_time), max_tm(Max_time) {}

  virtual void add_observer(Observer<Orbit>& Obs) 
  { add_observer_do(Obs, *this);}
  virtual void remove_observer(Observer<Orbit>& Obs) 
  { remove_observer_do(Obs, *this);}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Orbit() {}

//-----------------------------------------------------------------------
/// Give the frame coordinates that a particular point on the ground
/// is seen.
//-----------------------------------------------------------------------

  FrameCoordinate frame_coordinate(Time T, const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const
  { return orbit_data(T)->frame_coordinate(Gc, C, Band);}

//-----------------------------------------------------------------------
/// Give the frame coordinates that a particular point on the ground
/// is seen.
//-----------------------------------------------------------------------

  FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(Time T, const GroundCoordinate& Gc, 
				   const Camera& C, int Band = 0) const
  { return orbit_data(T)->frame_coordinate_with_derivative(Gc, C, Band);}

//-----------------------------------------------------------------------
/// Return location on the reference surface that a particular frame 
/// coordinate is seen. This is approximate, in the same way 
/// CartesianFixed::reference_intersect_approximate is approximate.
//-----------------------------------------------------------------------

  boost::shared_ptr<CartesianFixed> 
  reference_surface_intersect_approximate(Time T, const Camera& C, 
	  const FrameCoordinate& Fc, int Band = 0, 
          double Height_reference_surface = 0.0) const
  { return orbit_data(T)->reference_surface_intersect_approximate(C, Fc, 
				  Band, Height_reference_surface); }

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianInertialLookVector for the
/// given time. We should have min_time() <= T < max_time(). 
//-----------------------------------------------------------------------

  virtual CartesianInertialLookVector ci_look_vector(Time T, 
			     const ScLookVector& Sl) const
  { return orbit_data(T)->ci_look_vector(Sl); }

  virtual CartesianInertialLookVectorWithDerivative 
  ci_look_vector(const TimeWithDerivative& T, 
		 const ScLookVectorWithDerivative& Sl) const
  { return orbit_data(T)->ci_look_vector(Sl); }

//-----------------------------------------------------------------------
/// Convert from ScLookVector to CartesianFixedLookVector for the
/// given time. We should have min_time() <= T < max_time(). 
//-----------------------------------------------------------------------

  virtual CartesianFixedLookVector cf_look_vector(Time T, 
					  const ScLookVector& Sl) const
  { return orbit_data(T)->cf_look_vector(Sl); }

  virtual CartesianFixedLookVectorWithDerivative 
  cf_look_vector(const TimeWithDerivative& T, 
		 const ScLookVectorWithDerivative& Sl) const
  { return orbit_data(T)->cf_look_vector(Sl); }

//-----------------------------------------------------------------------
/// Convert from CartesianInertialLookVector to ScLookVector for the
/// given time. We should have min_time() <= T < max_time(). 
//-----------------------------------------------------------------------

  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianInertialLookVector& Ci) const
  { return orbit_data(T)->sc_look_vector(Ci); }
  virtual ScLookVectorWithDerivative 
  sc_look_vector(const TimeWithDerivative& T, 
		 const CartesianInertialLookVectorWithDerivative& Ci) const
  { return orbit_data(T)->sc_look_vector(Ci); }

//-----------------------------------------------------------------------
/// Convert from CartesianFixedLookVector to ScLookVector for the
/// given time. We should have min_time() <= T < max_time(). 
//-----------------------------------------------------------------------

  virtual ScLookVector sc_look_vector(Time T, 
			      const CartesianFixedLookVector& Cf) const
  { return orbit_data(T)->sc_look_vector(Cf); }

  virtual ScLookVectorWithDerivative sc_look_vector
  (const TimeWithDerivative& T, 
   const CartesianFixedLookVectorWithDerivative& Cf) const
  { return orbit_data(T)->sc_look_vector(Cf); }
//-----------------------------------------------------------------------
/// Return position at given time. We should have min_time() <= T <
/// max_time(). 
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> position_ci(Time T) const
  { return orbit_data(T)->position_ci(); }

  virtual boost::array<AutoDerivative<double>, 3> 
  position_ci_with_derivative(const TimeWithDerivative& T) const
  { return orbit_data(T)->position_ci_with_derivative(); }

//-----------------------------------------------------------------------
/// Return position at given time. We should have min_time() <= T <
/// max_time(). 
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const
  { return orbit_data(T)->position_cf(); }

  virtual boost::array<AutoDerivative<double>, 3> 
  position_cf_with_derivative(const TimeWithDerivative& T) const
  { return orbit_data(T)->position_cf_with_derivative(); }

//-----------------------------------------------------------------------
/// Return velocity at given time. This is in m/s, in same coordinate
/// system as position.
//-----------------------------------------------------------------------

  virtual boost::array<double, 3> velocity_ci(Time T) const
  { return orbit_data(T)->velocity_ci(); }

//-----------------------------------------------------------------------
/// Minimum time that we can return OrbitData for.
//-----------------------------------------------------------------------

  Time min_time() const {return min_tm;}

//-----------------------------------------------------------------------
/// Maximum time that we can return OrbitData for.
//-----------------------------------------------------------------------

  Time max_time() const {return max_tm;}

//-----------------------------------------------------------------------
/// Return OrbitData for the given time. We should have min_time() <=
/// T < max_time().
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const = 0;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const  = 0;
  virtual void print(std::ostream& Os) const { Os << "Orbit"; }
protected:
//-----------------------------------------------------------------------
/// This calculates the weighting factor to use to do a Lagrangian 
/// interpolation.
//-----------------------------------------------------------------------

  template<class iterator> std::vector<double> 
  lagrangian_interpolation_factor(iterator tstart, 
			    iterator tend, Time t) const
  {
    std::vector<double> res;
    for(iterator j = tstart; j != tend; ++j) {
      double tf = 1.0, bf = 1.0;
      for(iterator i = tstart; i != tend; ++i)
	if(i != j) {
	  tf *= (t - *i);
	  bf *= (*j - *i);
	}
      res.push_back(tf / bf);
    }
    return res;
  }

//-----------------------------------------------------------------------
/// This calculates a Lagrangian interpolation of the given set of
/// blitz::Array<double, 1> with the given time intervals.
//-----------------------------------------------------------------------

  template<class iterator, class iterator2> 
  blitz::Array<double, 1> lagrangian_interpolation(iterator tstart, 
						   iterator tend,
						   Time tm,
						   iterator2 vstart,
						   iterator2 vend) const
  {
    std::vector<double> fac = lagrangian_interpolation_factor(tstart, tend, tm);
    std::vector<double>::iterator ifac = fac.begin();
    blitz::Array<double, 1> res((*vstart).shape());
    res = 0.0;
    for(iterator2 v = vstart; v != vend && ifac != fac.end(); ++v, ++ifac)
      res += (*ifac) * (*v);
    return res;
  }
  void interpolate(const boost::array<double, 3>& P1,
		   const boost::array<double, 3>& V1,
		   const boost::array<double, 3>& P2,
		   const boost::array<double, 3>& V2,
		   double toffset, double tspace,
		   boost::array<double, 3>& Pres,
		   boost::array<double, 3>& Vres) const;
  void interpolate(const boost::array<AutoDerivative<double>, 3>& P1,
		   const boost::array<AutoDerivative<double>, 3>& V1,
		   const boost::array<AutoDerivative<double>, 3>& P2,
		   const boost::array<AutoDerivative<double>, 3>& V2,
		   const AutoDerivative<double>& toffset, 
		   double tspace,
		   boost::array<AutoDerivative<double>, 3>& Pres,
		   boost::array<AutoDerivative<double>, 3>& Vres) const;
  boost::math::quaternion<double> interpolate(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace) const;
  boost::math::quaternion<AutoDerivative<double> > interpolate(
              const boost::math::quaternion<AutoDerivative<double> >& Q1, 
              const boost::math::quaternion<AutoDerivative<double> >& Q2,
	      const AutoDerivative<double>& toffset, double tspace) const;
  Time min_tm;			///< Minimum time that we have
				///OrbitData for.
  Time max_tm;			///< Maximum time that we have
				///OrbitData for.
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};


/****************************************************************//**
  This is a simple implementation of an Orbit. It just uses Kepler's
  equations. This is intended primarily for easy testing of other
  classes that need an Orbit, rather than being a realistic orbit
  simulation for real use.

  The default parameters are for a nominal MISR orbit.
*******************************************************************/

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
  virtual ~KeplerOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Epoch that rest of data is for.
//-----------------------------------------------------------------------

  const Time& epoch() const {return epoch_;}

//-----------------------------------------------------------------------
/// Set epoch that rest of data is for.
//-----------------------------------------------------------------------

  void epoch(const Time& Epoch) { epoch_ = Epoch; }

//-----------------------------------------------------------------------
/// Semimajor axis in meters.
//-----------------------------------------------------------------------
  
  double semimajor_axis() const {return a_;}

//-----------------------------------------------------------------------
/// Set semimajor axis in meters
//-----------------------------------------------------------------------

  void semimajor_axis(double Semimajor_axis) 
  { 
    a_ = Semimajor_axis;
    calc_freq_rev();
  }

//-----------------------------------------------------------------------
/// Argument of perigee at epoch, in degrees.
//-----------------------------------------------------------------------

  double argument_of_perigee() const 
  { return ap_ * Constant::rad_to_deg; }

//-----------------------------------------------------------------------
/// Set Argument of perigee at epoch, in degrees.
//-----------------------------------------------------------------------

  void argument_of_perigee(double Ap_at_epoch)
  { ap_ = Ap_at_epoch * Constant::deg_to_rad; }

//-----------------------------------------------------------------------
/// Eccentricity of orbit.
//-----------------------------------------------------------------------

  double eccentricity() const {return e_; }

//-----------------------------------------------------------------------
/// Set eccentricity of orbit.
//-----------------------------------------------------------------------

  void eccentricity(double Eccentricity) {e_ = Eccentricity; }

//-----------------------------------------------------------------------
/// Mean anomoly at epoch, in degrees.
//-----------------------------------------------------------------------
  
  double mean_anomoly() const {return ma_ * Constant::rad_to_deg; }

//-----------------------------------------------------------------------
/// Set mean anomoly at epoch, in degrees.
//-----------------------------------------------------------------------
  
  void mean_anomoly(double Mean_anomaly_at_epoch) 
  { ma_ = Mean_anomaly_at_epoch * Constant::deg_to_rad; }

//-----------------------------------------------------------------------
/// Inclination of orbit, in degrees.
//-----------------------------------------------------------------------

  double inclination() const {return inc_ * Constant::rad_to_deg; }

//-----------------------------------------------------------------------
/// Set inclination of orbit, in degrees.
//-----------------------------------------------------------------------

  void inclination(double Inclination) 
  { 
    inc_  = Inclination * Constant::deg_to_rad; 
    calc_r();
  }

//-----------------------------------------------------------------------
/// Right ascension of ascending node, in degrees.
//-----------------------------------------------------------------------

  double right_ascension() const 
  { return ra_ * Constant::rad_to_deg;}

//-----------------------------------------------------------------------
/// Set right ascension of ascending node, in degrees.
//-----------------------------------------------------------------------

  void right_ascension(double Ra_ascending_node) 
  { 
    ra_  = Ra_ascending_node * Constant::deg_to_rad;
    calc_r();
  }

//-----------------------------------------------------------------------
/// Period in seconds
//-----------------------------------------------------------------------

  double period() const 
  { return 2 * Constant::pi / freq_rev_; }
private:
  Time epoch_;			///< Epoch that rest of data is for.
  double a_;			///< Semimajor axis in meters.
  double ap_;			///< Argument of perigee at epoch, in radians.
  double e_;			///< Eccentricity of orbit.
  double freq_rev_;		///< 2 * pi / period of orbit. Called
				/// omega in Goldstein. 
  double ma_;			///< Mean anomoly at epoch (in
				///radians).
  double inc_;			///< Inclination of orbit, in radians.
  double ra_;			///< Right ascension of ascending
				///node, in radians.
  blitz::Array<double, 2> r_;	///< Rotation matrix that goes from
				/// orbit in xy plane to one with
				/// correct inclination and Right
				/// ascending node. 
  void calc_freq_rev();
  void calc_r();
};

  boost::shared_ptr<QuaternionOrbitData>
  interpolate(const QuaternionOrbitData& t1, 
	      const QuaternionOrbitData& t2, 
	      const TimeWithDerivative& tm);
}

GEOCAL_EXPORT_KEY(QuaternionOrbitData);
GEOCAL_EXPORT_KEY(OrbitData);
GEOCAL_EXPORT_KEY(Orbit);
#endif

