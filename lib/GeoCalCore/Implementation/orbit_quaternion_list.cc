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

GEOCAL_IMPLEMENT(OrbitListCache);
#endif

// See base class for description.
boost::shared_ptr<OrbitData> OrbitQuaternionList::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());
  time_map::iterator i = orbit_data_map.lower_bound(T);
  check_lazy_evaluation(i);
  if(i->first - T == 0.0)
    return i->second;
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  check_lazy_evaluation(i);
  const QuaternionOrbitData& q1 = *(i->second);
  return GeoCal::interpolate(q1, q2, T);
}

// See base class for description.
boost::shared_ptr<OrbitData> 
OrbitQuaternionList::orbit_data(const TimeWithDerivative& T) const
{
  range_check(T.value(), min_time(), max_time());
  time_map::iterator i = orbit_data_map.lower_bound(T.value());
  // Special handling if we are looking at the very first point
  if(i == orbit_data_map.begin() && i->first - T == 0.0)
    ++i;
  check_lazy_evaluation(i);
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  check_lazy_evaluation(i);
  const QuaternionOrbitData& q1 = *(i->second);
  return GeoCal::interpolate(q1, q2, T);
}

boost::shared_ptr<CartesianFixed> OrbitQuaternionList::position_cf(Time T) const
{
  range_check(T, min_time(), max_time());
  time_map::iterator i = orbit_data_map.lower_bound(T);
  check_lazy_evaluation(i);
  if(i->first - T == 0.0)
    return i->second->position_cf();
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  check_lazy_evaluation(i);
  const QuaternionOrbitData& q1 = *(i->second);

  double tspace = q2.time() - q1.time();
  double toffset = T - q1.time();
  boost::array<double, 3> pos1, pos2, vel_cf, pos_cf;
  pos1[0] = q1.position_cf()->position[0];
  pos1[1] = q1.position_cf()->position[1];
  pos1[2] = q1.position_cf()->position[2];
  pos2[0] = q2.position_cf()->position[0];
  pos2[1] = q2.position_cf()->position[1];
  pos2[2] = q2.position_cf()->position[2];
  Orbit::interpolate(pos1, q1.velocity_cf(), pos2, q1.velocity_cf(), toffset,
		     tspace, pos_cf, vel_cf);
  return q1.position_cf()->create(pos_cf);
}

// See base class for description.
ScLookVector OrbitQuaternionList::sc_look_vector
(Time T, const CartesianFixed& Pt) const
{
  range_check(T, min_time(), max_time());
  time_map::iterator i = orbit_data_map.lower_bound(T);
  check_lazy_evaluation(i);
  if(i->first - T == 0.0) {
    boost::shared_ptr<OrbitData> od = i->second;
    boost::array<double, 3> p1 = Pt.position;
    boost::array<double, 3> p2 = od->position_cf()->position;
    CartesianFixedLookVector lv;
    lv.look_vector[0] = p1[0] - p2[0];
    lv.look_vector[1] = p1[1] - p2[1];
    lv.look_vector[2] = p1[2] - p2[2];
    return od->sc_look_vector(lv);
  }
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  check_lazy_evaluation(i);
  const QuaternionOrbitData& q1 = *(i->second);

  double tspace = q2.time() - q1.time();
  double toffset = T - q1.time();
  boost::math::quaternion<double> sc_to_cf_ = 
    interpolate_quaternion_rotation(q1.sc_to_cf(), 
			   q2.sc_to_cf(), toffset, tspace);
  boost::array<double, 3> pos1, pos2, vel_cf, pos_cf;
  pos1[0] = q1.position_cf()->position[0];
  pos1[1] = q1.position_cf()->position[1];
  pos1[2] = q1.position_cf()->position[2];
  pos2[0] = q2.position_cf()->position[0];
  pos2[1] = q2.position_cf()->position[1];
  pos2[2] = q2.position_cf()->position[2];
  Orbit::interpolate(pos1, q1.velocity_cf(), pos2, q1.velocity_cf(), toffset,
		     tspace, pos_cf, vel_cf);

  boost::array<double, 3> p1 = Pt.position;
  CartesianFixedLookVector lv;
  lv.look_vector[0] = p1[0] - pos_cf[0];
  lv.look_vector[1] = p1[1] - pos_cf[1];
  lv.look_vector[2] = p1[2] - pos_cf[2];

  // Do abberation of light correction.
  ScLookVector res;
  double k = lv.length() / Constant::speed_of_light;
  boost::math::quaternion<double> vel_cf_q(0, vel_cf[0], vel_cf[1], vel_cf[2]);
  boost::math::quaternion<double> sc =
    conj(sc_to_cf_) * (lv.look_quaternion() + k * vel_cf_q) * sc_to_cf_;
  res.look_quaternion(sc);
  return res;
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


void OrbitListCache::notify_update(const Orbit& Orb)
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
  for(int i = tt->min_line(); i <= tt->max_line(); ++i) {
    Time t;
    FrameCoordinate fc;
    tt->time(ImageCoordinate(i, sample_), t, fc);
    orbit_data_map[t] = null;
  }
  initialize();
}

