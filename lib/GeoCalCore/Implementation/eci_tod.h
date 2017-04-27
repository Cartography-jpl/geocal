#ifndef ECI_TOD_H
#define ECI_TOD_H
#include "eci.h"

namespace GeoCal {

/****************************************************************//**
  This is a ECI true of date coordinate. This is a dynamic frame that
  uses the true equator and equinox of a particular data.
*******************************************************************/

class EciTod : public CartesianInertial {
public:
  enum {EARTH_NAIF_CODE = 399};

//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  EciTod() {}
  EciTod(const Eci& Eci_coor, const Time& T);

//-----------------------------------------------------------------------
/// Make an EciTod with the given position, in meters.
//-----------------------------------------------------------------------

  EciTod(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an Eci with the given position in meters.
//-----------------------------------------------------------------------

  EciTod(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~EciTod() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const;

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>(new EciTod(P)); }
  Eci to_eci(const Time& T);

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

GEOCAL_EXPORT_KEY(EciTod);
#endif
