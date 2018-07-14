#ifndef HDF_ORBIT_H
#define HDF_ORBIT_H
#include "orbit_quaternion_list.h"
#include "eci_tod.h"
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

   The Attitude is always assumed to have the real part first, like
   boost library uses. We could probably add an option to change this 
   if needed, but for now this is always the case.

   By default, the reported attitude goes from the spacecraft to the
   reference frame (e.g., ECI). However, you can optionally specify
   that the attitude goes in the other direction. 
*******************************************************************/

template<class PositionType, class TimeCreatorType> class HdfOrbit : 
    public OrbitQuaternionList {
public:

//-----------------------------------------------------------------------
/// Read the given orbit data file. You can optional pass the base
/// group of the HDF file, the default is "/Orbit"
//-----------------------------------------------------------------------

  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit",
	   const std::string& Eph_time = "/Ephemeris/Time",
	   const std::string& Eph_pos = "/Ephemeris/Position",
	   const std::string& Eph_vel = "/Ephemeris/Velocity",
	   const std::string& Att_time = "/Attitude/Time",
	   const std::string& Att_quat = "/Attitude/Quaternion",
	   bool Att_from_sc_to_ref_frame = true
	   )
    : fname(Fname),
      bgroup(Base_group),
      eph_time(Eph_time),
      eph_pos(Eph_pos),
      eph_vel(Eph_vel),
      att_time(Att_time),
      att_quat(Att_quat),
      att_from_sc_to_ref_frame(Att_from_sc_to_ref_frame)
  { init(); }
  virtual ~HdfOrbit() {}

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
protected:
  virtual boost::shared_ptr<QuaternionOrbitData> 
  orbit_data_create(Time T) const;
private:
  // Separate out initialization to make serialization a little easier
  void init();
  std::string fname;
  std::string bgroup, eph_time, eph_pos, eph_vel, att_time, att_quat;
  bool att_from_sc_to_ref_frame;
  typedef std::map<Time, boost::math::quaternion<double> > 
  time_attmap;
  time_attmap att_map;
  typedef typename std::pair<PositionType, boost::array<double, 3> > posvel;
  typedef std::map<Time,  posvel> time_posmap;
  time_posmap pos_map;
  HdfOrbit() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

//-----------------------------------------------------------------------
/// Separate out initialization to make serialization a little easier
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType> inline 
void HdfOrbit<PositionType, TimeCreatorType>::init()
{
  using namespace blitz;
  TimeCreatorType tc;
  HdfFile f(fname);
  Array<double, 1> tdouble = 
    f.read_field<double, 1>(bgroup + eph_time);
  Array<double, 2> pos = 
    f.read_field<double, 2>(bgroup + eph_pos);
  Array<double, 2> vel = 
    f.read_field<double, 2>(bgroup + eph_vel);
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
    f.read_field<double, 1>(bgroup + att_time);
  Array<double, 2> quat = 
    f.read_field<double, 2>(bgroup + att_quat);
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
  // Do lazy evaluation of orbit data, since orbit file may be much
  // bigger than the times we are interested (e.g., a full day).
  boost::shared_ptr<QuaternionOrbitData> null;
  for(int i = 0; i < tdouble2.rows(); ++i) {
    Time t = tc(tdouble2(i));
    // att_map always goes from sc_to_refframe (e.g. sc_to_ci). If the
    // underlying data goes the other way, take the conjugate as we
    // read it in
    
    if(att_from_sc_to_ref_frame)
      att_map[t] = boost::math::quaternion<double>(quat(i, 0), quat(i, 1),
						   quat(i, 2), quat(i, 3));
    else
      att_map[t] = boost::math::quaternion<double>(quat(i, 0), -quat(i, 1),
						   -quat(i, 2), -quat(i, 3));
    orbit_data_map[t] = null;
  }
  min_tm = std::max(att_map.begin()->first, pos_map.begin()->first);
  max_tm = std::min(att_map.rbegin()->first, pos_map.rbegin()->first);
}

//-----------------------------------------------------------------------
/// Do full calculation to get orbit data
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType>
inline boost::shared_ptr<QuaternionOrbitData> HdfOrbit<PositionType, TimeCreatorType>::orbit_data_create(Time T) const
{
  range_check_inclusive(T, min_time(), max_time());
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
  return boost::shared_ptr<QuaternionOrbitData>(new QuaternionOrbitData(T, pv, vel, att));
}

typedef HdfOrbit<EciTod, TimeAcsCreator> HdfOrbit_EciTod_TimeAcs;
typedef HdfOrbit<Eci, TimePgsCreator> HdfOrbit_Eci_TimePgs;
typedef HdfOrbit<Eci, TimeJ2000Creator> HdfOrbit_Eci_TimeJ2000;
typedef HdfOrbit<EciTod, TimeJ2000Creator> HdfOrbit_EciTod_TimeJ2000;
}

GEOCAL_EXPORT_KEY(HdfOrbit_EciTod_TimeAcs);
GEOCAL_EXPORT_KEY(HdfOrbit_Eci_TimePgs);
GEOCAL_EXPORT_KEY(HdfOrbit_Eci_TimeJ2000);
GEOCAL_EXPORT_KEY(HdfOrbit_EciTod_TimeJ2000);
GEOCAL_CLASS_VERSION(HdfOrbit_EciTod_TimeAcs, 2);
GEOCAL_CLASS_VERSION(HdfOrbit_Eci_TimePgs, 2);
GEOCAL_CLASS_VERSION(HdfOrbit_Eci_TimeJ2000, 2);
GEOCAL_CLASS_VERSION(HdfOrbit_EciTod_TimeJ2000, 2);
#endif
