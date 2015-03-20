#ifndef ECI_TOD_BURL_H
#define ECI_TOD_BURL_H
#include "eci.h"

namespace GeoCal {

/****************************************************************//**
  This is a ECI true of date coordinate. This is a dynamic frame that
  uses the true equator and equinox of a particular data.

  This implementation uses Mike Burl's code. I believe EciTod is 
  more accurate since it accounts for polar drift, but I'm not
  positive. We want to be able to compare between the two, so I'll
  put this in place.
*******************************************************************/

class EciTodBurl : public CartesianInertial {
public:
//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  EciTodBurl() {}

//-----------------------------------------------------------------------
/// Make an EciTod with the given position, in meters.
//-----------------------------------------------------------------------

  EciTodBurl(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an Eci with the given position in meters.
//-----------------------------------------------------------------------

  EciTodBurl(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~EciTodBurl() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const;

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>(new EciTodBurl(P)); }

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// delta_ut1 to use in time conversion.
//-----------------------------------------------------------------------

  static double delta_ut1;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(EciTodBurl);
#endif
