// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "planet_coordinate.h"
%}
%base_import(ground_coordinate)
namespace GeoCal {
template<int NCODE> class PlanetConstant {
public:
  // Note, we'd like this to be properties. But it turns out getting
  // class properties to work is a bit involved in python. Not really 
  // worth doing, although we can always come back to this if it becomes
  // an issue.
  static double planet_a();
  static double planet_b();
  static double planet_esq();
  static std::string planet_name();
};

template<int NAIF_CODE> class PlanetFixed : public CartesianFixed {
public:
  PlanetFixed(const GroundCoordinate& Gc);
  PlanetFixed(double X, double Y, double Z);
  PlanetFixed(const boost::array<double, 3>& Pos);
  PlanetFixed();
  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;
  virtual boost::shared_ptr<CartesianFixed> 
    create(boost::array<double, 3> P) const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  Planetocentric<NAIF_CODE> convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0)
    const;
  static PlanetFixed<NAIF_CODE> target_position
  (const std::string& Target_name, const Time& T);
  %pickle_init(1, self.position[0], self.position[1], self.position[2])
};

template<int NAIF_CODE> class Planetocentric : public GroundCoordinate {
public:
  Planetocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Planetocentric(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Planetocentric();
  %python_attribute(height_reference_surface, double);
  %python_attribute(latitude, double);
  %python_attribute(longitude, double);
  %pickle_init(1, self.latitude, self.longitude, self.height_reference_surface)
};

template<int NAIF_CODE> class PlanetInertial : public CartesianInertial {
public:
  PlanetInertial();
  PlanetInertial(double X, double Y, double Z);
  PlanetInertial(const boost::array<double, 3>& Pos);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const ;
  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
    const;
  %pickle_init(1, self.position[0], self.position[1], self.position[2])
};

}

%geocal_shared_ptr(GeoCal::PlanetConstant<499>);
%geocal_shared_ptr(GeoCal::PlanetFixed<499>);
%geocal_shared_ptr(GeoCal::PlanetInertial<499>);
%geocal_shared_ptr(GeoCal::Planetocentric<499>);
%template(MarsConstant) GeoCal::PlanetConstant<499>;
%template(MarsFixed) GeoCal::PlanetFixed<499>;
%template(MarsInertial) GeoCal::PlanetInertial<499>;
%template(MarsPlanetocentric) GeoCal::Planetocentric<499>;

%geocal_shared_ptr(GeoCal::PlanetConstant<502>);
%geocal_shared_ptr(GeoCal::PlanetFixed<502>);
%geocal_shared_ptr(GeoCal::PlanetInertial<502>);
%geocal_shared_ptr(GeoCal::Planetocentric<502>);
%template(EuropaConstant) GeoCal::PlanetConstant<502>;
%template(EuropaFixed) GeoCal::PlanetFixed<502>;
%template(EuropaInertial) GeoCal::PlanetInertial<502>;
%template(EuropaPlanetocentric) GeoCal::Planetocentric<502>;
