// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ogr_coordinate.h"
%}

%geocal_shared_ptr(GeoCal::OgrWrapper);
%geocal_shared_ptr(GeoCal::OgrCoordinate);
%geocal_shared_ptr(GeoCal::OgrCoordinateConverter);

namespace GeoCal {
class OgrWrapper {
public:
  OgrWrapper(const std::string& Wkt);
  OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  static boost::shared_ptr<OgrWrapper> from_epsg(int Epsg_id);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OGRSpatialReference>)
  %python_attribute(transform, const OGRCoordinateTransformation&)
  %python_attribute(inverse_transform, const OGRCoordinateTransformation&)
  %python_attribute(projected_cs_type_geo_key, std::string)
  %python_attribute(pcs_citation_geo_key, std::string)
  %python_attribute(wkt, std::string)
  %python_attribute(pretty_wkt, std::string)
  std::string print_to_string() const;
  %pickle_init(1, self.wkt)
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
  %pickle_init(1, self.ogr, self.x, self.y, self.z)
};

class OgrCoordinateConverter : public CoordinateConverter {
public:
  OgrCoordinateConverter(boost::shared_ptr<OgrWrapper> Ogr);
  %python_attribute2(ogr, ogr_ptr, boost::shared_ptr<OgrWrapper>)
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const;
  %pickle_init(1, self.ogr)
};

}

