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
%base_import(dem)
%import "image_ground_connection.i"
%import "array_ad.i"
%import "auto_derivative.i"

%geocal_shared_ptr(GeoCal::LocalRcParameter);
%geocal_shared_ptr(GeoCal::LocalRectangularCoordinate);
%geocal_shared_ptr(GeoCal::LocalRcConverter);
%geocal_shared_ptr(GeoCal::LocalZDem);

namespace GeoCal {
class LocalRcParameter : public GenericObject {
public:
  enum CoordinateCreation {FOLLOW_SAMPLE_LOCAL, FOLLOW_LINE_LOCAL,
    FOLLOW_SAMPLE_FULL, FOLLOW_LINE_FULL};
  LocalRcParameter();
  LocalRcParameter(const ImageGroundConnection& Igc, double Height = 0,
		   double Line = -1, double Sample = -1,
		   CoordinateCreation C = FOLLOW_SAMPLE_LOCAL);
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
  ArrayAd<double, 1> convert_to_cf(const AutoDerivative<double>& X,
				   const AutoDerivative<double>& Y,
				   const AutoDerivative<double>& Z) const;
  %python_attribute(parameter, boost::shared_ptr<LocalRcParameter>);
  %pickle_serialization();
};

class LocalZDem : public Dem {
public:
  LocalZDem(const boost::shared_ptr<LocalRcConverter>& Cconv,
	    int Z);
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  %python_attribute(z, double);
  %python_attribute(coordinate_converter, boost::shared_ptr<LocalRcConverter>);
  %pickle_serialization();
};
  
}

// List of things "import *" will include
%python_export("LocalRcParameter", "LocalRectangularCoordinate", "LocalRcConverter", "LocalZDem")

