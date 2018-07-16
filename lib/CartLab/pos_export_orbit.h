#ifndef POS_EXPORT_ORBIT_H
#define POS_EXPORT_ORBIT_H
#include "orbit_quaternion_list.h"
#include "aircraft_orbit_data.h"

namespace GeoCal {
/****************************************************************//**
  This read a POS export file to supply an Orbit. This is a text file
  that is written by the Applanix POS Export Utility.

  Note that the time in the POS export file is in GPS second of the
  week. This starts at midnight UTC time between Saturday and
  Sunday. Presumably if we happen to be flying at this time, the time
  will reset. We don't have any handling in place for this, we'll need
  to add handling if this ever becomes an issue.
*******************************************************************/
class PosExportOrbit :  public OrbitQuaternionList {
public:
  PosExportOrbit(const std::string& Fname, const Time& Epoch);
  virtual ~PosExportOrbit() {}
  const AircraftOrbitData& aircraft_orbit_data(const Time& T) const;
  const std::string& file_name() const {return fname;}
  const Time& file_epoch() const {return file_epoch_;}
protected:
  PosExportOrbit() {}
private:
  void init(const std::string& Fname, const Time& Epoch);
  void process_line(const Time& Epoch, const std::string& ln,
		    Geodetic& Last_point, Time& Last_tm);
  std::string fname;
  Time file_epoch_;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}
GEOCAL_EXPORT_KEY(PosExportOrbit);
#endif
