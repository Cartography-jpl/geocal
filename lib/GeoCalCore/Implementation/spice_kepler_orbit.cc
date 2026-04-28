#include "spice_kepler_orbit.h"
#include "geocal_serialize_support.h"
#include "geocal_config.h"
#include "spice_helper.h"
#include "eci.h"
#ifdef HAVE_SPICE
extern "C" {
#include "SpiceUsr.h"
}
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SpiceKeplerOrbit::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(elements);
}

GEOCAL_IMPLEMENT(SpiceKeplerOrbit);
#endif

//-----------------------------------------------------------------------
/// Create an kepler orbit
//-----------------------------------------------------------------------

SpiceKeplerOrbit::SpiceKeplerOrbit(const blitz::Array<double, 1>& elements, Time min_time,
				   Time max_time)
: Orbit(min_time, max_time),
  elements_(elements.copy())
{
}

template<class T> inline T dotb(const blitz::Array<T, 1>& x, 
				const blitz::Array<T, 1>& y)
{
  return sum(x * y);
}

template<class T> inline T normb(const blitz::Array<T, 1>& x)
{
  return std::sqrt(dotb(x, x));
}

template<class T> inline blitz::Array<T, 1> cross2
(const blitz::Array<T, 1>& x,
 const blitz::Array<T, 1>& y)
{
  blitz::Array<T, 1> res(3);
  res(0) = x(1) * y(2) - x(2) * y(1);
  res(1) = x(2) * y(0) - x(0) * y(2);
  res(2) = x(0) * y(1) - x(1) * y(0);
  return res;
}

// See base class for description
boost::shared_ptr<OrbitData> SpiceKeplerOrbit::orbit_data(Time T) const
{
  blitz::Array<double, 1> sv = SpiceKeplerOrbit::spice_conics(elements_, T);
  blitz::Array<double, 1> p(3), v(3);
  p(0) = sv(0) * 1000;
  p(1) = sv(1) * 1000;
  p(2) = sv(2) * 1000;
  v(0) = sv(3) * 1000;
  v(1) = sv(4) * 1000;
  v(2) = sv(5) * 1000;
  blitz::Array<double, 1> x(3), y(3), z(3);
  z = -p / normb(p);
  x = v - z * dotb(v, z);
  x /= normb(x);
  y = cross2(z, x);
  double sc_to_ci[3][3] = {{x(0), y(0), z(0)},
			   {x(1), y(1), z(1)},
			   {x(2), y(2), z(2)}};
  boost::shared_ptr<CartesianInertial> pci(new Eci(p(0), p(1), p(2)));
  boost::array<double, 3> v2 = {{v(0), v(1), v(2)}};
  return boost::shared_ptr<OrbitData>(new QuaternionOrbitData(T, pci, v2, 
		      matrix_to_quaternion(sc_to_ci)));
}

// Print to stream
void SpiceKeplerOrbit::print(std::ostream& Os) const
{
  Os << "SpiceKeplerOrbit:\n";
}

double SpiceKeplerOrbit::mu(const std::string& Body_name)
{
#ifdef HAVE_SPICE
  SpiceHelper::spice_setup();
  double res[1];
  int dim;
  bodvrd_c(Body_name.c_str(), "GM",  1, &dim, res);
  SpiceHelper::spice_error_check();
  return res[0];
#else
  throw SpiceNotAvailableException();
#endif    
}

blitz::Array<double, 1> SpiceKeplerOrbit::spice_conics
(const blitz::Array<double, 1>& elements, Time T)
{
#ifdef HAVE_SPICE
  SpiceHelper::spice_setup();
  blitz::Array<double, 1> res(6);
  conics_c(elements.data(), T.et(), res.data());
  SpiceHelper::spice_error_check();
  return res;
#else
  throw SpiceNotAvailableException();
#endif    
}

blitz::Array<double, 1> SpiceKeplerOrbit::spice_oscelt
(const OrbitData& od)
{
#ifdef HAVE_SPICE
  SpiceHelper::spice_setup();
  // Constant for earth. Can read from spice kernel, but we don't
  // normally load that particular one (gm_de440.tpc)
  double mu = 398600.4355070226;
  double state[6];
  state[0] = od.position_ci()->position[0] / 1000;
  state[1] = od.position_ci()->position[1] / 1000;
  state[2] = od.position_ci()->position[2] / 1000;
  state[3] = od.velocity_ci()[0] / 1000;
  state[4] = od.velocity_ci()[1] / 1000;
  state[5] = od.velocity_ci()[2] / 1000;
  blitz::Array<double, 1> res(8);
  oscelt_c(state, od.time().et(), mu, res.data());
  SpiceHelper::spice_error_check();
  return res;
#else
  throw SpiceNotAvailableException();
#endif    
}

