#include "orbit_quaternion_list.h"
#include <boost/foreach.hpp>

using namespace GeoCal;

// See base class for description.
boost::shared_ptr<OrbitData> OrbitQuaternionList::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());
  time_map::const_iterator i = orbit_data_map.lower_bound(T);
  if(i->first - T == 0.0)
    return i->second;
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  const QuaternionOrbitData& q1 = *(i->second);
  return GeoCal::interpolate(q1, q2, T);
}

// See base class for description.
boost::shared_ptr<OrbitData> 
OrbitQuaternionList::orbit_data(const TimeWithDerivative& T) const
{
  range_check(T.value(), min_time(), max_time());
  time_map::const_iterator i = orbit_data_map.lower_bound(T.value());
  if(i->first - T == 0.0)
    return i->second;
  const QuaternionOrbitData& q2 = *(i->second);
  --i;
  const QuaternionOrbitData& q1 = *(i->second);
  return GeoCal::interpolate(q1, q2, T);
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
