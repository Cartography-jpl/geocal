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

namespace GeoCal {
template<int NCODE> class PlanetConstant {
public:
  // Note, we'd like this to be properties. But it turns out getting
  // class properties to work is a bit involved in python. Not really 
  // worth doing, although we can always come back to this if it becomes
  // an issue.
  static double a();
  static double b();
  static double esq();
  static double flattening();
  static double inverse_flattening();
  static std::string name();
  static int naif_code();
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
  virtual void cf_to_ci_with_vel(const Time& T, double Cf_to_ci[6][6]) const;
  Planetocentric<NAIF_CODE> convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0)
    const;
  static PlanetFixed<NAIF_CODE> target_position
  (const std::string& Target_name, const Time& T);
  static boost::shared_ptr<QuaternionOrbitData> orbit_data
  (const std::string& Target_name, 
   const std::string& Spacecraft_reference_frame_name, const Time& T);
  virtual int naif_code() const;
  %pickle_serialization();
};

template<int NAIF_CODE> class Planetocentric : public GroundCoordinate {
public:
  Planetocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Planetocentric(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Planetocentric();
  int naif_code();
  %python_attribute(height_reference_surface, double);
  %python_attribute(latitude, double);
  %python_attribute(longitude, double);
  %pickle_serialization();
};

template<int NAIF_CODE> class PlanetInertial : public CartesianInertial {
public:
  PlanetInertial();
  PlanetInertial(double X, double Y, double Z);
  PlanetInertial(const boost::array<double, 3>& Pos);
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
  int naif_code();
  %pickle_serialization();
};

template<int NAIF_CODE> class PlanetocentricConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  int naif_code();
  %pickle_serialization();
};

}

%geocal_shared_ptr(GeoCal::PlanetConstant<499>);
%geocal_shared_ptr(GeoCal::PlanetFixed<499>);
%geocal_shared_ptr(GeoCal::PlanetInertial<499>);
%geocal_shared_ptr(GeoCal::Planetocentric<499>);
%geocal_shared_ptr(GeoCal::SimpleDemT<GeoCal::Planetocentric<499> >);
%geocal_shared_ptr(GeoCal::PlanetocentricConverter<499>);
%template(MarsConstant) GeoCal::PlanetConstant<499>;
%template(MarsFixed) GeoCal::PlanetFixed<499>;
%template(MarsInertial) GeoCal::PlanetInertial<499>;
%template(MarsPlanetocentric) GeoCal::Planetocentric<499>;
%template(MarsSimpleDem) GeoCal::SimpleDemT<GeoCal::Planetocentric<499> >;
%template(MarsPlanetocentricConverter) GeoCal::PlanetocentricConverter<499>;

%geocal_shared_ptr(GeoCal::PlanetConstant<502>);
%geocal_shared_ptr(GeoCal::PlanetFixed<502>);
%geocal_shared_ptr(GeoCal::PlanetInertial<502>);
%geocal_shared_ptr(GeoCal::Planetocentric<502>);
%geocal_shared_ptr(GeoCal::SimpleDemT<GeoCal::Planetocentric<502> >);
%geocal_shared_ptr(GeoCal::PlanetocentricConverter<502>);
%template(EuropaConstant) GeoCal::PlanetConstant<502>;
%template(EuropaFixed) GeoCal::PlanetFixed<502>;
%template(EuropaInertial) GeoCal::PlanetInertial<502>;
%template(EuropaPlanetocentric) GeoCal::Planetocentric<502>;
%template(EuropaSimpleDem) GeoCal::SimpleDemT<GeoCal::Planetocentric<502> >;
%template(EuropaPlanetocentricConverter) GeoCal::PlanetocentricConverter<502>;
