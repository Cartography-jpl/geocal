#ifndef ECR_H
#define ECR_H
#include "ground_coordinate.h"

namespace GeoCal {
class Geodetic;			// Forward declaration

/****************************************************************//**
  This is a ground coordinate, represented in ECR coordinates.
*******************************************************************/

class Ecr : public CartesianFixed {
public:
  enum {EARTH_NAIF_CODE = 399};

  Ecr(const GroundCoordinate& Gc);

//-----------------------------------------------------------------------
/// Make an Ecr with the given position, in meters.
//-----------------------------------------------------------------------

  Ecr(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an Ecr with the given position in meters.
//-----------------------------------------------------------------------

  Ecr(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Ecr() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Ecr() {}

  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;

//-----------------------------------------------------------------------
/// Create an instance of Ecr.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
  create(boost::array<double, 3> P) const
  { return boost::shared_ptr<CartesianFixed>(new Ecr(P)); }

//-----------------------------------------------------------------------
/// Matrix to convert Eci to Ecr. The transpose of this will convert
/// Ecr to Eci.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(EARTH_NAIF_CODE,
							   T, Ci_to_cf); 
  }

  virtual double height_reference_surface() const;
  Geodetic convert_to_geodetic() const;
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