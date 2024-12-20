#ifndef ECI_H
#define ECI_H
#include "ground_coordinate.h"
#include "ecr.h"

namespace GeoCal {

/****************************************************************//**
  This is a ECI coordinate (J2000)
*******************************************************************/

class Eci : public CartesianInertial {
public:
  enum {EARTH_NAIF_CODE = 399};

//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  Eci() {}

//-----------------------------------------------------------------------
/// Convert from Ecr to Eci
//-----------------------------------------------------------------------

  Eci(const Ecr& P, const Time& T)
  {
    CartesianFixed::toolkit_coordinate_interface->
      to_inertial((int) EARTH_NAIF_CODE, T, P, *this);
  }

//-----------------------------------------------------------------------
/// Convert from GroundCoordinate to Eci
//-----------------------------------------------------------------------

  Eci(const GroundCoordinate& Gc, const Time& T)
  {
    Ecr p(Gc);
    CartesianFixed::toolkit_coordinate_interface->
      to_inertial((int) EARTH_NAIF_CODE, T, p, *this);
  }
  
//-----------------------------------------------------------------------
/// Make an Eci with the given position, in meters.
//-----------------------------------------------------------------------

  Eci(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an Eci with the given position in meters.
//-----------------------------------------------------------------------

  Eci(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Eci() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;

//-----------------------------------------------------------------------
/// Matrix to convert Eci to Ecr. The transpose of this will convert
/// Ecr to Eci.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(EARTH_NAIF_CODE,
							   T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Matrix to convert Eci to Ecr with velocity. The transpose of this
/// will convert Ecr to Eci.
//-----------------------------------------------------------------------

  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed_with_vel
      (EARTH_NAIF_CODE, T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>(new Eci(P)); }

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const;

  virtual int naif_code() const { return EARTH_NAIF_CODE; }
  
//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Eci);
#endif
