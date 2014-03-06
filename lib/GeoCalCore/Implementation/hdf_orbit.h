#ifndef HDF_ORBIT_H
#define HDF_ORBIT_H
#include "orbit.h"
#include "hdf_file.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This is an implementation of an Orbit that reads position, velocity,
  and attitude quaternion from an HDF file. For times that fall
  between the values given in the file, we interpolate to get the
  OrbitData.

  The file should have the following fields:

  <Base group>/Attitude/Time  - natt in size, time of attitude
      measurement as doubles.
  <Base group>/Attitude/Quaternion - natt x 4 in size, quaternion
      to take same coordinate system as Position (e.g., ECI)
  <Base group>/Ephemeris/Time - neph in size, time of ephemeris
      measurement as doubles
  <Base group>/Ephemeris/Position - neph x 3 in size, position
      measurement as doubles in meters
  <Base group>/Ephemeris/Velocity - neph x 3 in size, velocity
      measurement as doubles in meter/second

   Because it is useful, we allow the type of position measurement
   and time to be changed. The measurement class is passed in, e.g.,
   Eci, as is a small wrapper to give the conversion to Time.
*******************************************************************/

template<class PositionType, class TimeCreatorType> class HdfOrbit : public Orbit {
public:

  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit");
  virtual ~HdfOrbit() {}

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
    Os << "HdfOrbit\n"
       << "  File name: " << file_name() << "\n"
       << "  Min time:      " << min_time() << "\n"
       << "  Max time:      " << max_time() << "\n";
  }

//-----------------------------------------------------------------------
/// Return the file name
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Return the base group
//-----------------------------------------------------------------------

  const std::string& base_group() const {return bgroup;}
private:
  std::string fname;
  std::string bgroup;
  typedef std::map<Time, boost::math::quaternion<double> > 
  time_attmap;
  time_attmap att_map;
  typedef typename std::pair<PositionType, boost::array<double, 3> > posvel;
  typedef std::map<Time,  posvel> time_posmap;
  time_posmap pos_map;
};

//-----------------------------------------------------------------------
/// Read the given orbit data file. You can optional pass the base
/// group of the HDF file, the default is "/Orbit"
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType> inline 
HdfOrbit<PositionType, TimeCreatorType>::HdfOrbit(const std::string& Fname, const std::string& Base_group)
  :fname(Fname),
   bgroup(Base_group)
{
  using namespace blitz;
  TimeCreatorType tc;
  HdfFile f(fname);
  Array<double, 1> tdouble = 
    f.read_field<double, 1>(bgroup + "/Ephemeris/Time");
  Array<double, 2> pos = 
    f.read_field<double, 2>(bgroup + "/Ephemeris/Position");
  Array<double, 2> vel = 
    f.read_field<double, 2>(bgroup + "/Ephemeris/Velocity");
  if(tdouble.rows() != pos.rows() || 
     tdouble.rows() != vel.rows()) {
    Exception e("Need to have the same size data for time, position and velocity\n");
    e << "  File:       " << fname << "\n"
      << "  Base group: " << bgroup << "\n";
    throw e;
  }
  if(pos.cols() != 3 ||
     vel.cols() != 3) {
    Exception e("Position and velocity need to have 3 columns\n");
    e << "  File:       " << fname << "\n"
      << "  Base group: " << bgroup << "\n";
    throw e;
  }
  for(int i = 0; i < tdouble.rows(); ++i) {
    Time t = tc(tdouble(i));
    PositionType p(pos(i, 0), pos(i, 1), pos(i, 2));
    boost::array<double, 3> v = {{vel(i, 0), vel(i, 1), vel(i, 2)}};
    pos_map[t] = posvel(p, v);
  }
  Array<double, 1> tdouble2 = 
    f.read_field<double, 1>(bgroup + "/Attitude/Time");
  Array<double, 2> quat = 
    f.read_field<double, 2>(bgroup + "/Attitude/Quaternion");
  if(tdouble2.rows() != quat.rows()) {
    Exception e("Need to have the same size data for time, and attitude\n");
    e << "  File:       " << fname << "\n"
      << "  Base group: " << bgroup << "\n";
    throw e;
  }
  if(quat.cols() != 4) {
    Exception e("Quaternion needs to have 4 columns\n");
    e << "  File:       " << fname << "\n"
      << "  Base group: " << bgroup << "\n";
    throw e;
  }
  for(int i = 0; i < tdouble2.rows(); ++i) {
    Time t = tc(tdouble2(i));
    att_map[t] = boost::math::quaternion<double>(quat(i, 0), quat(i, 1),
						 quat(i, 2), quat(i, 3));
  }
  min_tm = std::max(att_map.begin()->first, pos_map.begin()->first);
  max_tm = std::min(att_map.rbegin()->first, pos_map.rbegin()->first);
}

//-----------------------------------------------------------------------
/// Return orbit data for given time.
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType>
inline boost::shared_ptr<OrbitData> HdfOrbit<PositionType, TimeCreatorType>::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());
  time_attmap::const_iterator i = att_map.lower_bound(T);
  boost::math::quaternion<double> att;
  if(i->first - T == 0.0)
    att = i->second;
  else {
    const boost::math::quaternion<double>& a2 = i->second;
    Time t2 = i->first;
    --i;
    const boost::math::quaternion<double>& a1 = i->second;
    Time t1 = i->first;
    att = interpolate(a1, a2, T - t1, t2 - t1);
  }
  boost::array<double, 3> pos;
  boost::array<double, 3> vel;
  typename time_posmap::const_iterator i2 = pos_map.lower_bound(T);
  if(i2->first - T == 0.0) {
    pos = i2->second.first.position;
    vel = i2->second.second;
  } else {
    boost::array<double, 3> p2 = i2->second.first.position;
    boost::array<double, 3> v2 = i2->second.second;
    Time t2 = i2->first;
    --i2;
    boost::array<double, 3> p1 = i2->second.first.position;
    boost::array<double, 3> v1 = i2->second.second;
    Time t1 = i2->first;
    interpolate(p1, v1, p2, v2, T - t1, t2 - t1, pos, vel);
  }
  boost::shared_ptr<PositionType> pv(new PositionType(pos[0], pos[1], pos[2]));
  return boost::shared_ptr<OrbitData>(new QuaternionOrbitData(T, pv, vel, att));
}

}
#endif