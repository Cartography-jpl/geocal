#include "orbit_quaternion_list_offset.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitQuaternionListOffset::save(Archive& Ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void OrbitQuaternionListOffset::load(Archive& Ar, const unsigned int version)
{
  init();
}
template<class Archive>
void OrbitQuaternionListOffset::serialize(Archive & ar,
					   const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(orbit_underlying) & GEOCAL_NVP_(pos_off);
  boost::serialization::split_member(ar, *this, version);
}  

template<class Archive>
void OrbitScCoorOffset::serialize(Archive & ar,
					   const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(orbit_underlying) & GEOCAL_NVP_(pos_off);
}  

GEOCAL_IMPLEMENT(OrbitQuaternionListOffset);
GEOCAL_IMPLEMENT(OrbitScCoorOffset);
#endif

//-----------------------------------------------------------------------
/// Finish initialization.
//-----------------------------------------------------------------------

void OrbitQuaternionListOffset::init()
{
  if(pos_off_.rows() != 3)
    throw Exception("Position offset needs to be size 3.");
  BOOST_FOREACH(time_map::value_type i, orbit_underlying()->orbit_data_map)
    orbit_data_map[i.first] = boost::shared_ptr<QuaternionOrbitData>();
  OrbitQuaternionList::initialize();
}

// See base class for description
void OrbitQuaternionListOffset::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitQuaternionListOffset\n"
     << "  Position offset: (" << pos_off_(0) << ", " << pos_off_(1) << ", "
     << pos_off_(2) << ")\n"
     << "  Underlying orbit:\n";
  opad << *orbit_underlying() << "\n";
  opad.strict_sync();
}

// See base class for description
void OrbitScCoorOffset::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitScCoorOffset\n"
     << "  Position offset: (" << pos_off_(0) << ", " << pos_off_(1) << ", "
     << pos_off_(2) << ")\n"
     << "  Underlying orbit:\n";
  opad << *orbit_underlying() << "\n";
  opad.strict_sync();
}

boost::shared_ptr<QuaternionOrbitData>
OrbitQuaternionListOffset::orbit_data_create(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData> od = orbit_underlying()->orbit_data_create(T);
  ScLookVector slv(pos_off_(0), pos_off_(1), pos_off_(2));
  boost::array<double, 3> pos_off;
  if(od->from_cf()) {
    CartesianFixedLookVector lv = od->cf_look_vector(slv);
    pos_off = lv.look_vector;
  } else {
    CartesianInertialLookVector lv = od->ci_look_vector(slv);
    pos_off = lv.look_vector;
  }
  return boost::make_shared<QuaternionOrbitData>(*od, pos_off,
			 boost::math::quaternion<double>(1,0,0,0));
}

boost::shared_ptr<OrbitData>
OrbitScCoorOffset::orbit_data(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData> od = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orbit_underlying()->orbit_data(T));
  if(!od)
    throw Exception("OrbitScCoorOffset only works with orbits that return a QuaternionOrbitData");
  ScLookVector slv(pos_off_(0), pos_off_(1), pos_off_(2));
  boost::array<double, 3> pos_off;
  if(od->from_cf()) {
    CartesianFixedLookVector lv = od->cf_look_vector(slv);
    pos_off = lv.look_vector;
  } else {
    CartesianInertialLookVector lv = od->ci_look_vector(slv);
    pos_off = lv.look_vector;
  }
  return boost::make_shared<QuaternionOrbitData>(*od, pos_off,
			 boost::math::quaternion<double>(1,0,0,0));
}
