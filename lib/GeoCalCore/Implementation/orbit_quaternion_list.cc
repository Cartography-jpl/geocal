#include "orbit_quaternion_list.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <boost/foreach.hpp>

using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitListCache::save(Archive& Ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void OrbitListCache::load(Archive& Ar, const unsigned int version)
{
  init();
}

template<class Archive>
void OrbitListCache::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(orbit_underlying) & GEOCAL_NVP(tt)
    & GEOCAL_NVP_(sample);
  boost::serialization::split_member(ar, *this, version);
}

// Note, derived classes likely do *not* want to call this
// serialization method. However, it is useful to have this defined
// since quick and dirty python code may want to use a
// OrbitQuaternionList to generate an orbit from some other data
// source (e.g., CSV file), and then be able to save it.

template<class Archive>
void OrbitQuaternionList::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(orbit_data_map);
}

GEOCAL_IMPLEMENT(OrbitListCache);
GEOCAL_IMPLEMENT(OrbitQuaternionList);
#endif

//-----------------------------------------------------------------------
/// Return the two QuaternionOrbitData that we should
/// interpolate/extrapolate from. Derived classes can override this if
/// there is extra logic to this (e.g., see EcostressOrbit in the
/// ecostress-level1 repository).
//-----------------------------------------------------------------------

void OrbitQuaternionList::interpolate_or_extrapolate_data
(Time T, boost::shared_ptr<GeoCal::QuaternionOrbitData>& Q1,
 boost::shared_ptr<GeoCal::QuaternionOrbitData>& Q2) const
{
  range_check_inclusive(T, min_time(), max_time());
  time_map::iterator i = orbit_data_map.lower_bound(T);
  check_lazy_evaluation(i);
  // Special handling if we are looking at the very first point
  if(i == orbit_data_map.begin() && i->first - T == 0.0) {
    ++i;
    check_lazy_evaluation(i);
  }
  Q2 = i->second;
  --i;
  check_lazy_evaluation(i);
  Q1 = i->second;
}

// See base class for description.
boost::shared_ptr<OrbitData> OrbitQuaternionList::orbit_data(Time T) const
{
  boost::shared_ptr<GeoCal::QuaternionOrbitData> q1, q2;
  interpolate_or_extrapolate_data(T, q1, q2);
  return QuaternionOrbitData::interpolate(*q1, *q2, T, true);
}

// See base class for description.
boost::shared_ptr<OrbitData> 
OrbitQuaternionList::orbit_data(const TimeWithDerivative& T) const
{
  boost::shared_ptr<GeoCal::QuaternionOrbitData> q1, q2;
  interpolate_or_extrapolate_data(T.value(), q1, q2);
  return QuaternionOrbitData::interpolate(*q1, *q2, T, true);
}

boost::shared_ptr<CartesianFixed> OrbitQuaternionList::position_cf(Time T) const
{
  boost::shared_ptr<GeoCal::QuaternionOrbitData> q1, q2;
  interpolate_or_extrapolate_data(T, q1, q2);
  double tspace = q2->time() - q1->time();
  double toffset = T - q1->time();
  boost::array<double, 3> pos1, pos2, vel_cf, pos_cf;
  pos1[0] = q1->position_cf()->position[0];
  pos1[1] = q1->position_cf()->position[1];
  pos1[2] = q1->position_cf()->position[2];
  pos2[0] = q2->position_cf()->position[0];
  pos2[1] = q2->position_cf()->position[1];
  pos2[2] = q2->position_cf()->position[2];
  Orbit::interpolate(pos1, q1->velocity_cf(), pos2, q1->velocity_cf(), toffset,
		     tspace, pos_cf, vel_cf);
  return q1->position_cf()->create(pos_cf);
}

// See base class for description.
ScLookVector OrbitQuaternionList::sc_look_vector
(Time T, const CartesianFixed& Pt) const
{
  boost::shared_ptr<GeoCal::QuaternionOrbitData> q1, q2, q;
  interpolate_or_extrapolate_data(T, q1, q2);
  q = QuaternionOrbitData::interpolate(*q1, *q2, T, true);
  CartesianFixedLookVector lv(*q->position_cf(), Pt);
  return q->sc_look_vector(lv);
}

// See base class for description.
void OrbitQuaternionList::print(std::ostream& Os) const
{
  Os << "OrbitQuaternionList:\n"
     << "  Min time:      " << min_time() << "\n"
     << "  Max time:      " << max_time() << "\n"
     << "  Number points: " << orbit_data_map.size() << "\n";
}

//-----------------------------------------------------------------------
/// Return the list of QuaternionOrbitData.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<QuaternionOrbitData> > 
OrbitQuaternionList::quaternion_orbit_data() const
{
  std::vector<boost::shared_ptr<QuaternionOrbitData> >  res;
  res.reserve(orbit_data_map.size());
  BOOST_FOREACH(time_map::value_type i, orbit_data_map)
    res.push_back(i.second);
  return res;
}

//-----------------------------------------------------------------------
/// Return the Times of QuaternionOrbitData.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<Time> >
OrbitQuaternionList::quaternion_orbit_data_time() const
{
  std::vector<Time>  res;
  res.reserve(orbit_data_map.size());
  BOOST_FOREACH(time_map::value_type i, orbit_data_map)
    res.push_back(new Time(i.first));
  return res;
}

// See base class for description.
void OrbitListCache::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitListCache:\n"
     << "  Underlying orbit:\n";
  opad << orbit_underlying_ << "\n";
  opad.strict_sync();
  Os << "  Time table:\n";
  opad << tt << "\n";
  opad.strict_sync();
  Os << "  Sample: " << sample_ << "\n";
}


void OrbitListCache::notify_update()
{
  boost::shared_ptr<QuaternionOrbitData> null;
  // Throw away all cached values if underlying orbit changes.
  BOOST_FOREACH(time_map::value_type& i, orbit_data_map)
    i.second = null;
}

boost::shared_ptr<QuaternionOrbitData> OrbitListCache::orbit_data_create
(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData>
    od = boost::dynamic_pointer_cast<QuaternionOrbitData>(orbit_underlying_->orbit_data(T));
  if(!od)
    throw Exception("Class only works with QuaternionOrbitData");
  return od;
}

void OrbitListCache::init()
{
  boost::shared_ptr<QuaternionOrbitData> null;
  for(int i = tt->min_line() - 1; i <= tt->max_line() + 1; ++i) {
    Time t;
    FrameCoordinate fc;
    tt->time(ImageCoordinate(i, sample_), t, fc);
    orbit_data_map[t] = null;
  }
  initialize();
}

