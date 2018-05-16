// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ogr_coordinate.h"
#include "planet_coordinate.h"  
%}
%base_import(generic_object)
%base_import(ground_coordinate)
%base_import(coordinate_converter)
%import "geodetic.i"
%import "planet_coordinate.i"
%geocal_shared_ptr(GeoCal::OgrWrapper);
%geocal_shared_ptr(GeoCal::OgrCoordinate);
%geocal_shared_ptr(GeoCal::OgrCoordinateConverter);

namespace GeoCal {
class OgrWrapper : public GenericObject {
public:
  OgrWrapper(const std::string& Wkt);
  OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  static boost::shared_ptr<OgrWrapper> from_epsg(int Epsg_id);
  static boost::shared_ptr<OgrWrapper> 
  from_proj4(const std::string& Proj4_string);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OGRSpatialReference>)
  %python_attribute(transform, const OGRCoordinateTransformation*)
  %python_attribute(inverse_transform, const OGRCoordinateTransformation*)
  %python_attribute(cf_transform, const OGRCoordinateTransformation*)
  %python_attribute(cf_inverse_transform, const OGRCoordinateTransformation*)
  %python_attribute(projected_cs_type_geo_key, std::string)
  %python_attribute(pcs_citation_geo_key, std::string)
  %python_attribute(geogcs_name, std::string)
  %python_attribute(wkt, std::string)
  %python_attribute(pretty_wkt, std::string)
  %python_attribute(naif_code, int)
  std::string print_to_string() const;
  %pickle_serialization();
};

class OgrCoordinate : public GroundCoordinate {
public:
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		double x, double y, double z);
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const Geodetic& G);
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const GroundCoordinate& G);
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const Planetocentric& G);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OgrWrapper>);
  %python_attribute(utm_zone, int);
  double x;
  double y;
  double z;
  virtual int naif_code() const;
  static OgrCoordinate to_utm(const Geodetic& Gc, int zone = -999);
  %pickle_serialization();
};

class OgrCoordinateConverter : public CoordinateConverter {
public:
  OgrCoordinateConverter(boost::shared_ptr<OgrWrapper> Ogr);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OgrWrapper>)
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const;
  static boost::shared_ptr<OgrCoordinateConverter>
  utm_converter(int Zone);
  %pickle_serialization();
};

}

// List of things "import *" will include
%python_export("OgrWrapper", "OgrCoordinate", "OgrCoordinateConverter")
