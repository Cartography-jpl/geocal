// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "geodetic.h"
%}

%geocal_shared_ptr(Geodetic);
%geocal_shared_ptr(Geocentric);
namespace GeoCal {
class Geodetic : public GroundCoordinate {
public:
  Geodetic(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Geodetic(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Geodetic();
  %pickle_init(self.latitude, self.longitude, self.height_reference_surface)
};

class Geocentric : public GroundCoordinate {
public:
  Geocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  Geocentric(double Latitude, double Longitude, double Height_ellipsoid = 0);
  Geocentric();
  %pickle_init(self.latitude, self.longitude, self.height_reference_surface)
};

}
