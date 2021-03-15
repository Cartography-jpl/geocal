#ifndef HDF_ORBIT_H
#define HDF_ORBIT_H
#include "orbit_array.h"
#include "hdf_file.h"

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
    public OrbitArray<PositionType, TimeCreatorType> {
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
      att_quat(Att_quat)
  {
    OrbitArray<PositionType, TimeCreatorType>::att_from_sc_to_ref_frame = Att_from_sc_to_ref_frame;
    init();
  }
  virtual ~HdfOrbit() {}

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
    Os << "HdfOrbit\n"
       << "  File name: " << file_name() << "\n"
       << "  Min time:      " << OrbitArray<PositionType, TimeCreatorType>::min_time() << "\n"
       << "  Max time:      " << OrbitArray<PositionType, TimeCreatorType>::max_time() << "\n";
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
  HdfOrbit() {}
private:
  // Separate out initialization to make serialization a little easier
  void init();
  std::string fname;
  std::string bgroup, eph_time, eph_pos, eph_vel, att_time, att_quat;
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
  HdfFile f(fname);
  Array<double, 1> tdouble = 
    f.read_field<double, 1>(bgroup + eph_time);
  Array<double, 2> pos = 
    f.read_field<double, 2>(bgroup + eph_pos);
  Array<double, 2> vel = 
    f.read_field<double, 2>(bgroup + eph_vel);
  Array<double, 1> tdouble2 = 
    f.read_field<double, 1>(bgroup + att_time);
  Array<double, 2> quat = 
    f.read_field<double, 2>(bgroup + att_quat);
  OrbitArray<PositionType, TimeCreatorType>::init(tdouble,
     pos, vel, tdouble2, quat,
     OrbitArray<PositionType, TimeCreatorType>::att_from_sc_to_ref_frame,
     false);
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
GEOCAL_CLASS_VERSION(HdfOrbit_EciTod_TimeAcs, 3);
GEOCAL_CLASS_VERSION(HdfOrbit_Eci_TimePgs, 3);
GEOCAL_CLASS_VERSION(HdfOrbit_Eci_TimeJ2000, 3);
GEOCAL_CLASS_VERSION(HdfOrbit_EciTod_TimeJ2000, 3);
#endif
