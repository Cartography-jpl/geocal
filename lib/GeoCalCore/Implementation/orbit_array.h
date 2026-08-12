#ifndef ORBIT_ARRAY_H
#define ORBIT_ARRAY_H
#include "orbit_quaternion_list.h"
#include "eci_tod.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This is an implementation of an Orbit that is directly given an
  arrays with time, position, velocity:

  Attitude_time  - natt in size, time of attitude
      measurement as doubles.
  Attitude_quaternion - natt x 4 in size, quaternion
      to take same coordinate system as Position (e.g., ECI)
  Ephemeris_time - neph in size, time of ephemeris
      measurement as doubles
  Ephemeris_position - neph x 3 in size, position
      measurement as doubles in meters
  Ephemeris_velocity - neph x 3 in size, velocity
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

   Derived classes may have a way to populate the data
   (e.g. HdfOrbit), so an additional field indicates if the class
   should serialize this data itself or let the derived object handle
   this (e.g. HdfOrbit stores a file name, and gets the data from that
   file).
*******************************************************************/

template<class PositionType, class TimeCreatorType> class OrbitArray : 
    public OrbitQuaternionList {
public:
  OrbitArray(const blitz::Array<double, 1>& Eph_time,
	     const blitz::Array<double, 2>& Eph_pos,
	     const blitz::Array<double, 2>& Eph_vel,
	     const blitz::Array<double, 1>& Att_time,
	     const blitz::Array<double, 2>& Att_quat,
	     bool Att_from_sc_to_ref_frame = true,
	     bool Serialize_data = true)
  { init(Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat,
	 Att_from_sc_to_ref_frame, Serialize_data);
  }
  virtual ~OrbitArray() {}

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
    Os << "OrbitArray\n";
  }
protected:
  virtual boost::shared_ptr<QuaternionOrbitData> 
  orbit_data_create(Time T) const;
  OrbitArray() {}
  void init(const blitz::Array<double, 1>& Eph_time,
	    const blitz::Array<double, 2>& Eph_pos,
	    const blitz::Array<double, 2>& Eph_vel,
	    const blitz::Array<double, 1>& Att_time,
	    const blitz::Array<double, 2>& Att_quat,
	    bool Att_from_sc_to_ref_frame = true,
	    bool Serialize_data = true);
         
  bool att_from_sc_to_ref_frame, serialize_data;
private:
  typedef std::map<Time, boost::math::quaternion<double> > 
  time_attmap;
  time_attmap att_map;
  typedef typename std::pair<PositionType, boost::array<double, 3> > posvel;
  typedef std::map<Time,  posvel> time_posmap;
  time_posmap pos_map;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};

  //-----------------------------------------------------------------------
/// Separate out initialization to make serialization a little easier
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType> inline 
void OrbitArray<PositionType, TimeCreatorType>::init
(const blitz::Array<double, 1>& Eph_time,
 const blitz::Array<double, 2>& Eph_pos,
 const blitz::Array<double, 2>& Eph_vel,
 const blitz::Array<double, 1>& Att_time,
 const blitz::Array<double, 2>& Att_quat,
 bool Att_from_sc_to_ref_frame,
 bool Serialize_data)
{
  using namespace blitz;
  TimeCreatorType tc;
  att_from_sc_to_ref_frame = Att_from_sc_to_ref_frame;
  serialize_data = Serialize_data;
  if(Eph_time.rows() != Eph_pos.rows() || 
     Eph_time.rows() != Eph_vel.rows())
    throw Exception("Need to have the same size data for time, position and velocity");
  if(Eph_pos.cols() != 3 ||
     Eph_vel.cols() != 3)
    throw Exception("Position and velocity need to have 3 columns");
  for(int i = 0; i < Eph_time.rows(); ++i) {
    Time t = tc(Eph_time(i));
    PositionType p(Eph_pos(i, 0), Eph_pos(i, 1), Eph_pos(i, 2));
    boost::array<double, 3> v = {{Eph_vel(i, 0), Eph_vel(i, 1), Eph_vel(i, 2)}};
    pos_map[t] = posvel(p, v);
  }
  if(Att_time.rows() != Att_quat.rows())
    throw Exception("Need to have the same size data for time, and attitude");
  if(Att_quat.cols() != 4)
    throw Exception("Quaternion needs to have 4 columns");
  // Do lazy evaluation of orbit data, since orbit file may be much
  // bigger than the times we are interested (e.g., a full day).
  boost::shared_ptr<QuaternionOrbitData> null;
  for(int i = 0; i < Att_time.rows(); ++i) {
    Time t = tc(Att_time(i));
    // att_map always goes from sc_to_refframe (e.g. sc_to_ci). If the
    // underlying data goes the other way, take the conjugate as we
    // read it in
    
    if(att_from_sc_to_ref_frame)
      att_map[t] = boost::math::quaternion<double>(Att_quat(i, 0), Att_quat(i, 1),
						   Att_quat(i, 2), Att_quat(i, 3));
    else
      att_map[t] = boost::math::quaternion<double>(Att_quat(i, 0), -Att_quat(i, 1),
						   -Att_quat(i, 2), -Att_quat(i, 3));
  }
  min_tm = std::max(att_map.begin()->first, pos_map.begin()->first);
  max_tm = std::min(att_map.rbegin()->first, pos_map.rbegin()->first);
  orbit_data_map[min_tm] = nullptr;
  orbit_data_map[max_tm] = nullptr;
  double tdelta = (max_tm - min_tm) / Att_time.rows();
  for(int i = 0; i < Att_time.rows(); ++i) {
    Time t = tc(Att_time(i));
    if(t > min_tm + tdelta * 0.1 && t < max_tm - tdelta * 0.1)
      orbit_data_map[t] = nullptr;
  }
}

//-----------------------------------------------------------------------
/// Do full calculation to get orbit data
//-----------------------------------------------------------------------

template<class PositionType, class TimeCreatorType>
inline boost::shared_ptr<QuaternionOrbitData> OrbitArray<PositionType, TimeCreatorType>::orbit_data_create(Time T) const
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

typedef OrbitArray<EciTod, TimeAcsCreator> OrbitArray_EciTod_TimeAcs;
typedef OrbitArray<Eci, TimePgsCreator> OrbitArray_Eci_TimePgs;
typedef OrbitArray<Eci, TimeJ2000Creator> OrbitArray_Eci_TimeJ2000;
typedef OrbitArray<EciTod, TimeJ2000Creator> OrbitArray_EciTod_TimeJ2000;
}  
GEOCAL_EXPORT_KEY(OrbitArray_EciTod_TimeAcs);
GEOCAL_EXPORT_KEY(OrbitArray_Eci_TimePgs);
GEOCAL_EXPORT_KEY(OrbitArray_Eci_TimeJ2000);
GEOCAL_EXPORT_KEY(OrbitArray_EciTod_TimeJ2000);
#endif
