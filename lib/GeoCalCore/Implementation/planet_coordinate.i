// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "planet_coordinate.h"
%}
%base_import(ground_coordinate)
%base_import(dem)
%base_import(coordinate_converter)
%import "orbit.i"
%import "simple_dem.i"

%geocal_shared_ptr(GeoCal::PlanetConstant);
%geocal_shared_ptr(GeoCal::PlanetFixed);
%geocal_shared_ptr(GeoCal::PlanetInertial);
%geocal_shared_ptr(GeoCal::Planetocentric);
%geocal_shared_ptr(GeoCal::PlanetocentricConverter);
%geocal_shared_ptr(GeoCal::PlanetSimpleDem);

namespace GeoCal {
class PlanetConstant {
public:
  enum {MARS_NAIF_CODE=499, EUROPA_NAIF_CODE=502};
  static double a(int Naif_code);
  static double b(int Naif_code);
  static double esq(int Naif_code);
  static double flattening(int Naif_code);
  static double inverse_flattening(int Naif_code);
  static std::string name(int Naif_code);
};

class PlanetFixed : public CartesianFixed {
public:
  PlanetFixed(const GroundCoordinate& Gc);
  PlanetFixed(double X, double Y, double Z, int NAIF_CODE);
  PlanetFixed(const boost::array<double, 3>& Pos, int NAIF_CODE);
  PlanetFixed(int NAIF_CODE = -1);
  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;
  virtual boost::shared_ptr<CartesianFixed> 
    create(boost::array<double, 3> P) const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual void cf_to_ci_with_vel(const Time& T, double Cf_to_ci[6][6]) const;
  // Temp
  //Planetocentric<NAIF_CODE> convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0)
    const;
  static PlanetFixed target_position
  (const std::string& Target_name, const Time& T, int Naif_code);
  static boost::shared_ptr<QuaternionOrbitData> orbit_data
  (const std::string& Target_name, 
   const std::string& Spacecraft_reference_frame_name, const Time& T,
   int Naif_code);
  virtual int naif_code() const;
  %pickle_serialization();
};

class Planetocentric : public GroundCoordinate {
public:
  Planetocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Planetocentric(double Latitude, double Longitude, double Height_ellipsoid,
		 int Naif_code);
  Planetocentric(int Naif_code=-1);
  virtual int naif_code() const;
  %python_attribute(height_reference_surface, double);
  %python_attribute(latitude, double);
  %python_attribute(longitude, double);
  %python_attribute(planet_radius, double);
  %pickle_serialization();
};

class PlanetInertial : public CartesianInertial {
public:
  PlanetInertial(int Naif_code = -1);
  PlanetInertial(double X, double Y, double Z, int Naif_code);
  PlanetInertial(const boost::array<double, 3>& Pos, int Naif_code);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const ;
  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
    const;
  virtual int naif_code() const;
  %pickle_serialization();
};

class PlanetocentricConverter : public CoordinateConverter {
public:
  PlanetocentricConverter(int Naif_code = -1);
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  virtual int naif_code() const;
  %pickle_serialization();
};

class PlanetSimpleDem : public Dem {
public:
  PlanetSimpleDem(double H, int Naif_code);
  PlanetSimpleDem(int Naif_code = -1);
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  %python_attribute_with_set(h, double)
  %python_attribute_with_set(naif_code, int)
  %pickle_serialization();
};
}
