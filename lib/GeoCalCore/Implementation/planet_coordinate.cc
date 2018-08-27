#include "planet_coordinate.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PlanetFixed::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianFixed)
    & GEOCAL_NVP_(naif_code);
}

template<class Archive>
void PlanetInertial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianInertial)
    & GEOCAL_NVP_(naif_code);
}

template<class Archive>
void Planetocentric::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(Planetocentric, GroundCoordinate);
  ar & GEOCAL_NVP2("latitude", lat_)
    & GEOCAL_NVP2("longitude", lon_)
    & GEOCAL_NVP_(height_ellipsoid)
    & GEOCAL_NVP_(naif_code);
}

template<class Archive>
void PlanetocentricConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(PlanetocentricConverter, CoordinateConverter);
  ar & GEOCAL_NVP_(naif_code);
}

template<class Archive>
void PlanetSimpleDem::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(PlanetSimpleDem, Dem);
  ar & GEOCAL_NVP_(h)
    & GEOCAL_NVP_(naif_code);
}

GEOCAL_IMPLEMENT(PlanetFixed);
GEOCAL_IMPLEMENT(PlanetInertial);
GEOCAL_IMPLEMENT(Planetocentric);
GEOCAL_IMPLEMENT(PlanetocentricConverter);
GEOCAL_IMPLEMENT(PlanetSimpleDem);
#endif

//-----------------------------------------------------------------------
/// Map from NAIF code to planet constants
//-----------------------------------------------------------------------

std::map<int, SpicePlanetConstant> PlanetConstant::h_map;

//-----------------------------------------------------------------------
/// Get the planet constants for the given NAIF code.
//-----------------------------------------------------------------------

const SpicePlanetConstant& PlanetConstant::h(int Naif_code)
{
  if(h_map.count(Naif_code) == 0)
    h_map[Naif_code] = SpicePlanetConstant(Naif_code);
  return h_map[Naif_code];
}

//-----------------------------------------------------------------------
/// Create PlanetFixed from GroundCoordinate
//-----------------------------------------------------------------------

PlanetFixed::PlanetFixed(const GroundCoordinate& Gc)
    : naif_code_(Gc.naif_code())
{
  if(const PlanetFixed* g = 
     dynamic_cast<const PlanetFixed*>(&Gc)) {
    *this = *g;
    return;
  }
  boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
  if(cf->naif_code() != naif_code_) {
    Exception e;
    e << "Cannot convert ground coordinate to "
      << PlanetConstant::name(naif_code_)<< "Fixed\n"
      << "Coordinate: " << Gc << "\n";
    throw e;
  }
  position = cf->position;
}

//-----------------------------------------------------------------------
/// Make an PlanetFixed with the given position, in meters.
//-----------------------------------------------------------------------

PlanetFixed::PlanetFixed(double X, double Y, double Z, int Naif_code)
  : naif_code_(Naif_code)
{
  position[0] = X;
  position[1] = Y;
  position[2] = Z;
}

boost::shared_ptr<CartesianFixed>
PlanetFixed::reference_surface_intersect_approximate
(const CartesianFixedLookVector& Cl, double Height_reference_surface)
  const
{
  double aph = PlanetConstant::a(naif_code_) + 
    Height_reference_surface;
  double bph = PlanetConstant::b(naif_code_) + 
    Height_reference_surface;
  boost::array<double, 3> dirci;
  dirci[0] = Cl.look_vector[0]/ aph;
  dirci[1] = Cl.look_vector[1]/ aph;
  dirci[2] = Cl.look_vector[2]/ bph;
  double t = norm(dirci);
  dirci[0] /= t;
  dirci[1] /= t;
  dirci[2] /= t;
  boost::array<double, 3> pci;
  pci[0] = position[0] / aph;
  pci[1] = position[1] / aph;
  pci[2] = position[2] / bph;
  double ddotp = dot(dirci, pci);
  double dl = -ddotp - sqrt(ddotp * ddotp + (1 - dot(pci, pci)));
  boost::array<double, 3> res;
  res[0] = (pci[0] + dirci[0] * dl) * aph;
  res[1] = (pci[1] + dirci[1] * dl) * aph;
  res[2] = (pci[2] + dirci[2] * dl) * bph;
  return create(res);
}

void PlanetFixed::print(std::ostream& Os) const
{
  Os << PlanetConstant::name(naif_code_)
     << " Fixed (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << "m)";
}

//-----------------------------------------------------------------------
/// Use spice to determine the position of the given body at the given
/// time.
//-----------------------------------------------------------------------

PlanetFixed PlanetFixed::target_position
(const std::string& Target_name, const Time& T, int Naif_code)
{
  PlanetFixed res(Naif_code);
  boost::array<double, 3> vel;
  SpiceHelper::state_vector(Naif_code, Target_name, T, res.position, vel);
  return res;
}

//-----------------------------------------------------------------------
/// Return orbit data for the given target and spacecraft reference
/// frame. The target is the name of the satellite used by spice (e.g,
/// "GLL", "MRO"), and the reference frame is the name of the camera
/// reference frame (e.g., "GLL_SCAN_PLANE", "MRO_CTX"). Note that
/// this combined both the space craft coordinate system and
/// conversion to frame coordinates. We could split this out into a
/// separate camera model, but there doesn't seem to be much of a
/// reason to do this.
///
/// Note that the Target_name is of a body (e.g., something we have a
/// spk kernel for), while the Spacecraft_reference_frame_name is for
/// a frame (e.g., something we have a fk kernel for). In addition to
/// the frame definition, you'll generally need a C kernel file (ck
/// kernel) giving the orientation of the frame with the target.
//-----------------------------------------------------------------------

boost::shared_ptr<QuaternionOrbitData> PlanetFixed::orbit_data
(const std::string& Target_name, 
 const std::string& Spacecraft_reference_frame_name, const Time& T,
 int Naif_code)
{
  boost::shared_ptr<PlanetFixed> pos(new PlanetFixed(Naif_code));
  boost::array<double, 3> vel;
  SpiceHelper::state_vector(Naif_code, Target_name, T, pos->position, vel);
  return boost::shared_ptr<QuaternionOrbitData>
    (new QuaternionOrbitData(T, pos, vel, 
     SpiceHelper::conversion_quaternion(Spacecraft_reference_frame_name, 
     SpiceHelper::fixed_frame_name(Naif_code), T)));
}

//-----------------------------------------------------------------------
/// Make an PlanetInertial with the given position, in meters.
//-----------------------------------------------------------------------

PlanetInertial::PlanetInertial(double X, double Y, double Z, int Naif_code)
  : naif_code_(Naif_code)
{
  position[0] = X;
  position[1] = Y;
  position[2] = Z;
}

boost::shared_ptr<CartesianFixed> PlanetInertial::convert_to_cf(const Time& T) 
  const
{
  boost::shared_ptr<PlanetFixed> res(new PlanetFixed(naif_code_));
  CartesianFixed::toolkit_coordinate_interface->
    to_fixed(naif_code_, T, *this, *res);
  return res;
}

boost::shared_ptr<CartesianInertial>
PlanetInertial::reference_surface_intersect_approximate
(const CartesianInertialLookVector& Cl, double Height_reference_surface) 
  const
{
  double aph = PlanetConstant::a(naif_code_) + 
    Height_reference_surface;
  double bph = PlanetConstant::b(naif_code_) + 
    Height_reference_surface;
  boost::array<double, 3> dirci;
  dirci[0] = Cl.look_vector[0]/ aph;
  dirci[1] = Cl.look_vector[1]/ aph;
  dirci[2] = Cl.look_vector[2]/ bph;
  double t = norm(dirci);
  dirci[0] /= t;
  dirci[1] /= t;
  dirci[2] /= t;
  boost::array<double, 3> pci;
  pci[0] = position[0] / aph;
  pci[1] = position[1] / aph;
  pci[2] = position[2] / bph;
  double ddotp = dot(dirci, pci);
  double dl = -ddotp - sqrt(ddotp * ddotp + (1 - dot(pci, pci)));
  boost::array<double, 3> res;
  res[0] = (pci[0] + dirci[0] * dl) * aph;
  res[1] = (pci[1] + dirci[1] * dl) * aph;
  res[2] = (pci[2] + dirci[2] * dl) * bph;
  return create(res);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void PlanetInertial::print(std::ostream& Os) const
{
  Os << PlanetConstant::name(naif_code_)
     << " Inertial (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << "m)";
}

//-----------------------------------------------------------------------
/// Convert from GroundCoordinate.
//-----------------------------------------------------------------------

Planetocentric::Planetocentric(const GroundCoordinate& Gc)
  : naif_code_(Gc.naif_code())
{
  if(const Planetocentric* g = 
     dynamic_cast<const Planetocentric*>(&Gc)) {
    *this = *g;
    return;
  }
  Gc.lat_lon_height(lat_, lon_, height_ellipsoid_);
}

boost::shared_ptr<CartesianFixed> Planetocentric::convert_to_cf() const
{
  double lat = lat_ * Constant::deg_to_rad;
  double lon = lon_ * Constant::deg_to_rad;
  double r = planet_radius_lat(lat) + height_ellipsoid_;
  return boost::shared_ptr<CartesianFixed>
    (new PlanetFixed(r * cos(lat) * cos(lon),
		     r * cos(lat) * sin(lon),
		     r * sin(lat), naif_code_));
}

void Planetocentric::print(std::ostream& Os) const
{
  Os << PlanetConstant::name(naif_code_)
     << " Planetocentric: (" << latitude() << " deg, " 
     << longitude() << " deg, "
     << height_reference_surface() << " m)";
}

//-----------------------------------------------------------------------
/// Make an Planetocentric with the given latitude, longitude, and height.
/// Latitude and longitude are in degrees, height is in meters.
/// Longitude should be between -180 and 180 and latitude -90 and 90.
//-----------------------------------------------------------------------

Planetocentric::Planetocentric
(double Latitude, double Longitude, double Height_ellipsoid,
 int Naif_code)
  : lat_(Latitude), lon_(Longitude), height_ellipsoid_(Height_ellipsoid),
    naif_code_(Naif_code)
{
  range_check_inclusive(lat_, -90.0, 90.0);
  range_check_inclusive(lon_, -180.0, 180.0);
}

boost::shared_ptr<CartesianInertial> 
PlanetFixed::convert_to_ci(const Time& T) const
{
  boost::shared_ptr<CartesianInertial> res(new PlanetInertial(naif_code_));
  CartesianFixed::toolkit_coordinate_interface->
    to_inertial(naif_code_, T, *this, *res);
  return res;
}

double PlanetFixed::height_reference_surface() const
{
  return norm(position) - planet_radius();
}

Planetocentric PlanetFixed::convert_to_planetocentric() const
{
  return Planetocentric(latitude(), longitude(), height_reference_surface(),
			naif_code_);
}


