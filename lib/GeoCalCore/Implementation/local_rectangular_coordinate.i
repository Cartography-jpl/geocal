// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "local_rectangular_coordinate.h"
#include "image_ground_connection.h"  
%}
%base_import(generic_object)
%base_import(ground_coordinate)
%base_import(coordinate_converter)
%import "image_ground_connection.i"

%geocal_shared_ptr(GeoCal::LocalRcParameter);
%geocal_shared_ptr(GeoCal::LocalRectangularCoordinate);
%geocal_shared_ptr(GeoCal::LocalRcConverter);

namespace GeoCal {
class LocalRcParameter : public GenericObject {
public:
  LocalRcParameter();
  LocalRcParameter(const ImageGroundConnection& Igc, double Height = 0);
  static void check_rc_assumption
  (const ImageGroundConnection& Igc, double Height = 0);
  std::string print_to_string() const;
  double cf_to_rc[3][3];
  boost::array<double, 3> cf_offset;
  %pickle_serialization();
};

class LocalRectangularCoordinate : public GroundCoordinate {
public:
  LocalRectangularCoordinate();
  LocalRectangularCoordinate(const boost::shared_ptr<LocalRcParameter>& P,
			     const GroundCoordinate& Gc);
  LocalRectangularCoordinate(const boost::shared_ptr<LocalRcParameter>& P,
			     double X, double Y, double Z);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual int naif_code() const;
  boost::shared_ptr<LocalRcParameter> parameter;
  boost::array<double, 3> position;
  %pickle_serialization();
};

class LocalRcConverter : public CoordinateConverter
{
public:
  LocalRcConverter(const boost::shared_ptr<LocalRcParameter>& P);
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %python_attribute(parameter, boost::shared_ptr<LocalRcParameter>);
  %pickle_serialization();
};
  
}

// List of things "import *" will include
%python_export("LocalRcParameter", "LocalRectangularCoordinate", "LocalRcConverter")

