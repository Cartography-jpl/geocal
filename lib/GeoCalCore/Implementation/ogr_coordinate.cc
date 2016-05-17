#include "ogr_coordinate.h"
#include "geocal_serialize_support.h"

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
    ogr_geodetic->SetWellKnownGeogCS("WGS84");
  }
  ogr_transform_ = OGRCreateCoordinateTransformation(ogr_.get(), 
						     ogr_geodetic.get());
  if(!ogr_transform_)
    throw Exception("Couldn't create transformation to geodetic coordinate system");
  ogr_inverse_transform_ = OGRCreateCoordinateTransformation(ogr_geodetic.get(),
							     ogr_.get());
  if(!ogr_inverse_transform_) {
    delete ogr_transform_;
    throw Exception("Couldn't create transformation from geodetic coordinate system");
  }
}

//-----------------------------------------------------------------------
/// Convert from Geodetic to the coordinate system given by Ogr.
//-----------------------------------------------------------------------

OgrCoordinate::OgrCoordinate(const boost::shared_ptr<OgrWrapper>& Ogr,
			     const Geodetic& G)
  : ogr_(Ogr)
{
  x = G.longitude(); 
  y = G.latitude();
  z = G.height_reference_surface();
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = 
    const_cast<OGRCoordinateTransformation&>(ogr_->inverse_transform()).
    Transform(1, &x, &y, &z);
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
  delete ogr_transform_;
  delete ogr_inverse_transform_;
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
  return ogr_->GetAttrValue("PROJCS");
}

//-----------------------------------------------------------------------
/// Return latitude in degrees. Latitude is -90 to 90.
//-----------------------------------------------------------------------

double OgrCoordinate::latitude() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = const_cast<OGRCoordinateTransformation&>(ogr_->transform()).
    Transform(1, &xr, &yr, &zr);
  if(!status)
    throw Exception("Call to OGR Transform failed");
  return yr;
}

//-----------------------------------------------------------------------
/// Return longitude in degrees. Longitude is -180 to 180.
//-----------------------------------------------------------------------

double OgrCoordinate::longitude() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = const_cast<OGRCoordinateTransformation&>(ogr_->transform()).
    Transform(1, &xr, &yr, &zr);
  if(!status)
    throw Exception("Call to OGR Transform failed");
  return xr;
}

//-----------------------------------------------------------------------
/// Height above ellipsoid, in meters.
//-----------------------------------------------------------------------

double OgrCoordinate::height_reference_surface() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = const_cast<OGRCoordinateTransformation&>(ogr_->transform()).
    Transform(1, &xr, &yr, &zr);
  if(!status)
    throw Exception("Call to OGR Transform failed");
  return zr;
}

void OgrCoordinate::lat_lon_height(double& Latitude, double& Longitude, 
				   double& Height_reference_surface) const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = const_cast<OGRCoordinateTransformation&>(ogr_->transform()).
    Transform(1, &xr, &yr, &zr);
  if(!status)
    throw Exception("Call to OGR Transform failed");
  Latitude = yr;
  Longitude = xr;
  Height_reference_surface = zr;
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
