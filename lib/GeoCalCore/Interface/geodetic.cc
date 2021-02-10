#include "geocal_internal_config.h"
#include "geodetic.h"
#include "ecr.h"
#include "constant.h"
#include "wgs84_constant.h"
#include "geocal_serialize_support.h"
#include "ogr_coordinate.h"
#include <cmath>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Geodetic::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(Geodetic, GroundCoordinate);
  ar & GEOCAL_NVP2("latitude", lat_)
    & GEOCAL_NVP2("longitude", lon_)
    & GEOCAL_NVP_(height_ellipsoid);
}

template<class Archive>
void Geocentric::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(Geocentric, GroundCoordinate);
  ar & GEOCAL_NVP2("latitude", lat_)
    & GEOCAL_NVP2("longitude", lon_)
    & GEOCAL_NVP_(height_ellipsoid);
}

GEOCAL_IMPLEMENT(Geodetic);
GEOCAL_IMPLEMENT(Geocentric);
#endif

//-----------------------------------------------------------------------
/// Convert from GroundCoor.
//-----------------------------------------------------------------------

Geodetic::Geodetic(const GroundCoordinate& Gc)
{
  const OgrCoordinate* ogr = dynamic_cast<const OgrCoordinate*>(&Gc);
  if(ogr && ogr->ogr().naif_code() != Ecr::EARTH_NAIF_CODE) {
    Exception e;
    e << "Can't convert coordinate to Geodetic. Naif_code for the coordinate is " << ogr->ogr().naif_code();
    throw e;
  }
  Gc.lat_lon_height(lat_, lon_, height_ellipsoid_);
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> Geodetic::convert_to_cf() const
{
  const double f = (Constant::wgs84_a - Constant::wgs84_b) / Constant::wgs84_a;

#ifdef HAVE_SINCOS
  double slat, clat, slon, clon;
  sincos(Constant::deg_to_rad * latitude(), &slat, &clat);
  sincos(Constant::deg_to_rad * longitude(), &slon, &clon);
#else
  double slat = sin(Constant::deg_to_rad * latitude());
  double clat = cos(Constant::deg_to_rad * latitude());
  double slon = sin(Constant::deg_to_rad * longitude());
  double clon = cos(Constant::deg_to_rad * longitude());
#endif
  double r1 = 1.0 / sqrt(1 - (2*f - f * f) * slat * slat);
  double r2 = (Constant::wgs84_a * r1 + height_reference_surface()) * clat;
  boost::array<double, 3> pos;
  pos[0] = r2 * clon;
  pos[1] = r2 * slon;
  pos[2] = (Constant::wgs84_a * (1 - f) * (1 - f) * r1 + 
	    height_reference_surface()) * slat;
  return boost::shared_ptr<CartesianFixed>(new Ecr(pos));
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void Geodetic::print(std::ostream& Os) const
{
  Os << "Geodetic: (" << latitude() << " deg, " << longitude() << " deg, "
     << height_reference_surface() << " m)";
}

inline double sqr(double x) { return x * x; }

//-----------------------------------------------------------------------
/// Convert from GroundCoor.
//-----------------------------------------------------------------------

Geocentric::Geocentric(const GroundCoordinate& Gc)
{
  if(const Geocentric* g = dynamic_cast<const Geocentric*>(&Gc)) {
    *this = *g;
    return;
  }
  Ecr gc_ecr(Gc);
  lon_ = atan2(gc_ecr.position[1], gc_ecr.position[0]) * 
    Constant::rad_to_deg;
  lat_ = atan2(gc_ecr.position[2],
	       sqrt(sqr(gc_ecr.position[0]) +
		    sqr(gc_ecr.position[1]))) *
    Constant::rad_to_deg;
  height_ellipsoid_ =
    sqrt(sqr(gc_ecr.position[0]) + sqr(gc_ecr.position[1]) +
	 sqr(gc_ecr.position[2])) -
    earth_radius(lat_);
}

//-----------------------------------------------------------------------
/// Radius of Earth.
//-----------------------------------------------------------------------

double Geocentric::earth_radius(
double             Latitude     // Geocentric latitude, in degrees.
) const
{
  return Constant::wgs84_b / 
    sqrt(1 - Constant::wgs84_esq * sqr(cos(Latitude * Constant::deg_to_rad)));
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> Geocentric::convert_to_cf() const
{
  double r = earth_radius(lat_) + height_ellipsoid_;
  double lat = lat_ * Constant::deg_to_rad;
  double lon = lon_ * Constant::deg_to_rad;
  return boost::shared_ptr<CartesianFixed>(
    new Ecr(r * cos(lat) * cos(lon),
	    r * cos(lat) * sin(lon),
	    r * sin(lat)));
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void Geocentric::print(std::ostream& Os) const
{
  Os << "Geocentric: (" << latitude() << " deg, " << longitude() << " deg, "
     << height_reference_surface() << " m)";
}

