#include "ogr_coordinate.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Geodetic coordinate system, used as base for doing transformation.
//-----------------------------------------------------------------------

boost::scoped_ptr<OGRSpatialReference> OgrWrapper::ogr_geodetic;

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

OgrWrapper::OgrWrapper(const boost::shared_ptr<OGRSpatialReference>& Ogr)
  : ogr_(Ogr)
{
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
/// Convert to Geodetic coordinates.
//-----------------------------------------------------------------------

Geodetic OgrCoordinate::to_geodetic() const
{
  double xr = x, yr = y , zr = z;
  // Note that operation Transform really is const, this just doesn't
  // have the right signature in GDAL.
  int status = const_cast<OGRCoordinateTransformation&>(ogr_->transform()).
    Transform(1, &xr, &yr, &zr);
  if(!status)
    throw Exception("Call to OGR Transform failed");
  return Geodetic(yr, xr, zr);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void OgrCoordinate::print(std::ostream& Os) const
{
  Os << "OgrCoordinate: (" << x << ", " << y << ", " << z << ")";
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

