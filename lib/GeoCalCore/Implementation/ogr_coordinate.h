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
  class Planetocentric;
  
/****************************************************************//**
  This is a wrapper around the OGRSpatialReference class. We hold onto
  a OGRSpatialReference class and a transformation from that
  coordinate system to the Geodetic coordinate system. This class
  handles the lifetime of these objects. This is separated out from
  OgrCoordinate because typically we'll have lots of OgrCoordinates
  that have the same OgrWrapper.

  Note a complication in axis ordering. The WKT changed to include
  axis ordering, see https://trac.osgeo.org/gdal/wiki/rfc20_srs_axes
  and https://trac.osgeo.org/gdal/wiki/rfc73_proj6_wkt2_srsbarn
  (axis ordering section). This change took place in GDAL 3. All
  our code was written before this change, so in lots of places 
  we assume the old behavior. Our low level coordinate code doesn't
  actually care, but anything using this likely makes assumptions
  about what the order is. We also will need to support GDAL 2 for
  sometime in addition got GDAL 3.

  So we don't break a lot of existing code, we take a 
  "use_traditional_gis_order" flag. If this is true, we use the
  OAMS_TRADITIONAL_GIS_ORDER. false is an error if we are using gdal
  older than version 3 (since it isn't supported with gdal 2),
  otherwise we use OAMS_AUTHORITY_COMPLIANT. At some point we can just
  directly take the GDAL enumeration, but since this is in GDAL 3 only
  we instead take a boolean that can be use with 2.

  We don't directly support OAMS_CUSTOM, although you can pass a 
  OGRSpatialReference directly with that. However, serialization isn't
  currently supported for OAMS_CUSTOM. We could add that if needed, we
  just need to think through how to save that. For now, I don't see
  much of a need for that.
*******************************************************************/

class OgrWrapper : public boost::noncopyable,
	           public Printable<OgrWrapper>  {
public:
  OgrWrapper(const std::string& Wkt, bool Use_traditional_gis_order = true);
  OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  static boost::shared_ptr<OgrWrapper> from_epsg(int Epsg_id,
	 bool Use_traditional_gis_order = true);
  static boost::shared_ptr<OgrWrapper> 
  from_proj4(const std::string& Proj4_string,
	     bool Use_traditional_gis_order = true);
  static void add_spatial_reference(const std::string& Name,
				    int Naif_code,
				    boost::shared_ptr<OGRSpatialReference>& Geodetic_or_planetocentric,
				    boost::shared_ptr<OGRSpatialReference>& Cf);
  static void add_spatial_reference(const std::string& Name,
				    int Naif_code,
				    const std::string& Wkt_planet_centric);
  virtual ~OgrWrapper();

//-----------------------------------------------------------------------
/// Return underlying OGRSpatialReference.
//-----------------------------------------------------------------------

  const OGRSpatialReference& ogr() const { return *ogr_;}
  const boost::shared_ptr<OGRSpatialReference>& ogr_ptr() const { return ogr_;}

//-----------------------------------------------------------------------
/// Return transformation that takes us from our coordinate system to
/// Geodetic/Planetocentric.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* transform() const {return ogr_transform_;}

//-----------------------------------------------------------------------
/// Return inverse of transform(). This goes from
/// Geodetic/Planetocentric to our coordinate system. 
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* inverse_transform() const 
  {return ogr_inverse_transform_;}

//-----------------------------------------------------------------------
/// Return transformation that takes us from our coordinate system to
/// CartesianFixed. Is null for other planets.
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* cf_transform() const
  {return ogr_cf_transform_;}

//-----------------------------------------------------------------------
/// Return inverse of cf_transform(). This goes from CartesianFixed to our
/// coordinate system. Is null for other planets
//-----------------------------------------------------------------------

  const OGRCoordinateTransformation* cf_inverse_transform() const 
  {return ogr_cf_inverse_transform_;}

  std::string pretty_wkt() const;
  std::string wkt() const;
  void print(std::ostream& Os) const;  
  std::string projected_cs_type_geo_key() const;
  std::string pcs_citation_geo_key() const;
  std::string geogcs_name() const;
  std::string geoccs_name() const;
  bool use_traditional_gis_order() const;

//-----------------------------------------------------------------------
/// Return the NAIF code for the planet this coordinate is for.
//-----------------------------------------------------------------------

  int naif_code() const { return naif_code_; }
protected:
  OgrWrapper() {}
private:
  void init(const boost::shared_ptr<OGRSpatialReference>& Ogr);
  void init(const std::string& Wkt, bool use_traditional_gis_order);
  static void handle_gis_order(OGRSpatialReference& Ogr,
			bool Use_traditional_gis_order);
  boost::shared_ptr<OGRSpatialReference> ogr_;
  OGRCoordinateTransformation* ogr_transform_;
  OGRCoordinateTransformation* ogr_inverse_transform_;
  OGRCoordinateTransformation* ogr_cf_transform_;
  OGRCoordinateTransformation* ogr_cf_inverse_transform_;
  int naif_code_;
  static void init_naif();
  static bool init_naif_done;
  static std::map<std::string, int> name_to_naif;
  static std::map<int, boost::shared_ptr<OGRSpatialReference> > naif_to_g_or_pc;
  static std::map<int, boost::shared_ptr<OGRSpatialReference> > naif_to_cf;
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

  See note in OgrWrapper about axis order.
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
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const Planetocentric& G);
  OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
		const GroundCoordinate& G);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OgrCoordinate() {}

  virtual double latitude() const;
  virtual double longitude() const;
  virtual double height_reference_surface() const;
  virtual void lat_lon_height(double& Latitude, double& Longitude, 
			      double& Height_reference_surface) const;
  std::string to_proj4() const;

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

  virtual int naif_code() const { return ogr_->naif_code(); }
  static OgrCoordinate to_utm(const Geodetic& Gc, int zone = -999);
protected:
  OgrCoordinate() {}
private:
  boost::shared_ptr<OgrWrapper> ogr_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a CoordinateConverter for working with OgrCoordinates.

  See note in OgrWrap about axis order.
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
    OgrCoordinate c(ogr_, Gc);
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
protected:
  OgrCoordinateConverter() {}
private:
  boost::shared_ptr<OgrWrapper> ogr_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(OgrWrapper);
GEOCAL_CLASS_VERSION(OgrWrapper, 1);
GEOCAL_EXPORT_KEY(OgrCoordinate);
GEOCAL_EXPORT_KEY(OgrCoordinateConverter);
#endif
