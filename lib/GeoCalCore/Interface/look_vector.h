#ifndef LOOK_VECTOR_H
#define LOOK_VECTOR_H
#include "printable.h"
#include <boost/array.hpp>
#include <boost/math/quaternion.hpp>

namespace GeoCal {

/****************************************************************//**
  This is a look vector in an unspecified coordinate system. Derived
  classes specify the coordinate system.
*******************************************************************/

class LookVector : public Printable<LookVector> {
public:
  virtual ~LookVector() {}

//-----------------------------------------------------------------------
/// Look vector. This should be in meters. This gives both a direction
/// and length.
//-----------------------------------------------------------------------

  boost::array<double, 3> look_vector;

//-----------------------------------------------------------------------
/// Look vector as a quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> look_quaternion() const
  { return boost::math::quaternion<double>(0, look_vector[0], look_vector[1],
					   look_vector[2]); }

//-----------------------------------------------------------------------
/// Set look vector using a quaternion.
//-----------------------------------------------------------------------
  
  void look_quaternion(const boost::math::quaternion<double>& V)
  { look_vector[0] = V.R_component_2(); look_vector[1] = V.R_component_3();
    look_vector[2] = V.R_component_4(); }
  
  boost::array<double, 3> direction() const;
  double length() const;
  virtual void print(std::ostream& Os) const = 0;
protected:
//-----------------------------------------------------------------------
/// Default constructor, protected so we can only make by derived classes.
//-----------------------------------------------------------------------

  LookVector() {}
  
  LookVector(const boost::array<double, 3>& Lv) 
  {look_vector = Lv;}

  LookVector(double x, double y, double z) 
  {look_vector[0] = x; look_vector[1] = y; look_vector[2] = z;}

  LookVector(const boost::math::quaternion<double>& V)
  { look_vector[0] = V.R_component_2(); look_vector[1] = V.R_component_3();
    look_vector[2] = V.R_component_4(); }

};

/****************************************************************//**
  This is a look vector in an spacecraft coordinates.
*******************************************************************/

class ScLookVector : public LookVector {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  ScLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVector(const boost::array<double, 3>& Lv) : LookVector(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  ScLookVector(double x, double y, double z) : LookVector(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  ScLookVector(const boost::math::quaternion<double>& V) : LookVector(V) {}

  virtual ~ScLookVector() {}
  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a look vector in CartesianInertial coordinates.
*******************************************************************/

class CartesianInertialLookVector : public LookVector {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianInertialLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVector(const boost::array<double, 3>& Lv) : LookVector(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianInertialLookVector(double x, double y, double z) : LookVector(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  CartesianInertialLookVector(const boost::math::quaternion<double>& V) : LookVector(V) {}

  virtual ~CartesianInertialLookVector() {}
  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a look vector in CartesianFixed coordinates.
*******************************************************************/

class CartesianFixedLookVector : public LookVector {
public:
//-----------------------------------------------------------------------
/// Default constructor. Does not initialize look_vector.
//-----------------------------------------------------------------------

  CartesianFixedLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianFixedLookVector(const boost::array<double, 3>& Lv) : LookVector(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  CartesianFixedLookVector(double x, double y, double z) : LookVector(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor using quaternion
//-----------------------------------------------------------------------
  CartesianFixedLookVector(const boost::math::quaternion<double>& V) : LookVector(V) {}

  virtual ~CartesianFixedLookVector() {}
  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a look vector in LocalNorth coordinates
*******************************************************************/

class LnLookVector : public LookVector {
public:
//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(const boost::array<double, 3>& Lv) : LookVector(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(double x, double y, double z) : LookVector(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  LnLookVector(const boost::math::quaternion<double>& V) : LookVector(V) {}

  virtual ~LnLookVector() {}
  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a look vector in Detector Coordinate System coordinates
*******************************************************************/

class DcsLookVector : public LookVector {
public:
//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector() {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(const boost::array<double, 3>& Lv) : LookVector(Lv) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(double x, double y, double z) : LookVector(x,y,z) {}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

  DcsLookVector(const boost::math::quaternion<double>& V) : LookVector(V) {}

  virtual ~DcsLookVector() {}
  virtual void print(std::ostream& Os) const;
};

}

#endif
