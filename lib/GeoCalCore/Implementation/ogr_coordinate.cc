#include "ogr_coordinate.h"
#include "planet_coordinate.h"
#include "geocal_serialize_support.h"
#include "ecr.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OgrWrapper::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(OgrWrapper);
  std::string wkt_s = wkt();
  ar & GEOCAL_NVP2("wkt", wkt_s);
}

template<class Archive>
void OgrWrapper::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(OgrWrapper);
  std::string wkt_s;
  ar & GEOCAL_NVP2("wkt", wkt_s);
  init(wkt_s);
}

template<class Archive>
void OgrCoordinate::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(OgrCoordinate, GroundCoordinate);
  ar & GEOCAL_NVP(x) & GEOCAL_NVP(y) & GEOCAL_NVP(z) & GEOCAL_NVP_(ogr);
}

template<class Archive>
void OgrCoordinateConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(OgrCoordinateConverter, CoordinateConverter);
  ar & GEOCAL_NVP_(ogr);
}

GEOCAL_IMPLEMENT(OgrCoordinate);
GEOCAL_IMPLEMENT(OgrCoordinateConverter);
GEOCAL_SPLIT_IMPLEMENT(OgrWrapper);
#endif

//-----------------------------------------------------------------------
/// Geodetic coordinate system, used as base for doing transformation.
//-----------------------------------------------------------------------

boost::scoped_ptr<OGRSpatialReference> OgrWrapper::ogr_geodetic;
boost::scoped_ptr<OGRSpatialReference> OgrWrapper::ogr_ecr;
boost::scoped_ptr<OGRSpatialReference> OgrWrapper::ogr_mars_pc;
boost::scoped_ptr<OGRSpatialReference> OgrWrapper::ogr_ceres_pc;

//-----------------------------------------------------------------------
/// Constructor that creates a OGRSpatialReference from a WKT (Well
/// Known Text) string.
//-----------------------------------------------------------------------

OgrWrapper::OgrWrapper(const std::string& Wkt)
{
  init(Wkt);
}

//-----------------------------------------------------------------------
/// Initialize, given a WKT (Well Known Text) string.
//-----------------------------------------------------------------------

void OgrWrapper::init(const std::string& Wkt)
{
  boost::shared_ptr<OGRSpatialReference> ogr_create(new OGRSpatialReference);
  char* wkt_str = const_cast<char*>(Wkt.c_str());
  OGRErr status = ogr_create->importFromWkt(&wkt_str);
  if(status != OGRERR_NONE) {
    Exception e;
    e << "Create of OGRSpatialReference failed. "
      << "The WKT (Well Known Text) was:\n"
      << Wkt;
    throw e;
  }
  init(ogr_create);
}

//-----------------------------------------------------------------------
/// Constructor, from an existing OGRSpatialReference
//-----------------------------------------------------------------------

OgrWrapper::OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr)
{
  init(Ogr);
}

//-----------------------------------------------------------------------
/// Initialize, given a OGRSpatialReference
//-----------------------------------------------------------------------

void OgrWrapper::init(const boost::shared_ptr<OGRSpatialReference>& Ogr)
{
  ogr_ = Ogr;
  if(!ogr_geodetic.get()) {
    ogr_geodetic.reset(new OGRSpatialReference);
    OGRErr status = ogr_geodetic->SetWellKnownGeogCS("WGS84");
    if(status != OGRERR_NONE)
      throw Exception("Call to SetWellKnownGeogCS failed");
  }
  if(!ogr_ecr.get()) {
    // Look this up http://spatialreference.org/ref/epsg/4328/
    ogr_ecr.reset(new OGRSpatialReference);
    OGRErr status = ogr_ecr->importFromEPSG(4328);
    if(status != OGRERR_NONE)
      throw Exception("Call to importFromEPSG failed");
  }
  if(!ogr_mars_pc.get()) {
    // Look this up http://spatialreference.org/ref/iau2000/49900/
    ogr_mars_pc.reset(new OGRSpatialReference);
    const char *wkt = "GEOGCS[\"Mars 2000\",\
    DATUM[\"D_Mars_2000\",\
        SPHEROID[\"Mars_2000_IAU_IAG\",3396190.0,169.89444722361179]],\
    PRIMEM[\"Greenwich\",0],\
    UNIT[\"Decimal_Degree\",0.0174532925199433]]";
    char * wkt_str = const_cast<char*>(wkt);
    OGRErr status = ogr_mars_pc->importFromWkt(&wkt_str);
    if(status != OGRERR_NONE) {
      Exception e;
      e << "Create of OGRSpatialReference failed. "
	<< "The WKT (Well Known Text) was:\n"
	<< wkt;
      throw e;
    }
  }
  if(!ogr_ceres_pc.get()) {
    ogr_ceres_pc.reset(new OGRSpatialReference);
    const char *wkt = "GEOGCS[\"GCS_CERES\",\
    DATUM[\"D_CERES\",\
        SPHEROID[\"CERES\",487300,0.0668992407141]],\
    PRIMEM[\"Reference_Meridian\",0],\
    UNIT[\"Decimal_Degree\",0.0174532925199433]]";
    char * wkt_str = const_cast<char*>(wkt);
    OGRErr status = ogr_ceres_pc->importFromWkt(&wkt_str);
    if(status != OGRERR_NONE) {
      Exception e;
      e << "Create of OGRSpatialReference failed. "
	<< "The WKT (Well Known Text) was:\n"
	<< wkt;
      throw e;
    }
  }
  OGRSpatialReference * og;
  OGRSpatialReference * og_cf;
  std::string gname = geogcs_name();
  if(gname == "")
    gname = geoccs_name();
  boost::algorithm::to_lower(gname);
  // We may need a more sophisticated way of handling this, but for
  // now just search for "mars" in the GCS name
  if(gname.find("mars") != std::string::npos) {
    naif_code_ = PlanetConstant::MARS_NAIF_CODE;
    og =  ogr_mars_pc.get();
    // Note we use to have a mars cartesian fixed conversion. However,
    // GDAL (version 2.0.2) seems to have trouble converting correctly
    // we have a coordinate system with a spherical mars model. See
    // Issue #19 in github for details on this.
    og_cf = 0;
  } else if(gname.find("ceres") != std::string::npos) {
    naif_code_ = PlanetConstant::CERES_NAIF_CODE;
    og =  ogr_ceres_pc.get();
    og_cf = 0;
  } else {
    naif_code_ = Ecr::EARTH_NAIF_CODE;
    og = ogr_geodetic.get();
    og_cf = ogr_ecr.get();
  }
  ogr_transform_ = 0;
  ogr_inverse_transform_ = 0;
  ogr_cf_transform_ = 0;
  ogr_cf_inverse_transform_ = 0;

  if(og) {
    ogr_transform_ = OGRCreateCoordinateTransformation(ogr_.get(), og);
    if(!ogr_transform_) {
      delete ogr_transform_;
      delete ogr_inverse_transform_;
      delete ogr_cf_transform_;
      delete ogr_cf_inverse_transform_;
      throw Exception("Couldn't create transformation to geodetic coordinate system");
    }
    ogr_inverse_transform_ = OGRCreateCoordinateTransformation(og, ogr_.get());
    if(!ogr_inverse_transform_) {
      delete ogr_transform_;
      delete ogr_inverse_transform_;
      delete ogr_cf_transform_;
      delete ogr_cf_inverse_transform_;
      throw Exception("Couldn't create transformation from geodetic coordinate system");
    }
  }
  if(og_cf) {
    ogr_cf_transform_ = OGRCreateCoordinateTransformation(ogr_.get(), og_cf);
    if(!ogr_cf_transform_) {
      delete ogr_transform_;
      delete ogr_inverse_transform_;
      delete ogr_cf_transform_;
      delete ogr_cf_inverse_transform_;
      throw Exception("Couldn't create transformation to cartesian fixed coordinate system");
    }
    ogr_cf_inverse_transform_ = OGRCreateCoordinateTransformation(og_cf, ogr_.get());
    if(!ogr_cf_inverse_transform_) {
      delete ogr_transform_;
      delete ogr_inverse_transform_;
      delete ogr_cf_transform_;
      delete ogr_cf_inverse_transform_;
      throw Exception("Couldn't create transformation to cartesian fixed coordinate system");
    }
  }
}

//-----------------------------------------------------------------------
/// Convert from Geodetic to the coordinate system given by Ogr.
//-----------------------------------------------------------------------

OgrCoordinate::OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
			     const Geodetic& G)
  : ogr_(Ogr)
{
  if(ogr_->naif_code() != Ecr::EARTH_NAIF_CODE)
    throw Exception("Can't convert from Geodetic");
  x = G.longitude(); 
  y = G.latitude();
  z = G.height_reference_surface();
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->inverse_transform()) {
    int status = 
      const_cast<OGRCoordinateTransformation&>(*ogr_->inverse_transform()).
      Transform(1, &x, &y, &z);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    return;
  }
  throw Exception("Not implemented yet");
}

//-----------------------------------------------------------------------
/// Convert from Planetocentric to the coordinate system given by Ogr.
//-----------------------------------------------------------------------

OgrCoordinate::OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
			     const Planetocentric& G)
  : ogr_(Ogr)
{
  if(ogr_->naif_code() != G.naif_code())
    throw Exception("Can't convert from Planetocentric");
  x = G.longitude(); 
  y = G.latitude();
  z = G.height_reference_surface();
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->inverse_transform()) {
    int status = 
      const_cast<OGRCoordinateTransformation&>(*ogr_->inverse_transform()).
      Transform(1, &x, &y, &z);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    return;
  }
  throw Exception("Not implemented yet");
}

//-----------------------------------------------------------------------
/// Convert from GroundCoordinate to the coordinate system given by Ogr.
//-----------------------------------------------------------------------

OgrCoordinate::OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
			     const GroundCoordinate& G)
  : ogr_(Ogr)
{
  if(ogr_->cf_transform()) {
    boost::shared_ptr<CartesianFixed> cf = G.convert_to_cf();
    if(ogr_->naif_code() != cf->naif_code()) {
      Exception e;
      e << "Inconsistent NAIF codes.\n"
	<< "  G:    " << cf->naif_code() << "\n"
	<< "  ogr_: " << ogr_->naif_code();
      throw e;
    }
    x = cf->position[0];
    y = cf->position[1];
    z = cf->position[2];
    int status = 
      const_cast<OGRCoordinateTransformation&>(*ogr_->cf_inverse_transform()).
      Transform(1, &x, &y, &z);
    if(!status)
      throw Exception("Call to OGR Transform failed");
  }
  // y really is first, latitude is y
  G.lat_lon_height(y, x, z);
  int status = const_cast<OGRCoordinateTransformation&>
    (*ogr_->inverse_transform()).Transform(1, &x, &y, &z);
  if(!status)
    throw Exception("Call to OGR Transform failed");
}

//-----------------------------------------------------------------------
/// Create a OgrWrapper for a coordinate system given by the EPSG ID.
/// You can look the EPSG code up for various coordinate systems at
/// http://www.epsg-registry.org
//-----------------------------------------------------------------------

boost::shared_ptr<OgrWrapper> OgrWrapper::from_epsg(int Epsg_id)
{
  boost::shared_ptr<OGRSpatialReference> ogr(new OGRSpatialReference);
  ogr->importFromEPSG(Epsg_id);
  return boost::shared_ptr<OgrWrapper>(new OgrWrapper(ogr));
}

//-----------------------------------------------------------------------
/// Create a OgrWrapper for a coordinate system from a Proj 4 string.
//-----------------------------------------------------------------------

boost::shared_ptr<OgrWrapper> OgrWrapper::from_proj4(const std::string& Proj4_string)
{
  boost::shared_ptr<OGRSpatialReference> ogr(new OGRSpatialReference);
  ogr->importFromProj4(Proj4_string.c_str());
  return boost::shared_ptr<OgrWrapper>(new OgrWrapper(ogr));
}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

OgrWrapper::~OgrWrapper()
{
  // Note, it is fine of pointers are null, C++ allows deleting null pointers.
  delete ogr_transform_;
  delete ogr_inverse_transform_;
  delete ogr_cf_transform_;
  delete ogr_cf_inverse_transform_;
}

//-----------------------------------------------------------------------
/// Write out a prettified version of the WKT for ogr_.
//-----------------------------------------------------------------------

std::string OgrWrapper::pretty_wkt() const
{
  char *res;
  ogr_->exportToPrettyWkt(&res);
  std::string ress(res);
  OGRFree(res);
  return ress;
}

//-----------------------------------------------------------------------
/// Write out a the WKT (Well Known Text) for ogr_.
//-----------------------------------------------------------------------

std::string OgrWrapper::wkt() const
{
  char *res;
  ogr_->exportToWkt(&res);
  std::string ress(res);
  OGRFree(res);
  return ress;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void OgrWrapper::print(std::ostream& Os) const
{
  Os << "OgrWrapper:\n"
     << pretty_wkt() << "\n";
}

//-----------------------------------------------------------------------
/// When converting to the GEOTIFF header format used by VICAR, this 
/// is the value that the PROJECTEDCSTYPEGEOKEY should have. This is
/// just the EPSG id written as text.
//-----------------------------------------------------------------------

std::string OgrWrapper::projected_cs_type_geo_key() const
{
  if(!ogr_->GetAuthorityName("PROJCS") ||
     ogr_->GetAuthorityName("PROJCS") != std::string("EPSG"))
    throw Exception("Right now only support projections that are EPSG.");
  return ogr_->GetAuthorityCode("PROJCS");
}

//-----------------------------------------------------------------------
/// When converting to the GEOTIFF header format used by VICAR, this 
/// is the value that the PCSCitationGeoKey should have. This is
/// a short text description of the projection.
//-----------------------------------------------------------------------

std::string OgrWrapper::pcs_citation_geo_key() const
{
  const char* t = ogr_->GetAttrValue("PROJCS");
  if(t)
    return t;
  else
    return "";
}

//-----------------------------------------------------------------------
/// The name of the GEOGCS.
//-----------------------------------------------------------------------

std::string OgrWrapper::geogcs_name() const
{
  const char* t =ogr_->GetAttrValue("GEOGCS");
  if(t)
    return t;
  else
    return "";
}

//-----------------------------------------------------------------------
/// The name of the GEOCCS.
//-----------------------------------------------------------------------

std::string OgrWrapper::geoccs_name() const
{
  const char* t =ogr_->GetAttrValue("GEOCCS");
  if(t)
    return t;
  else
    return "";
}

//-----------------------------------------------------------------------
/// Return latitude in degrees. Latitude is -90 to 90.
///
/// Note that for the earth, the latitude is the standard geodetic
/// latitude. However, by convention other planets use Planetocentric
/// latitude, the equivalent of geocentric latitude for the other
/// planet. This is handled transparently and consistently by the
/// various coordinate classes (e.g., PlanetConstant, OgrCoordinate),
/// but you should be aware of this difference.
//-----------------------------------------------------------------------

double OgrCoordinate::latitude() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->transform()) {
    int status = const_cast<OGRCoordinateTransformation&>(*ogr_->transform()).
      Transform(1, &xr, &yr, &zr);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    return yr;
  }
  // Might be able to do this more efficiently, but for now just go
  // through CartesianFixed coordinates.
  boost::shared_ptr<CartesianFixed> cf = convert_to_cf();
  return cf->latitude();
}

//-----------------------------------------------------------------------
/// Return longitude in degrees. Longitude is -180 to 180.
//-----------------------------------------------------------------------

double OgrCoordinate::longitude() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->transform()) {
    int status = const_cast<OGRCoordinateTransformation&>(*ogr_->transform()).
      Transform(1, &xr, &yr, &zr);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    return xr;
  }
  // Might be able to do this more efficiently, but for now just go
  // through CartesianFixed coordinates.
  boost::shared_ptr<CartesianFixed> cf = convert_to_cf();
  return cf->longitude();
}

//-----------------------------------------------------------------------
/// Height above ellipsoid, in meters.
//-----------------------------------------------------------------------

double OgrCoordinate::height_reference_surface() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->transform()) {
    int status = const_cast<OGRCoordinateTransformation&>(*ogr_->transform()).
      Transform(1, &xr, &yr, &zr);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    return zr;
  }
  // Might be able to do this more efficiently, but for now just go
  // through CartesianFixed coordinates.
  boost::shared_ptr<CartesianFixed> cf = convert_to_cf();
  return cf->height_reference_surface();
}

void OgrCoordinate::lat_lon_height(double& Latitude, double& Longitude, 
				   double& Height_reference_surface) const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  if(ogr_->transform()) {
    int status = const_cast<OGRCoordinateTransformation&>(*ogr_->transform()).
      Transform(1, &xr, &yr, &zr);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    Latitude = yr;
    Longitude = xr;
    Height_reference_surface = zr;
    return;
  }
  // Might be able to do this more efficiently, but for now just go
  // through CartesianFixed coordinates.
  boost::shared_ptr<CartesianFixed> cf = convert_to_cf();
  Latitude = cf->latitude();
  Longitude = cf->longitude();
  Height_reference_surface = cf->height_reference_surface();
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> OgrCoordinate::convert_to_cf() const
{
  double xr = x, yr = y , zr = z;
  if(ogr_->cf_transform()) {
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
    int status = const_cast<OGRCoordinateTransformation&>(*ogr_->cf_transform()).
      Transform(1, &xr, &yr, &zr);
    if(!status)
      throw Exception("Call to OGR Transform failed");
    if(naif_code() == Ecr::EARTH_NAIF_CODE)
      return boost::make_shared<Ecr>(xr, yr, zr);
    return boost::make_shared<PlanetFixed>(xr, yr, zr, naif_code());
  }
  double lat, lon, h;
  lat_lon_height(lat, lon, h);
  return boost::make_shared<PlanetFixed>(Planetocentric(lat, lon, h,
							naif_code()));
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void OgrCoordinate::print(std::ostream& Os) const
{
  Os << "OgrCoordinate: " << ogr_->pcs_citation_geo_key() << " : (" << x << ", " << y << ", " << z << ")";
}

//-----------------------------------------------------------------------
/// Return the UTM zone number for the coordinate (positive for North,
/// negative for South). If this isn't actually in UTM, then this just
/// returns 0.
//-----------------------------------------------------------------------

int OgrCoordinate::utm_zone() const
{
  int is_north;
  int zone = ogr_->ogr().GetUTMZone(&is_north);
  if(!is_north)
    zone = -zone;
  return zone;
}

//-----------------------------------------------------------------------
/// Convert a GroundCoordinate to UTM. If desired, you can pass in the
/// zone number (use negative number for southern zones), or if left
/// blank we determine the appropriate zone to use.
//-----------------------------------------------------------------------
  
OgrCoordinate OgrCoordinate::to_utm(const Geodetic& Gc, int zone)
{
  if(zone == -999) {
    zone = (int) floor((Gc.longitude() + 180.0) / 6.0 + 1.0);
    if(Gc.latitude() < 0)
      zone = -zone;
  }
  boost::shared_ptr<OGRSpatialReference> ogr(new OGRSpatialReference);
  ogr->SetWellKnownGeogCS("WGS84");
  ogr->SetUTM((zone > 0 ? zone  : -zone), (zone > 0));
  boost::shared_ptr<OgrWrapper> ogrw(new OgrWrapper(ogr));
  return OgrCoordinate(ogrw, Gc);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void OgrCoordinateConverter::print(std::ostream& Os) const
{
  Os << "OgrCoordinateConverter:\n"
     << ogr_->pretty_wkt() << "\n";
}

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

bool OgrCoordinateConverter::is_same(const CoordinateConverter& Conv) const
{
  const OgrCoordinateConverter* c = 
    dynamic_cast<const OgrCoordinateConverter*>(&Conv);
  if(!c)
    return false;
  return ogr().ogr().IsSame(&c->ogr().ogr());
}

//-----------------------------------------------------------------------
/// Create a converter for UTM. The zone number should be positive for
/// north, negative for south.
//-----------------------------------------------------------------------

boost::shared_ptr<OgrCoordinateConverter>
OgrCoordinateConverter::utm_converter(int Zone)
{
  boost::shared_ptr<OGRSpatialReference> ogr(new OGRSpatialReference);
  ogr->SetWellKnownGeogCS("WGS84");
  ogr->SetUTM((Zone > 0 ? Zone  : -Zone), (Zone > 0));
  boost::shared_ptr<OgrWrapper> ogrw(new OgrWrapper(ogr));
  return boost::shared_ptr<OgrCoordinateConverter>
    (new OgrCoordinateConverter(ogrw));
}
