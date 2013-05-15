// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "coordinate_converter.h"
#include "ogr_coordinate.h"
%}

%geocal_shared_ptr(GeoCal::CoordinateConverter);
%geocal_shared_ptr(GeoCal::GeodeticConverter);

%shared_ptr_dynamic_list(GeoCal::CoordinateConverter,
			 GeoCal::GeodeticConverter,
			 GeoCal::OgrCoordinateConverter)

namespace GeoCal {
class CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const = 0;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const = 0;
  virtual bool is_same(const CoordinateConverter& Conv) const;
  std::string print_to_string() const;
};

class GeodeticConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pythoncode {
def __reduce__(self):
  return _new_from_init, (self.__class__,)
  }
};
}
