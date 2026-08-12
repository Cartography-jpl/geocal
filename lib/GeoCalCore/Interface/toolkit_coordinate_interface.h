#ifndef TOOLKIT_COORDINATE_INTERFACE_H
#define TOOLKIT_COORDINATE_INTERFACE_H
#include <boost/array.hpp>	// Definition of boost::array

namespace GeoCal {
class CartesianInertial;
class CartesianFixed;
class Time;

/****************************************************************//**
  Each toolkit supplies methods to convert to and from CartesianInertial.
*******************************************************************/
class ToolkitCoordinateInterface {
public:
  virtual ~ToolkitCoordinateInterface() {}

//-----------------------------------------------------------------------
/// This converts from CartesianFixed to CartesianInertial for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). We use this because it is a unique
/// coding, the underlying toolkit doesn't need to be SPICE.
//-----------------------------------------------------------------------

  virtual void to_inertial(int Body_id, const Time& T, 
     const CartesianFixed& From, CartesianInertial& To) = 0;

//-----------------------------------------------------------------------
/// This converts from CartesianFixed to CartesianInertial for the
/// given body, including velocity. We use the NAIF coding for the
/// bodies (see the SPICE documentation for details). We use this
/// because it is a unique  coding, the underlying toolkit doesn't
/// need to be SPICE. 
//-----------------------------------------------------------------------

  virtual void to_inertial(int Body_id, const Time& T, 
   const CartesianFixed& From, const boost::array<double, 3>& Vel_cf,
   CartesianInertial& To, boost::array<double, 3>& Vel_ci) = 0;

//-----------------------------------------------------------------------
/// This converts from CartesianInertial to CartesianFixed for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). We use this because it is a unique
/// coding, the underlying toolkit doesn't need to be SPICE.
//-----------------------------------------------------------------------

  virtual void to_fixed(int Body_id, const Time& T,
     const CartesianInertial& From, CartesianFixed& To) = 0;

//-----------------------------------------------------------------------
/// This converts from CartesianInertial to CartesianFixed for the
/// given body, including velocity. We use the NAIF coding for the
/// bodies (see the SPICE documentation for details). We use this
/// because it is a unique  coding, the underlying toolkit doesn't
/// need to be SPICE. 
//-----------------------------------------------------------------------

  virtual void to_fixed(int Body_id, const Time& T, 
   const CartesianInertial& From, const boost::array<double, 3>& Vel_ci,
   CartesianFixed& To, boost::array<double, 3>& Vel_cf) = 0;

//-----------------------------------------------------------------------
/// Return a matrix for converting from CartesianInertial to
/// CartesianFixed.
//-----------------------------------------------------------------------

  virtual void to_fixed(int Body_id, const Time& T, 
			double Ci_to_cf[3][3]) = 0;

//-----------------------------------------------------------------------
/// Return a matrix for converting from CartesianInertial to
/// CartesianFixed with velocity. Note unlike the 3x3 matrix this
/// matrix is *not* orthogonal, so the inverse is not the
/// transpose. Instead call to_inertial_with_vel for the inverse.
//-----------------------------------------------------------------------

  virtual void to_fixed_with_vel(int Body_id, const Time& T, 
				 double Ci_to_cf[6][6]) = 0;

//-----------------------------------------------------------------------
/// Return a matrix for converting from CartesianFixed to
/// CartesianInertial with velocity.
//-----------------------------------------------------------------------

  virtual void to_inertial_with_vel(int Body_id, const Time& T, 
				    double Cf_to_ci[6][6]) = 0;

//-----------------------------------------------------------------------
/// Return the subsolar point on the given body for the give time.
//-----------------------------------------------------------------------

  virtual void 
  sub_solar_point(int Body_id, const Time& T, CartesianFixed& P) = 0;

//-----------------------------------------------------------------------
/// Return the sub body2 point on the given body2 for the give time.
//-----------------------------------------------------------------------

  virtual void 
  sub_body_point(int Body_id, int Body2_id, const Time& T, CartesianFixed& P) = 0;
  
//-----------------------------------------------------------------------
/// Return distance from body to the sun, at the given time. Results
/// in meters.
//-----------------------------------------------------------------------

  virtual double solar_distance(int Body_id, const Time& T) = 0;

//-----------------------------------------------------------------------
/// Return distance from body to the body2, at the given time. Results
/// in meters.
//-----------------------------------------------------------------------

  virtual double body_distance(int Body_id, int Body2_id, const Time& T) = 0;
};

}
#endif
