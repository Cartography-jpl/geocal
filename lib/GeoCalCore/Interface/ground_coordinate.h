#ifndef GROUND_COORDINATE_H
#define GROUND_COORDINATE_H
#include "printable.h"
#include "geocal_time.h"
#include "toolkit_coordinate_interface.h"
#include "look_vector.h"
#include "geocal_quaternion.h"
#include <ostream>
#include <sstream>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

namespace GeoCal {

class CartesianFixed;		// Forward declaration, defined below.
class CartesianInertial;	// Forward declaration, defined below.

/****************************************************************//**
  This represents a location on the ground. It is a base class, each of the
  derived classes represent different coordinate systems that can be used to
  express the ground location. Any ground coordinate can be converted to
  CartesianFixed coordinates.

  Because it is commonly needed, we also can convert to latitude,
  longitude and height_reference_surface. The default is just convert
  first to CartesianFixed, and then convert to latitude, longitude or
  height_reference_surface, but derived classes can supply more
  efficient versions of this if desired.
*******************************************************************/

class GroundCoordinate : public Printable<GroundCoordinate> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GroundCoordinate() {}

//-----------------------------------------------------------------------
/// Convert to latitude, longitude, height_reference_surface in a
/// single call. By default, just convert to CartesianFixed and then
/// get latitude, longitude, and height_reference_surface. but derived
/// classes can do something more efficient.
//-----------------------------------------------------------------------

  inline virtual void lat_lon_height(double& Latitude, double& Longitude, 
			     double& Height_reference_surface) const; 

//-----------------------------------------------------------------------
/// Return latitude in degrees. By default we just convert to 
/// CartesianFixed and then to latitude, but derived classes can supply
/// more efficient versions of these if needed. Latitude is -90 to 90.
//-----------------------------------------------------------------------

  inline virtual double latitude() const;

//-----------------------------------------------------------------------
/// Return longitude in degrees. By default we just convert to 
/// CartesianFixed and then to longitude, but derived classes can supply
/// more efficient versions of these if needed. Longitude is -180 to 180.
//-----------------------------------------------------------------------

  inline virtual double longitude() const;

//-----------------------------------------------------------------------
/// Return height in meters above reference surface (e.g., WGS-84 for
/// the earth). By default we
/// just convert to CartesianFixed and then to height, but derived
/// classes can supply  more efficient versions of these if needed.
//-----------------------------------------------------------------------

  inline virtual double height_reference_surface() const;

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const = 0;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};

/****************************************************************//**
  This gives the location in a Cartesian inertial coordinate system
  (.e.g., ECI). See CartesianFixed for a discussion on converting to
  and from CartesianFixed.
*******************************************************************/

class CartesianInertial : public Printable<CartesianInertial> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------
  virtual ~CartesianInertial() {}

//-----------------------------------------------------------------------
/// Convert to a CartesianFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const = 0;

//-----------------------------------------------------------------------
/// Calculate matrix that can be used to convert CartesianFixed to
/// CartesianInertial at the given Time. The matrix for going from
/// CartesianInertial to CartesianFixed is the transpose of this matrix.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;

//-----------------------------------------------------------------------
/// Calculate matrix that can be used to convert CartesianInertial to
/// CartesianFixed at the given Time, including velocity. Note that
/// unlike the 3x3 matrix, this is *not* orthogonal so the transpose
/// is not the inverse.
//-----------------------------------------------------------------------

  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) 
    const = 0;

//-----------------------------------------------------------------------
/// Calculate quaternion that can be used to convert CartesianFixed to
/// CartesianInertial at the given Time.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const
  { 
    double ci_to_cf_m[3][3];
    ci_to_cf(T, ci_to_cf_m);
    return matrix_to_quaternion(ci_to_cf_m);
  }

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const = 0;

//-----------------------------------------------------------------------
/// Find the intersection with the surface at the given height
/// starting at this point and looking in the given direction. The
/// intersection is done in an approximate but quicker manner, see the
/// derived classes for the approximation used.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;

//-----------------------------------------------------------------------
/// Position
//-----------------------------------------------------------------------
  
  boost::array<double, 3> position;
};

/****************************************************************//**
  This gives the fixed Cartesian Coordinates for a ground coordinate.
  This is an abstract base class, there are CartesianFixed for the
  Earth (Ecr), Mars (MarsFixed), etc.

  We want to be able to convert to and from CartesianInertial
  coordinates. However, we want to abstract out the toolkit used to 
  do this. We use the toolkit pointed to by 
  CartesianFixed::toolkit_coordinate_interface. If either SPICE or
  SDP is available, then that toolkit is the one used for the
  conversion. If we have both available, then we default to SPICE. You
  can change the choice by updating the variable
  CartesianFixed::toolkit_coordinate_interface.
*******************************************************************/

class CartesianFixed : public GroundCoordinate {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------
  virtual ~CartesianFixed() {}

//-----------------------------------------------------------------------
/// Calculate matrix that can be used to convert CartesianFixed to
/// CartesianInertial at the given Time. The matrix for going from
/// CartesianInertial to CartesianFixed is the transpose of this matrix.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;

//-----------------------------------------------------------------------
/// Calculate matrix that can be used to convert CartesianFixed to
/// CartesianInertial at the given Time, including velocity. Note that
/// unlike the 3x3 matrix, this is *not* orthogonal so the transpose
/// is not the inverse.
//-----------------------------------------------------------------------

  virtual void cf_to_ci_with_vel(const Time& T, double Cf_to_ci[6][6]) 
    const = 0;

//-----------------------------------------------------------------------
/// Calculate quaternion that can be used to convert CartesianFixed to
/// CartesianInertial at the given Time.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const
  { 
    double ci_to_cf_m[3][3];
    ci_to_cf(T, ci_to_cf_m);
    return matrix_to_quaternion(ci_to_cf_m);
  }

//-----------------------------------------------------------------------
/// This calls create.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const
  { return create(position); }

//-----------------------------------------------------------------------
/// Convert to CartesianInertial
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> convert_to_ci(const Time& T) 
    const = 0;

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianFixed this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
    create(boost::array<double, 3> P) const = 0;

//-----------------------------------------------------------------------
/// Find the intersection with the surface at the given height
/// starting at this point and looking in the given direction. The
/// intersection is done in an approximate but quicker manner, see the
/// derived classes for the approximation used.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;

//-----------------------------------------------------------------------
/// This is the radius of the largest sphere that fits completely
/// into the reference surface (e.g., radius for a reference
/// sphere, minimum of a and b for a ellipsoid).
//-----------------------------------------------------------------------

  virtual double min_radius_reference_surface() const = 0;

//-----------------------------------------------------------------------
/// Position
//-----------------------------------------------------------------------
  
  boost::array<double, 3> position;

//-----------------------------------------------------------------------
/// Interface to use for converting to and from CartesianInertial
//-----------------------------------------------------------------------

  static ToolkitCoordinateInterface* toolkit_coordinate_interface;
};

// These are defined here instead of up in the class above because
// we need the declaration of CartesianFixed first.
inline void GroundCoordinate::lat_lon_height
(double& Latitude, double& Longitude, double& Height_reference_surface) const
{
  boost::shared_ptr<CartesianFixed> cf = convert_to_cf();
  Latitude = cf->latitude();
  Longitude = cf->longitude();
  Height_reference_surface = cf->height_reference_surface(); 
}
inline double GroundCoordinate::latitude() const 
{ return convert_to_cf()->latitude(); }
inline double GroundCoordinate::longitude() const 
{ return convert_to_cf()->longitude(); }
inline double GroundCoordinate::height_reference_surface() const 
{ return convert_to_cf()->height_reference_surface();}

double distance(const GroundCoordinate& G1, const GroundCoordinate& G2);

void convert_position_and_velocity
(const Time& T,
 const CartesianFixed& Cf,
 const boost::array<double, 3>& Vel_cf,
 boost::shared_ptr<CartesianInertial>& Ci,
 boost::array<double, 3>& Vel_ci
 );

void convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianFixed& Cf,
 const boost::array<AutoDerivative<double>,3>& Cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_cf,
 boost::shared_ptr<CartesianInertial>& Ci,
 boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_ci,
 boost::math::quaternion<AutoDerivative<double> > Cf_to_ci_q
 );

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

inline void convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianFixed& Cf,
 const boost::array<AutoDerivative<double>,3>& Cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_cf,
 boost::shared_ptr<CartesianInertial>& Ci,
 boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_ci
 )
{
  boost::math::quaternion<AutoDerivative<double> > junk;
  convert_position_and_velocity(T, Cf, Cf_with_der, Vel_cf, Ci, Ci_with_der, 
				Vel_ci, junk);
}

void convert_position_and_velocity
(const Time& T,
 const CartesianInertial& Ci,
 const boost::array<double, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 boost::array<double, 3>& Vel_cf,
 boost::math::quaternion<double>& Ci_to_cf_q
 );

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

inline void convert_position_and_velocity
(const Time& T,
 const CartesianInertial& Ci,
 const boost::array<double, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 boost::array<double, 3>& Vel_cf
 )
{
  boost::math::quaternion<double> junk;
  convert_position_and_velocity(T, Ci, Vel_ci, Cf, Vel_cf, junk);
}

void convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianInertial& Ci,
 const boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 boost::array<AutoDerivative<double>, 3>& Cf_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_cf,
 boost::math::quaternion<AutoDerivative<double> >& Ci_to_cf_q
 );

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

inline void convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianInertial& Ci,
 const boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 boost::array<AutoDerivative<double>, 3>& Cf_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_cf
 )
{
  boost::math::quaternion<AutoDerivative<double> > junk;
  convert_position_and_velocity(T, Ci, Ci_with_der, Vel_ci, Cf, Cf_with_der,
				Vel_cf, junk);
}

 
}
#endif

