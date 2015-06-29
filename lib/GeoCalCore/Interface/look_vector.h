#ifndef LOOK_VECTOR_H
#define LOOK_VECTOR_H
#include "printable.h"
#include "auto_derivative.h"
#include "auto_derivative_quaternion.h"
#include "constant.h"
#include <boost/array.hpp>
#include <boost/math/quaternion.hpp>
namespace GeoCal {
  class GroundCoordinate;	// Forward declaration.
  class Time;

/****************************************************************//**
  This is a look vector in an unspecified coordinate system. Derived
  classes specify the coordinate system.
*******************************************************************/

template<class T> class LookVector : public Printable<LookVector<T> > {
public:
  virtual ~LookVector() {}

//-----------------------------------------------------------------------
/// Look vector. This should be in meters. This gives both a direction
/// and length.
//-----------------------------------------------------------------------

  boost::array<T, 3> look_vector;

//-----------------------------------------------------------------------
/// Look vector as a quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<T> look_quaternion() const
  { return boost::math::quaternion<T>(0, look_vector[0], look_vector[1],
					   look_vector[2]); }

//-----------------------------------------------------------------------
/// Set look vector using a quaternion.
//-----------------------------------------------------------------------
  
  void look_quaternion(const boost::math::quaternion<T>& V)
  { look_vector[0] = V.R_component_2(); look_vector[1] = V.R_component_3();
    look_vector[2] = V.R_component_4(); }
  
//-----------------------------------------------------------------------
/// This is the direction, as a unit vector.
//-----------------------------------------------------------------------

  boost::array<T, 3> direction() const
  {
    boost::array<T, 3> res;
    T l = length();
    res[0] = look_vector[0] / l;
    res[1] = look_vector[1] / l;
    res[2] = look_vector[2] / l;
    return res;
  }

//-----------------------------------------------------------------------
/// Length of look vector, in meters.
//-----------------------------------------------------------------------

  T length() const
  {
    return std::sqrt(look_vector[0] * look_vector[0] + 
		     look_vector[1] * look_vector[1] + 
		     look_vector[2] * look_vector[2]);
  }
  
  virtual void print(std::ostream& Os) const = 0;
protected:
//-----------------------------------------------------------------------
/// Default constructor, protected so we can only make by derived classes.
//-----------------------------------------------------------------------

  LookVector() {}
  
  LookVector(const boost::array<T, 3>& Lv) 
  {look_vector = Lv;}

  LookVector(const T& x, const T& y, const T& z) 
  {look_vector[0] = x; look_vector[1] = y; look_vector[2] = z;}

  LookVector(const boost::math::quaternion<T>& V)
  { look_vector[0] = V.R_component_2(); look_vector[1] = V.R_component_3();
    look_vector[2] = V.R_component_4(); }

};

/****************************************************************//**
  This is a look vector in an spacecraft coordinates.
*******************************************************************/

class ScLookVector : public LookVector<double> {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  ScLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVector(const boost::array<double, 3>& Lv) : LookVector<double>(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVector(double x, double y, double z) : LookVector<double>(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  ScLookVector(const boost::math::quaternion<double>& V) : LookVector<double>(V) {}

  virtual ~ScLookVector() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in an spacecraft coordinates, including
  derivatives 
*******************************************************************/

class ScLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  ScLookVectorWithDerivative() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVectorWithDerivative(const boost::array<AutoDerivative<double> , 3>& Lv) : LookVector<AutoDerivative<double> >(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVectorWithDerivative(const AutoDerivative<double>&  x, 
			     const AutoDerivative<double>&  y, 
			     const AutoDerivative<double>&  z) : 
    LookVector<AutoDerivative<double> >(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  ScLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V) : LookVector<AutoDerivative<double> >(V) {}

  ScLookVectorWithDerivative(const ScLookVector& Slv)
    : LookVector<AutoDerivative<double> >(Slv.look_vector[0],
					  Slv.look_vector[1],
					  Slv.look_vector[2])
  { } 

  virtual ~ScLookVectorWithDerivative() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in CartesianInertial coordinates.
*******************************************************************/

class CartesianInertialLookVector : public LookVector<double> {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianInertialLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVector(const boost::array<double, 3>& Lv) : LookVector<double>(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVector(double x, double y, double z) : LookVector<double>(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  CartesianInertialLookVector(const boost::math::quaternion<double>& V) : LookVector<double>(V) {}

  virtual ~CartesianInertialLookVector() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in CartesianInertial coordinates.
*******************************************************************/

class CartesianInertialLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianInertialLookVectorWithDerivative() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVectorWithDerivative(const boost::array<AutoDerivative<double>, 3>& Lv) : LookVector<AutoDerivative<double> >(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVectorWithDerivative(const AutoDerivative<double>& x, 
					    const AutoDerivative<double>& y, 
					    const AutoDerivative<double>& z) 
    : LookVector<AutoDerivative<double> >(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  CartesianInertialLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V) : LookVector<AutoDerivative<double> >(V) {}

  virtual ~CartesianInertialLookVectorWithDerivative() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in CartesianFixed coordinates.
*******************************************************************/

class CartesianFixedLookVector : public LookVector<double> {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianFixedLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianFixedLookVector(const boost::array<double, 3>& Lv) : LookVector<double>(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianFixedLookVector(double x, double y, double z) : LookVector<double>(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  CartesianFixedLookVector(const boost::math::quaternion<double>& V) : LookVector<double>(V) {}

  CartesianFixedLookVector(const GroundCoordinate& From,
			   const GroundCoordinate& To);

  virtual ~CartesianFixedLookVector() {}
  virtual void print(std::ostream& Os) const;

  static CartesianFixedLookVector solar_look_vector(const Time& T);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in CartesianFixed coordinates.
*******************************************************************/

class CartesianFixedLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianFixedLookVectorWithDerivative() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianFixedLookVectorWithDerivative(const boost::array<AutoDerivative<double>, 3>& Lv) : LookVector<AutoDerivative<double> >(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------


  CartesianFixedLookVectorWithDerivative(const AutoDerivative<double>& x, 
					 const AutoDerivative<double>& y, 
					 const AutoDerivative<double>& z) 
    : LookVector<AutoDerivative<double> >(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------

  CartesianFixedLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V) : LookVector<AutoDerivative<double> >(V) {}
  virtual ~CartesianFixedLookVectorWithDerivative() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in LocalNorth coordinates. Note that there are
  2 common local north coordinate. The first is ENU (for "East, North,
  Up") where the local east axis is 'x', north is 'y' and up is Z. The
  other is NED (for North, East, Down), where local north is 'x', east
  is 'y' and 'z' is down. NED is used commonly for aircraft data
  (see AircraftOrbitData), since you tend to look down. This class 
  is for ENU, e.g., for calculating view zenith and azimuth angles.
*******************************************************************/

class LnLookVector : public LookVector<double> {
public:
//-----------------------------------------------------------------------
/// Constructor that translates a CartesianFixedLookVector to a
/// LnLookVector, using the coordinate system at the given Ref_pt.
//-----------------------------------------------------------------------

  LnLookVector(const CartesianFixedLookVector& Lv, 
	       const GroundCoordinate& Ref_pt)
  {
    boost::math::quaternion<double> to_ln = cf_to_enu(Ref_pt);
    look_quaternion(to_ln * Lv.look_quaternion() * conj(to_ln));
  }

//-----------------------------------------------------------------------
/// Translate from LnLookVector to CartesianFixedLookVector, using the
/// coordinate system at the given Ref_pt.
//-----------------------------------------------------------------------

  CartesianFixedLookVector to_cf(const GroundCoordinate& Ref_pt) const
  {
    boost::math::quaternion<double> to_cf = enu_to_cf(Ref_pt);
    return CartesianFixedLookVector(to_cf * look_quaternion() * conj(to_cf));
  }

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(const boost::array<double, 3>& Lv) : LookVector<double>(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(double x, double y, double z) : LookVector<double>(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(const boost::math::quaternion<double>& V) : LookVector<double>(V) {}

  virtual ~LnLookVector() {}
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Return view zenith angle in degrees.
//-----------------------------------------------------------------------

  double view_zenith() const
  { return acos(direction()[2]) * Constant::rad_to_deg; }

//-----------------------------------------------------------------------
/// Return view azimuth angle in degrees.
//-----------------------------------------------------------------------
  
  double view_azimuth() const
  { return atan2(look_vector[0], look_vector[1]) * Constant::rad_to_deg +
      180.0; }

//-----------------------------------------------------------------------
/// Return quaternion to go from CartesianFixed to ENU coordinates for
/// the given location.
//-----------------------------------------------------------------------
  static boost::math::quaternion<double> 
  cf_to_enu(const GroundCoordinate& Ref_pt)
  { return conj(enu_to_cf(Ref_pt)); }

  static boost::math::quaternion<double> 
  enu_to_cf(const GroundCoordinate& Ref_pt);
  
  static LnLookVector solar_look_vector(const Time& T, 
					const GroundCoordinate& Ref_pt);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in LocalNorth coordinates. Note that there are
  2 common local north coordinate. The first is ENU (for "East, North,
  Up") where the local east axis is 'x', north is 'y' and up is Z. The
  other is NED (for North, East, Down), where local north is 'x', east
  is 'y' and 'z' is down. NED is used commonly for aircraft data
  (see AircraftOrbitData), since you tend to look down. This class 
  is for ENU, e.g., for calculating view zenith and azimuth angles.
*******************************************************************/

class LnLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
//-----------------------------------------------------------------------
/// Constructor that translates a CartesianFixedLookVectorWithDerivative to a
/// LnLookVectorWithDerivative, using the coordinate system at the given Ref_pt.
//-----------------------------------------------------------------------

  LnLookVectorWithDerivative(const CartesianFixedLookVectorWithDerivative& Lv, 
	       const GroundCoordinate& Ref_pt)
  {
    boost::math::quaternion<double> to_ln = LnLookVector::cf_to_enu(Ref_pt);
    look_quaternion(to_ln * Lv.look_quaternion() * conj(to_ln));
  }

//-----------------------------------------------------------------------
/// Translate from LnLookVectorWithDerivative to 
/// CartesianFixedLookVectorWithDerivative, using the
/// coordinate system at the given Ref_pt.
//-----------------------------------------------------------------------

  CartesianFixedLookVectorWithDerivative 
  to_cf(const GroundCoordinate& Ref_pt) const
  {
    boost::math::quaternion<double> to_cf = LnLookVector::enu_to_cf(Ref_pt);
    return CartesianFixedLookVectorWithDerivative
      (to_cf * look_quaternion() * conj(to_cf));
  }

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVectorWithDerivative() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVectorWithDerivative(const boost::array<AutoDerivative<double>, 3>& Lv)
    : LookVector<AutoDerivative<double> >(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVectorWithDerivative(const AutoDerivative<double>& x, 
			     const AutoDerivative<double>& y, 
			     const AutoDerivative<double>& z) : 
    LookVector<AutoDerivative<double> >(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V) 
    : LookVector<AutoDerivative<double> >(V) {}

  virtual ~LnLookVectorWithDerivative() {}
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Return view zenith angle in degrees.
//-----------------------------------------------------------------------

  AutoDerivative<double> view_zenith() const
  { return std::acos(direction()[2]) * Constant::rad_to_deg; }

//-----------------------------------------------------------------------
/// Return view azimuth angle in degrees.
//-----------------------------------------------------------------------
  
  AutoDerivative<double> view_azimuth() const
  { return std::atan2(look_vector[0], look_vector[1]) * Constant::rad_to_deg +
      180.0; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in Detector Coordinate System coordinates
*******************************************************************/

class DcsLookVector : public LookVector<double> {
public:
//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(const boost::array<double, 3>& Lv) : LookVector<double>(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(double x, double y, double z) : LookVector<double>(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(const boost::math::quaternion<double>& V) : LookVector<double>(V) {}

  virtual ~DcsLookVector() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a look vector in Detector Coordinate System coordinates, 
  including derivatives 
*******************************************************************/

class DcsLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  DcsLookVectorWithDerivative() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVectorWithDerivative(const boost::array<AutoDerivative<double> , 3>& Lv) : LookVector<AutoDerivative<double> >(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVectorWithDerivative(const AutoDerivative<double>&  x, 
			     const AutoDerivative<double>&  y, 
			     const AutoDerivative<double>&  z) : 
    LookVector<AutoDerivative<double> >(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  DcsLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V) : LookVector<AutoDerivative<double> >(V) {}

  virtual ~DcsLookVectorWithDerivative() {}
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(ScLookVector);
GEOCAL_EXPORT_KEY(ScLookVectorWithDerivative);
GEOCAL_EXPORT_KEY(CartesianInertialLookVector);
GEOCAL_EXPORT_KEY(CartesianInertialLookVectorWithDerivative);
GEOCAL_EXPORT_KEY(CartesianFixedLookVector);
GEOCAL_EXPORT_KEY(CartesianFixedLookVectorWithDerivative);
GEOCAL_EXPORT_KEY(LnLookVector);
GEOCAL_EXPORT_KEY(LnLookVectorWithDerivative);
GEOCAL_EXPORT_KEY(DcsLookVector);
GEOCAL_EXPORT_KEY(DcsLookVectorWithDerivative);
#endif
