#ifndef ORBIT_QUATERNION_LIST_H
#define ORBIT_QUATERNION_LIST_H
#include "orbit.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This is an implementation of an Orbit that is a list of
  QuaternionOrbitData values. For times that fall between these
  values, we interpolate to get the OrbitData.
*******************************************************************/

class OrbitQuaternionList : public Orbit {
public:
//-----------------------------------------------------------------------
/// Constructor that takes a list of QuaternionOrbitData values. Note
/// that the data doesn't need to be sorted, we handle sorting as we
/// ingest the data.
//-----------------------------------------------------------------------

  OrbitQuaternionList(const 
       std::vector<boost::shared_ptr<QuaternionOrbitData> >& Data)
  { initialize(Data.begin(), Data.end()); }

//-----------------------------------------------------------------------
/// Constructor that takes a list of QuaternionOrbitData values. Note
/// that the data doesn't need to be sorted, we handle sorting as we
/// ingest the data.
//-----------------------------------------------------------------------

  template<class iterator>
  OrbitQuaternionList(iterator ibeg, iterator iend)
  { initialize(ibeg, iend); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OrbitQuaternionList() {}

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual void print(std::ostream& Os) const;

  std::vector<boost::shared_ptr<QuaternionOrbitData> > 
  quaternion_orbit_data() const;
protected:
//-----------------------------------------------------------------------
/// Default Constructor. Derived classes should call initialize.
//-----------------------------------------------------------------------

  OrbitQuaternionList() {}

//-----------------------------------------------------------------------
/// If derived classes have filled in the orbit_data_map map, then can
/// call this function to finish the class initialization.
//-----------------------------------------------------------------------

  void initialize()
  {
    min_tm = orbit_data_map.begin()->first;
    max_tm = orbit_data_map.rbegin()->first;
  }

  typedef std::map<Time, boost::shared_ptr<QuaternionOrbitData> > 
  time_map;
  time_map orbit_data_map;

//-----------------------------------------------------------------------
/// This version of initialize fills in orbit_data_map and then does
/// the parent class initialization. 
//-----------------------------------------------------------------------

  template<class iterator> void initialize(iterator ibeg, iterator iend)
  {
    for(iterator i = ibeg; i != iend; ++i)
      orbit_data_map[(*i)->time()] = *i;
    initialize();
  }
};
}
#endif
