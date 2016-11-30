// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geodetic.h"
%}

%import "look_vector.i"
%base_import(ground_coordinate)

%geocal_shared_ptr(GeoCal::Geodetic);
%geocal_shared_ptr(GeoCal::Geocentric);
namespace GeoCal {
class Geodetic : public GroundCoordinate {
public:
  Geodetic(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Geodetic(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Geodetic();
  %pickle_serialization();
};

class Geocentric : public GroundCoordinate {
public:
  Geocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Geocentric(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Geocentric();
  %pickle_serialization();
};

}
