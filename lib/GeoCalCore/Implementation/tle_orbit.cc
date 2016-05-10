#include "tle_orbit.h"
#include "eci.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
#include "geocal_internal_config.h"
#include "spice_helper.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#ifdef HAVE_SPICE
extern "C" {
#include "SpiceUsr.h"
int ev2lin_(double *et, double *geophs, double *
	    elems, double *state);
}
#endif

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void TleOrbit::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(tle);
}
GEOCAL_IMPLEMENT(TleOrbit);
#endif


// These are the physical constants used by space command. See
// https://celestrak.com/NORAD/documentation/spacetrk.pdf.
// This is J2, J3, J4, KE, QO, SO, ER, AE.

double TleOrbit::geophs[8] =
    {1.082616e-3, -2.53881e-6, -1.65597e-6, 7.43669161e-2,
      120.0, 78.0, 6378.135, 1.0};


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


//-----------------------------------------------------------------------
/// Fill in the elems and epoch, if not already done.
//-----------------------------------------------------------------------

void TleOrbit::fill_in_elems() const
{
  if(!elem_filled_in) {
    std::vector<std::string> svec;
    boost::split(svec, tle_, boost::is_any_of("\n"));
    int lineln = (int) std::max(svec[0].length(), svec[1].length()) + 1;
    std::vector<char> line(lineln * 2, '\0');
    std::copy(svec[0].begin(), svec[0].end(), line.begin());
    std::copy(svec[1].begin(), svec[1].end(), line.begin() + lineln);
#ifdef HAVE_SPICE
    SpiceHelper::spice_setup();
    double epoch;
    getelm_c(1957, lineln, (void*) &(*line.begin()), &epoch, elems);
    SpiceHelper::spice_error_check();
    epoch_ = Time::time_et(epoch);
#else
    throw SpiceNotAvailableException();
#endif
    elem_filled_in = true;
  }
}

// See base class for description
boost::shared_ptr<OrbitData> TleOrbit::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());
  fill_in_elems();
  double state[6];
#ifdef HAVE_SPICE
  double et = T.et();
  ev2lin_(&et, geophs, elems, state);
  SpiceHelper::spice_error_check();
#else
    throw SpiceNotAvailableException();
#endif
  // These are in eci coordinates.
  Array<double, 1> p(3), v(3);
  // Spice returns km, so we need to convert to meters
  p = state[0] * 1000, state[1] * 1000, state[2] * 1000;
  v = state[3] * 1000, state[4] * 1000, state[5] * 1000;

//-----------------------------------------------------------------------
// Rotation matrix. This is equivalent to a nadir looking.
//-----------------------------------------------------------------------

  Array<double, 1> x(3), y(3), z(3);
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

//-----------------------------------------------------------------------
/// Return the revolution number at the epoch.
//-----------------------------------------------------------------------

int TleOrbit::revolution_number_at_epoch() const
{
  // Find second line
  size_t i = tle_.find("\n2");
  // These positions are fixed. See
  // https://celestrak.com/columns/v04n03/
  return boost::lexical_cast<int>(tle_.substr(i + 64, 5));
}

void TleOrbit::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "TleOrbit:\n"
     << "  TLE:\n";
  opad << tle() << "\n";
  opad.strict_sync();
}
