// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "ogr_coordinate.h"
%}

%geocal_shared_ptr(OgrWrapper);
%geocal_shared_ptr(OgrCoordinate);
%geocal_shared_ptr(OgrCoordinateConverter);

namespace GeoCal {
class OgrWrapper {
public:
  OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  static boost::shared_ptr<OgrWrapper> from_epsg(int Epsg_id);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OGRSpatialReference>)
  %python_attribute(transform, const OGRCoordinateTransformation&)
  %python_attribute(inverse_transform, const OGRCoordinateTransformation&)
  %python_attribute(projected_cs_type_geo_key, std::string)
  %python_attribute(pcs_citation_geo_key, std::string)
  std::string print_to_string() const;
};

class OgrCoordinate : public GroundCoordinate {
public:
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		double x, double y, double z);
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const Geodetic& G);
  Geodetic to_geodetic() const;
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OgrWrapper>)
  double x;
  double y;
  double z;
  static OgrCoordinate to_utm(const Geodetic& Gc, int zone = -999);
};

class OgrCoordinateConverter : public CoordinateConverter {
public:
  OgrCoordinateConverter(boost::shared_ptr<OgrWrapper> Ogr);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OgrWrapper>)
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const;
};

}

