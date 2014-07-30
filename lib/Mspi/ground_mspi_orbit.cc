#include "ground_mspi_orbit.h"
#include "constant.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. The azimuth and zenith angles should be in degrees.
//-----------------------------------------------------------------------

GroundMspiOrbitData::GroundMspiOrbitData
(const Time& Tm, const GroundCoordinate& Pos,
 double Azimuth, double Zenith)
{
  // Space craft to local north
  sc_to_ln = quat_rot("zy", (90 - Azimuth) * Constant::deg_to_rad,
	       -Zenith * Constant::deg_to_rad);

  // This is labeled in the old code as "local north to ECI". However
  // this *isn't*, instead this is a combination of things. Duplicate
  // what we had in the code so we match.

  double lat = Pos.latitude() * Constant::deg_to_rad;
  double lon = Pos.longitude() * Constant::deg_to_rad;
  double sinlon = sin(lon);
  double coslon = cos(lon);
  double sinlat = sin(lat);
  double coslat = cos(lat);
  blitz::Array<double, 2> ln_to_cf_mat(3,3);
  ln_to_cf_mat = 
    -sinlon, -coslon * sinlat, coslon *coslat,
    coslon, -sinlon * sinlat, sinlon * coslat,
    0, coslat, sinlat;
  boost::math::quaternion<double> ln_to_cf = matrix_to_quaternion(ln_to_cf_mat);
  boost::math::quaternion<double>  sc_to_cf = ln_to_cf * sc_to_ln;
  boost::array<double, 3> vel = {{0,0,0}};
  initialize(Tm, Pos.convert_to_cf(), vel, sc_to_cf);
}

//-----------------------------------------------------------------------
/// Convert from ScLookVector to LnLookVector.
//-----------------------------------------------------------------------

LnLookVector GroundMspiOrbitData::ln_look_vector(const ScLookVector& Sl) const
{
  return LnLookVector(sc_to_ln * Sl.look_quaternion() *
		      conj(sc_to_ln));
}

//-----------------------------------------------------------------------
/// Convert from LnLookVector to ScLookVector.
//-----------------------------------------------------------------------

ScLookVector GroundMspiOrbitData::sc_look_vector(const LnLookVector& Ln) const
{
  return ScLookVector(conj(sc_to_ln) * Ln.look_quaternion() *
		      sc_to_ln);
}

void GroundMspiOrbitData::print(std::ostream& Os) const {
  Os << "GroundMspiOrbitData\n";
}

void GroundMspiOrbit::print(std::ostream& Os) const {
  Os << "GroundMspiOrbit\n"
     << "  Start time: " << tstart << "\n"
     << "  Position:   " << *pos << "\n"
     << "  Azimuth:    " << azimuth_ << " deg\n"
     << "  Start elevation angle: " << start_elevation_angle_ << " deg\n"
     << "  Rotation rate:         " << rotation_rate_ << " deg/s\n";
}

