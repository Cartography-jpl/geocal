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
  virtual int naif_code() const;
  %pickle_serialization();
};

class Geocentric : public GroundCoordinate {
public:
  Geocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Geocentric(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Geocentric();
  virtual int naif_code() const;
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("Geodetic", "Geocentric")
