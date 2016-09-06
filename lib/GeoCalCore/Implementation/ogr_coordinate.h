#ifndef OGR_COORDINATE_H
#define OGR_COORDINATE_H
#include "ground_coordinate.h"
#include "coordinate_converter.h"
#include "geodetic.h"
#include <ogr_spatialref.h>	
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a wrapper around the OGRSpatialReference class. We hold onto
  a OGRSpatialReference class and a transformation from that
  coordinate system to the Geodetic coordinate system. This class
  handles the lifetime of these objects. This is separated out from
  OgrCoordinate because typically we'll have lots of OgrCoordinates
  that have the same OgrWrapper.
*******************************************************************/

class OgrWrapper : public boost::noncopyable,
	           public Printable<OgrWrapper>  {
public:
  OgrWrapper(const std::string& Wkt);
  OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  static boost::shared_ptr<OgrWrapper> from_epsg(int Epsg_id);
  static boost::shared_ptr<OgrWrapper> 
  from_proj4(const std::string& Proj4_string);
  virtual ~OgrWrapper();

//-----------------------------------------------------------------------
/// Return underlying OGRSpatialReference.
//-----------------------------------------------------------------------

  const OGRSpatialReference& ogr() const { return *ogr_;}
  const boost::shared_ptr<OGRSpatialReference>& ogr_ptr() const { return ogr_;}

//-----------------------------------------------------------------------
/// Return transformation that takes us from our coordinate system to
/// Geodetic.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* transform() const {return ogr_transform_;}

//-----------------------------------------------------------------------
/// Return inverse of transform(). This goes from Geodetic to our
/// coordinate system.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* inverse_transform() const 
  {return ogr_inverse_transform_;}

//-----------------------------------------------------------------------
/// Return transformation that takes us from our coordinate system to
/// CartesianFixed.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* cf_transform() const
  {return ogr_cf_transform_;}

//-----------------------------------------------------------------------
/// Return inverse of cf_transform(). This goes from CartesianFixed to our
/// coordinate system.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* cf_inverse_transform() const 
  {return ogr_cf_inverse_transform_;}

  std::string pretty_wkt() const;
  std::string wkt() const;
  void print(std::ostream& Os) const;  
  std::string projected_cs_type_geo_key() const;
  std::string pcs_citation_geo_key() const;
  std::string geogcs_name() const;

//-----------------------------------------------------------------------
/// Return the NAIF code for the planet this coordinate is for.
//-----------------------------------------------------------------------

  int naif_code() const { return naif_code_; }
private:
  void init(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  void init(const std::string& Wkt);
  boost::shared_ptr<OGRSpatialReference> ogr_;
  OGRCoordinateTransformation* ogr_transform_;
  OGRCoordinateTransformation* ogr_inverse_transform_;
  OGRCoordinateTransformation* ogr_cf_transform_;
  OGRCoordinateTransformation* ogr_cf_inverse_transform_;
  int naif_code_;
  // Right now we have a fixed list of planets. If this becomes and
  // issue, we can come up with something more flexible.
  static boost::scoped_ptr<OGRSpatialReference> ogr_geodetic;
  static boost::scoped_ptr<OGRSpatialReference> ogr_ecr;
  static boost::scoped_ptr<OGRSpatialReference> ogr_mars_cf;
  OgrWrapper() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

/****************************************************************//**
  This is a ground coordinate, implemented by the OGRSpatialReference
  class supplied with GDAL. This class is particularly useful for
  working with GdalMapProjectedImage or VicarMapProjectedImage. The
  OGRSpatialReference can work with a number of ways of describing a
  coordinate system, including Well Known Text format (WKT) of the
  OpenGIS Transformation specification and the EPSG specification.
  Note in particular the EPSG specification is what is used by
  geotiff, the format used by the AFIDS system.

  You can see the documentation for OGRSpatialReference at
  http://www.gdal.org/ogr/osr_tutorial.html.
*******************************************************************/

class OgrCoordinate : public GroundCoordinate {
public:
//-----------------------------------------------------------------------
/// Constructor. X, Y, and Z are in whatever coordinates Ogr are in.
//-----------------------------------------------------------------------

  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		double x, double y, double z)
    : x(x), y(y), z(z), ogr_(Ogr)
  {}
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const Geodetic& G);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OgrCoordinate() {}

  virtual double latitude() const;
  virtual double longitude() const;
  virtual double height_reference_surface() const;
  virtual void lat_lon_height(double& Latitude, double& Longitude, 
			      double& Height_reference_surface) const; 

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Underlying OgrWrapper
//-----------------------------------------------------------------------

  const OgrWrapper& ogr() const { return *ogr_;}

//-----------------------------------------------------------------------
/// Underlying OgrWrapper
//-----------------------------------------------------------------------

  const boost::shared_ptr<OgrWrapper>& ogr_ptr() const 
  { return ogr_;}

//-----------------------------------------------------------------------
/// X coordinate, in coordinate system given by ogr.
//-----------------------------------------------------------------------

  double x;

//-----------------------------------------------------------------------
/// Y coordinate, in coordinate system given by ogr.
//-----------------------------------------------------------------------

  double y;

//-----------------------------------------------------------------------
/// Z coordinate, in coordinate system given by ogr.
//-----------------------------------------------------------------------

  double z;

  int utm_zone() const;

  static OgrCoordinate to_utm(const Geodetic& Gc, int zone = -999);
private:
  boost::shared_ptr<OgrWrapper> ogr_;
  OgrCoordinate() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a CoordinateConverter for working with OgrCoordinates.
*******************************************************************/

class OgrCoordinateConverter : public CoordinateConverter {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  OgrCoordinateConverter(const boost::shared_ptr<OgrWrapper> Ogr)
    : ogr_(Ogr) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OgrCoordinateConverter() {}

  virtual bool is_same(const CoordinateConverter& Conv) const;

//-----------------------------------------------------------------------
/// Underlying OgrWrapper
//-----------------------------------------------------------------------

  const OgrWrapper& ogr() const { return *ogr_;}

//-----------------------------------------------------------------------
/// Underlying OgrWrapper
//-----------------------------------------------------------------------

  const boost::shared_ptr<OgrWrapper>& ogr_ptr() const 
  { return ogr_;}

//-----------------------------------------------------------------------
/// Create a OgrCoordinate. X, Y, and Height are in whatever X, Y,
/// and Z are for the underlying OgrWrapper projection.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>(new 
		       OgrCoordinate(ogr_, X, Y, Height));
  }

//-----------------------------------------------------------------------
/// Convert to OgrCoordinate. X, Y and Height are in whatever X, Y and
/// Z are for the underlying OgrWrapper.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const
  {
    // Handle special case of GroundCoordinate already being in the
    // same coordinate system as ogr_. This is pretty common usage, so it is
    // worth checking.
    const OgrCoordinate* ogr_c = dynamic_cast<const OgrCoordinate*>(&Gc);
    if(ogr_c && ogr_c->ogr_ptr().get() == ogr_.get()) {
      X = ogr_c->x;
      Y = ogr_c->y;
      Height = ogr_c->z;
      return;
    }
    OgrCoordinate c(ogr_, Geodetic(Gc));
    X = c.x; 
    Y = c.y; 
    Height = c.z;
  }
  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Height) const
  {
    OgrCoordinate c(ogr_, Gc);
    X = c.x; 
    Y = c.y; 
    Height = c.z;
  }

  virtual void print(std::ostream& Os) const;
  static boost::shared_ptr<OgrCoordinateConverter>
  utm_converter(int Zone);
private:
  boost::shared_ptr<OgrWrapper> ogr_;
  OgrCoordinateConverter() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(OgrWrapper);
GEOCAL_EXPORT_KEY(OgrCoordinate);
GEOCAL_EXPORT_KEY(OgrCoordinateConverter);
#endif
