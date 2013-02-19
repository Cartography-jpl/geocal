// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "ground_coordinate.h"
#include "geocal_time.h"
#include "ecr.h"
#include "eci.h"
#include "eci_tod.h"
#include "eci_tod.h"
#include "eci_tod_burl.h"
#include "ogr_coordinate.h"
%}

%geocal_shared_ptr(GroundCoordinate);
%geocal_shared_ptr(CartesianFixed);
%geocal_shared_ptr(CartesianInertial);

%shared_ptr_dynamic_list(GeoCal::GroundCoordinate,
			 GeoCal::Ecr,
			 GeoCal::Geocentric,
			 GeoCal::Geodetic,
			 GeoCal::OgrCoordinate,
			 GeoCal::CartesianFixed);
%shared_ptr_dynamic_list(GeoCal::CartesianInertial,
			 GeoCal::Eci,
			 GeoCal::EciTod,
			 GeoCal::EciTodBurl
			 );
%shared_ptr_dynamic_list(GeoCal::CartesianFixed,
			 GeoCal::Ecr
			 );

namespace GeoCal {
class CartesianFixed;
class GroundCoordinate {
public:
  %python_attribute(latitude, virtual double)
  %python_attribute(longitude, virtual double)
  %python_attribute(height_reference_surface, virtual double)
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const = 0;
  std::string print_to_string() const;
};

class CartesianInertial {
public:
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const = 0;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;
  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const = 0;
  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;
  std::string print_to_string() const;
  %python_attribute_boost_array(position, double, 3)
};

class CartesianFixed : public GroundCoordinate {
public:
  virtual boost::shared_ptr<CartesianInertial> convert_to_ci(const Time& T) 
    const = 0;
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual boost::shared_ptr<CartesianFixed> 
    create(boost::array<double, 3> P) const = 0;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;
  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;
  %python_attribute(min_radius_reference_surface, virtual double)
  %python_attribute_boost_array(position, double, 3)
};

double distance(const GroundCoordinate& G1, const GroundCoordinate& G2);

}

%template(Vector_GroundCoordinate) std::vector<boost::shared_ptr<GeoCal::GroundCoordinate> >;
