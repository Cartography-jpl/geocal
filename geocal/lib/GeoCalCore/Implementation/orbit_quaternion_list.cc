#include "orbit_quaternion_list.h"

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
void OrbitQuaternionList::print(std::ostream& Os) const
{
  Os << "OrbitQuaternionList:\n"
     << "  Min time:      " << min_time() << "\n"
     << "  Max time:      " << max_time() << "\n"
     << "  Number points: " << orbit_data_map.size() << "\n";
}
