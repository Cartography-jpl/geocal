// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"
%{
#include "coordinate_converter.h"
%}

%base_import(generic_object)
%import "look_vector.i"
%import "ground_coordinate.i"

%geocal_shared_ptr(GeoCal::CoordinateConverter);
%geocal_shared_ptr(GeoCal::GeodeticConverter);
%geocal_shared_ptr(GeoCal::Geodetic360Converter);
%geocal_shared_ptr(GeoCal::GeodeticRadianConverter);
%geocal_shared_ptr(GeoCal::GeodeticRadian2piConverter);

namespace GeoCal {
class CoordinateConverter : public GenericObject {
public:
  enum { EARTH_NAIF_CODE = 399 };
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const = 0;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const = 0;
  %python_attribute(naif_code, int);
  virtual boost::shared_ptr<GroundCoordinate> create
  (const GroundCoordinate& Gc) const;
  virtual bool is_same(const CoordinateConverter& Conv) const;
  std::string print_to_string() const;
};

class GeodeticConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pickle_serialization();
};

class Geodetic360Converter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pickle_serialization();
};
  
class GeodeticRadianConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pickle_serialization();
};

class GeodeticRadian2piConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("CoordinateConverter", "GeodeticConverter", "Geodetic360Converter", "GeodeticRadianConverter", "GeodeticRadian2piConverter")


