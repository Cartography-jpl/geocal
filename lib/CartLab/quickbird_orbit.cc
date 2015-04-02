#include "quickbird_orbit.h"
#include "ecr.h"
#include "eci.h"
#include "constant.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void QuickBirdOrbit::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(eph) & GEOCAL_NVP(att);
}

template<class Archive>
void QuickBirdEphemeris::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(QuickBirdEphemeris);
  ar & GEOCAL_NVP(fname);
}

template<class Archive>
void QuickBirdEphemeris::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(QuickBirdEphemeris);
  ar & GEOCAL_NVP(fname);
  init(fname);
}

template<class Archive>
void QuickBirdAttitude::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(QuickBirdAttitude);
  ar & GEOCAL_NVP(fname);
}

template<class Archive>
void QuickBirdAttitude::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(QuickBirdAttitude);
  ar & GEOCAL_NVP(fname);
  init(fname);
}

GEOCAL_IMPLEMENT(QuickBirdOrbit);
GEOCAL_SPLIT_IMPLEMENT(QuickBirdAttitude);
GEOCAL_SPLIT_IMPLEMENT(QuickBirdEphemeris);
#endif


//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuickBirdEphemeris::print(std::ostream& Os) const
{
  Os << "Quickbird ephemeris file\n"
     << "  File name:     " << file_name() << "\n"
     << "  Min time:      " << min_time() << "\n"
     << "  Time spacing:  " << time_spacing() << " s\n"
     << "  Number points: " << ephemeris().size() << "\n";
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuickBirdAttitude::print(std::ostream& Os) const
{
  Os << "Quickbird attitude file\n"
     << "  File name:     " << file_name() << "\n"
     << "  Min time:      " << min_time() << "\n"
     << "  Time spacing:  " << time_spacing() << " s\n"
     << "  Number points: " << attitude().size() << "\n";
}

//-----------------------------------------------------------------------
/// Constructor. Usually the attitude and ephemeris file have names
/// like blah.EPH and blah.ATT. If you supply just the ephemeris name,
/// we fill in the attitude by replacing ".EPH" with ".ATT".
//-----------------------------------------------------------------------

QuickBirdOrbit::QuickBirdOrbit(const std::string& Ephemeris_file,
			       const std::string& Attitude_file)
{
  eph.reset(new QuickBirdEphemeris(Ephemeris_file));
  std::string atf = Attitude_file;
  if(atf == "") {
    atf = Ephemeris_file;
    size_t t = atf.find(".EPH");
    if(t != std::string::npos)
      atf.replace(t, t + 4, ".ATT");
  }
  att.reset(new QuickBirdAttitude(atf));
  min_tm = std::max(eph->min_time(), att->min_time());
  max_tm = std::min(eph->max_time() - eph->time_spacing(), 
		    att->max_time() - att->time_spacing());
}

//-----------------------------------------------------------------------
/// Return OrbitData for the given time. We should have min_time() <=
/// T < max_time().
//-----------------------------------------------------------------------

boost::shared_ptr<OrbitData> QuickBirdOrbit::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());

// Interpolate position.

  int i = (int) floor((T - eph->min_time()) / eph->time_spacing());
  double toffset = T - (eph->min_time() + i * eph->time_spacing());
  boost::array<double, 3> p1, v1, p2, v2, pres, vres;
  const boost::array<double, 12>& e1 = eph->ephemeris()[i];
  const boost::array<double, 12>& e2 = eph->ephemeris()[i + 1];
  p1[0] = e1[0];
  p1[1] = e1[1];
  p1[2] = e1[2];
  v1[0] = e1[3];
  v1[1] = e1[4];
  v1[2] = e1[5];
  p2[0] = e2[0];
  p2[1] = e2[1];
  p2[2] = e2[2];
  v2[0] = e2[3];
  v2[1] = e2[4];
  v2[2] = e2[5];
  interpolate(p1, v1, p2, v2, toffset, eph->time_spacing(), pres, vres);
  Ecr e(pres[0], pres[1], pres[2]);
  boost::array<double, 3> vel = {{vres[0], vres[1], vres[2]}};

// Interpolate quaternion.

  i = (int) floor((T - att->min_time()) / att->time_spacing());
  toffset = T - (att->min_time() + i * att->time_spacing());

  // Note that there are a few different conventions about the
  // ordering of the quaternion coefficients. The boost library places
  // the real part at the front, so we have a + b i + c j + d k and
  // the quaternion is 4-tuple (a, b, c, d). The convention used by
  // quickbird data  is q1 i + q2 j + q3 k + q4 with the 4-tuple is
  // (q1, q2, q3, q4). That means when we bring this over to the boost
  // library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

  const boost::array<double, 14>& a1 = att->attitude()[i];
  const boost::array<double, 14>& a2 = att->attitude()[i + 1];
  boost::math::quaternion<double> q1(a1[3], a1[0], a1[1], a1[2]);
  boost::math::quaternion<double> q2(a2[3], a2[0], a2[1], a2[2]);
  boost::math::quaternion<double> sc_to_cf = interpolate(q1, q2, toffset, 
 							 att->time_spacing());
  return boost::shared_ptr<OrbitData>(new 
     QuaternionOrbitData(T, e.convert_to_cf(), vel, sc_to_cf));
}

boost::shared_ptr<OrbitData> 
QuickBirdOrbit::orbit_data(const TimeWithDerivative& T) const
{
  range_check(T.value(), min_time(), max_time());

// Interpolate position.

  int i = (int) floor((T.value() - eph->min_time()) / eph->time_spacing());
  AutoDerivative<double> toffset = 
    T - (eph->min_time() + i * eph->time_spacing());
  boost::array<AutoDerivative<double>, 3> p1, v1, p2, v2, pres, vres;
  const boost::array<double, 12>& e1 = eph->ephemeris()[i];
  const boost::array<double, 12>& e2 = eph->ephemeris()[i + 1];
  p1[0] = e1[0];
  p1[1] = e1[1];
  p1[2] = e1[2];
  v1[0] = e1[3];
  v1[1] = e1[4];
  v1[2] = e1[5];
  p2[0] = e2[0];
  p2[1] = e2[1];
  p2[2] = e2[2];
  v2[0] = e2[3];
  v2[1] = e2[4];
  v2[2] = e2[5];
  interpolate(p1, v1, p2, v2, toffset, eph->time_spacing(), pres, vres);
  Ecr e(pres[0].value(), pres[1].value(), pres[2].value());
  boost::array<AutoDerivative<double>, 3> vel = {{vres[0], vres[1], vres[2]}};

// Interpolate quaternion.

  i = (int) floor((T.value() - att->min_time()) / att->time_spacing());
  toffset = T - (att->min_time() + i * att->time_spacing());

  // Note that there are a few different conventions about the
  // ordering of the quaternion coefficients. The boost library places
  // the real part at the front, so we have a + b i + c j + d k and
  // the quaternion is 4-tuple (a, b, c, d). The convention used by
  // quickbird data  is q1 i + q2 j + q3 k + q4 with the 4-tuple is
  // (q1, q2, q3, q4). That means when we bring this over to the boost
  // library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

  const boost::array<double, 14>& a1 = att->attitude()[i];
  const boost::array<double, 14>& a2 = att->attitude()[i + 1];
  boost::math::quaternion<AutoDerivative<double> > 
    q1(a1[3], a1[0], a1[1], a1[2]);
  boost::math::quaternion<AutoDerivative<double> > 
    q2(a2[3], a2[0], a2[1], a2[2]);
  boost::math::quaternion<AutoDerivative<double> > sc_to_cf = 
    interpolate(q1, q2, toffset, att->time_spacing());
  return boost::shared_ptr<OrbitData>(new 
      QuaternionOrbitData(T, e.convert_to_cf(), pres, vel, sc_to_cf));
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void QuickBirdOrbit::print(std::ostream& Os) const
{
  Os << "Quickbird orbit, with files:\n" << *eph << *att;
}


