#ifndef MARS_COORDINATE_H
#define MARS_COORDINATE_H
#include "ground_coordinate.h"

namespace GeoCal {
/****************************************************************//**
  This is a ground coordinate, expressed in fixed Mars coordinates.
*******************************************************************/
class MarsFixed : public CartesianFixed {
public:
  enum { MARS_NAIF_CODE = 499 };
  MarsFixed(const GroundCoordinate& Gc);

//-----------------------------------------------------------------------
/// Make an MarsFixed with the given position, in meters.
//-----------------------------------------------------------------------

  MarsFixed(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an MarsFixed with the given position in meters.
//-----------------------------------------------------------------------

  MarsFixed(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  MarsFixed() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MarsFixed() {}

  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;

//-----------------------------------------------------------------------
/// Create an instance of MarsFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
  create(boost::array<double, 3> P) const
  { return boost::shared_ptr<CartesianFixed>(new MarsFixed(P)); }

//-----------------------------------------------------------------------
/// Matrix to convert MarsInertial to MarsFixed. The transpose of this
/// will convert MarsFixed to MarsInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(MARS_NAIF_CODE,
							   T, Ci_to_cf); 
  }

  virtual double height_reference_surface() const;
  virtual double min_radius_reference_surface() const;
  virtual double latitude() const;
  virtual double longitude() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const;
  virtual void print(std::ostream& Os) const;
};

}
#endif
