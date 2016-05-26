#include "spice_orbit.h"
#include "geocal_serialize_support.h"
#include "geocal_internal_config.h"
#include "spice_helper.h"
#include <boost/lexical_cast.hpp>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SpiceOrbit::save(Archive & ar, const unsigned int version) const
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(body_id)
    & GEOCAL_NVP_(satellite_id)
    & GEOCAL_NVP_(kernel_name);
}

template<class Archive>
void SpiceOrbit::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(body_id)
    & GEOCAL_NVP_(satellite_id)
    & GEOCAL_NVP_(kernel_name);
  if(kernel_name_ != "")
    SpiceHelper::add_kernel(kernel_name_);
}

GEOCAL_IMPLEMENT(SpiceOrbit);
#endif

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
/// Create a orbit based on SPICE data. This takes the satellite ID,
/// which is the number used to generate the ID the SPICE toolkit
/// uses, for example the TLE spacecraft code. Note that
/// the SPICE number is actually -100000 - satellite ID
///
/// You need to make sure the spice kernel describing this satellite
/// has been loaded. Often you will have a single kernel that you want
/// used, in that case you can pass this to the constructor and that
/// kernel will be loaded.
///
/// Note that if you pass a kernel name, we save that and make sure it
/// is loaded when we create this class from serialization. 
//-----------------------------------------------------------------------

SpiceOrbit::SpiceOrbit(int Satellite_id, const std::string& Kernel_name,
		       int Body_id)
  : body_id_(Body_id),
    satellite_id_(Satellite_id),
    kernel_name_(Kernel_name)
{
  if(kernel_name_ != "")
    SpiceHelper::add_kernel(kernel_name_);
  // What is time range here?
}

// See base class for description
boost::shared_ptr<OrbitData> SpiceOrbit::orbit_data(Time T) const
{
  boost::array<double, 3> p, v;
  SpiceHelper::state_vector(body_id_, 
		    boost::lexical_cast<std::string>(satellite_spice_id()),
		    T, p, v);
  boost::shared_ptr<CartesianFixed> pf;
  if(body_id_ == Ecr::EARTH_NAIF_CODE)
    pf.reset(new Ecr(p));
  else
    // We just need to set the right PlanetFixed<NAIF_CODE>. Not sure
    // if we need a giant if statement, or if there is some other way
    // to do this. But for now, just punt.
    throw Exception("Right now only work with earth");
  // Want to put in frame kernel, but do that in the next step
  Array<double, 1> x(3), y(3), z(3), p2(3), v2(3);
  p2 = p[0], p[1], p[2];
  v2 = v[0], v[1], v[2];
  z = -p2 / normb(p2);
  x = v2 - z * dotb(v2, z);
  x /= normb(x);
  y = cross2(z, x);
  double sc_to_cf[3][3] = {{x(0), y(0), z(0)},
			   {x(1), y(1), z(1)},
			   {x(2), y(2), z(2)}};
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(T, pf, v, matrix_to_quaternion(sc_to_cf)));
}

// Print to stream
void SpiceOrbit::print(std::ostream& Os) const
{
  Os << "SpiceOrbit:\n"
     << "  Body ID:      " << body_id_ << "\n"
     << "  Satellite ID: " << satellite_id() << "\n"
     << "  Kernel name:  " << kernel_name_ << "\n";
}

