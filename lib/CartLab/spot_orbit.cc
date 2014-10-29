#include "spot_orbit.h"
#include "ecr.h"
#include "geocal_matrix.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor.
///
/// \param Ephemeris_time Time of each ephemeris point
/// \param Ephemeris Ephemeris at each time. This is a number_eph_time x 6
///   array. Each row has the position x, y, z and velocity x, y, z.
///   This is in meters and meter/second, in ECR coordinates.
/// \param Attitude_time Time of each attitude point.
/// \param Ypr Attitude. This is a number_att_time x 3 array. Each row
///   has the yaw, pitch and roll. This is in radians. This is YPR
///   reported by SPOT in the DIMAP file, and uses the conventions
///   described in "SPOT Geometry Handbook"
//-----------------------------------------------------------------------

SpotOrbit::SpotOrbit(const std::vector<Time>& Ephemeris_time, 
		     const blitz::Array<double, 2>& Ephemeris,
		     const std::vector<Time>& Attitude_time,
		     const blitz::Array<double, 2>& Ypr)
{
  if((int) Ephemeris_time.size() != Ephemeris.shape()[0])
    throw Exception("Ephemeris_time and Empheris need to be the same size");
  if(Ephemeris.shape()[1] != 6)
    throw Exception("Ephemeris needs to have 6 columns");
  if((int) Attitude_time.size() != Ypr.shape()[0])
    throw Exception("Attitude_time and Ypr need to be the same size");
  if(Ypr.shape()[1] != 3)
    throw Exception("Ypr needs to have 3 columns");
  for(int i = 0; i < Ephemeris.shape()[0]; ++i) {
    if(i > 0 &&
       Ephemeris_time[i] < Ephemeris_time[i - 1])
      throw Exception("Time needs to be sorted");
    teph.push_back(Ephemeris_time[i]);
    posvel.push_back(Ephemeris(i, blitz::Range::all()).copy());
  }

//-----------------------------------------------------------------------
// For historical reasons (according to "SPOT Geometry Guide"), the
// pitch and roll are about -X and -Y. So we need to change the sign
// of Roll and Pitch, but not yaw, here.
//
//-----------------------------------------------------------------------

  for(int i = 0; i < Ypr.shape()[0]; ++i) {
    double yaw = Ypr(i,0);
    double pitch = Ypr(i,1);
    double roll = Ypr(i,2);
    sc_to_orb[Attitude_time[i]] = quat_rot_x(-pitch) * quat_rot_y(-roll) * 
      quat_rot_z(yaw);
  }
  min_tm = std::max(teph.front(), sc_to_orb.begin()->first);
  max_tm = std::min(teph.back(), sc_to_orb.rbegin()->first);
}

//-----------------------------------------------------------------------
/// Return orbit data for the given time.
//-----------------------------------------------------------------------

boost::shared_ptr<OrbitData> SpotOrbit::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());

//-----------------------------------------------------------------------
// Interpolate position and velocity. We do a Lagrangian
// interpolation, using up to 4 points before and after the current
// time.
//
// Note that we originally did the spline fit to interpolate position
// and velocity that we typically do with MISR orbit data (e.g.,
// Orbit::interpolate function). This worked pretty well
// for SPOT 5, but for SPOT 4 there were significant errors. Not sure
// of the source of this, perhaps there is problems with the velocity
// measurement or each individual ephemeris point. But when we did the
// 8 point Lagrangian interpolation as described in the SPOT Geometry
// Handbook (equation 2a and 2b), we got much better results.
//-----------------------------------------------------------------------

  typedef std::vector<Time>::difference_type itype;
  itype iv = (int) (std::upper_bound(teph.begin(), teph.end(), T) - 
		    teph.begin());
  itype istart = iv - 4;
  itype iend = iv + 4;
  istart = std::max((itype) 0, istart);
  iend = std::min(iend, (itype) teph.size());
  blitz::Array<double, 1> pv = lagrangian_interpolation(teph.begin() + istart,
							teph.begin() + iend,
							T,
							posvel.begin() + istart,
							posvel.begin() + iend);

//-----------------------------------------------------------------------
// From the "SPOT Geometry Handbook", we have z is unit vector in
// direction of p, x is cross product of v and z, and y is cross
// product z and x. See note in front of this class for comparison of
// this coordinate system with the one used on other instruments such
// as MISR.
//-----------------------------------------------------------------------

  boost::array<double, 3> p, x, y, z, v;
  p[0] = pv(0);
  p[1] = pv(1);
  p[2] = pv(2);
  double t = norm(p);
  z[0] = pv(0) / t; 
  z[1] = pv(1) / t; 
  z[2] = pv(2) / t;
  v[0] = pv(3);
  v[1] = pv(4);
  v[2] = pv(5);
  cross(v,z,x);
  t = norm(x);
  x[0] /= t;
  x[1] /= t;
  x[2] /= t;
  cross(z,x,y);
  double orb_to_cf_m[3][3] = {{x[0], y[0], z[0]},
			      {x[1], y[1], z[1]},
			      {x[2], y[2], z[2]}};
  boost::math::quaternion<double> orb_to_cf = matrix_to_quaternion(orb_to_cf_m);

//-----------------------------------------------------------------------
// Interpolate attitude quaternion.
//-----------------------------------------------------------------------

  typedef std::map<Time, boost::math::quaternion<double> >::const_iterator ci2;
  ci2 s1 = sc_to_orb.upper_bound(T);
  ci2 s2 = s1;
  --s1;
  Time tstart = s1->first;
  Time tend = s2->first;
  boost::math::quaternion<double> sc_to_cf = orb_to_cf * 
    interpolate(s1->second, s2->second, T - tstart, tend - tstart);
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(T, boost::shared_ptr<CartesianFixed>(new Ecr(p)),
			     v, sc_to_cf));
}

boost::shared_ptr<OrbitData> 
SpotOrbit::orbit_data(const TimeWithDerivative& T) const
{
  throw Exception("Not implemented yet");
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void SpotOrbit::print(std::ostream& Os) const
{
  Os << "SpotOrbit";
}
