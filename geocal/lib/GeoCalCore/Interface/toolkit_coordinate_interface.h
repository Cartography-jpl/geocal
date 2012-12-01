#ifndef TOOLKIT_COORDINATE_INTERFACE_H
#define TOOLKIT_COORDINATE_INTERFACE_H

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
/// This converts from CartesianInertial to CartesianFixed for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). We use this because it is a unique
/// coding, the underlying toolkit doesn't need to be SPICE.
//-----------------------------------------------------------------------

  virtual void to_fixed(int Body_id, const Time& T,
     const CartesianInertial& From, CartesianFixed& To) = 0;

//-----------------------------------------------------------------------
/// Return a matrix for converting from CartesianInertial to
/// CartesianFixed.
//-----------------------------------------------------------------------

  virtual void to_fixed(int Body_id, const Time& T, 
			double Ci_to_cf[3][3]) = 0;


};

}
#endif
