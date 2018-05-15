#ifndef ORBIT_QUATERNION_LIST_OFFSET_H
#define ORBIT_QUATERNION_LIST_OFFSET_H
#include "orbit_quaternion_list.h"

namespace GeoCal {
/****************************************************************//**
  This is an adapter to a OrbitQuaternionList where we add a fixed
  offset in spacecraft coordinates to the position. The original use
  of this is to model Ecostress, where we are far enough away from the
  center of the ISS that we need to account for the difference (we are
  about 25 meters or so away, which is a significant fraction of a
  pixel away). Note that this is similar to but distinct from a
  position offset in OrbitOffsetCorrection. There the correction is
  time dependent, and represented in whatever the native position is
  (e.g., ECI coordinates). Here, the correction is a know constant and
  is expressed in spacecraft coordinate - we have a fixed known
  orientation. Note that for most spacecraft we can ignore the
  position difference, the ISS is a special case because it is so
  large.

  We could have a general "Orbit" adapter, however again our use case
  is a OrbitQuaternionList (or more specifically, a HdfOrbit). We just
  use this particular case for performance reasons, overriding the
  cached orbit_data_create rather than doing the correction every time
  we create a orbit data.
*******************************************************************/

class OrbitQuaternionListOffset : public OrbitQuaternionList {
public:
//-----------------------------------------------------------------------
/// Constructor. Add a fixed offset to the position in meters, in
/// space craft coordinate system.
//-----------------------------------------------------------------------
  
  OrbitQuaternionListOffset(const boost::shared_ptr<OrbitQuaternionList>&
      Orbit_underlying, const blitz::Array<double, 1>&
      Position_offset_sc_coordinate)
    : orbit_underlying_(Orbit_underlying),
      pos_off_(Position_offset_sc_coordinate)
  { init(); }
  virtual ~OrbitQuaternionListOffset() {}

//-----------------------------------------------------------------------
/// Return the base orbit we are applying the offset to.
//-----------------------------------------------------------------------
  
  boost::shared_ptr<OrbitQuaternionList> orbit_underlying() const
  { return boost::dynamic_pointer_cast<OrbitQuaternionList>(orbit_underlying_); }

//-----------------------------------------------------------------------
/// Return the position offset, in the spacecraft coordinate system,
/// in meters.
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& position_offset_sc_coordinate() const
  { return pos_off_; }

  virtual void print(std::ostream& Os) const;
protected:
  virtual boost::shared_ptr<QuaternionOrbitData> orbit_data_create(Time T)
    const;
private:
  // These needs to be a Orbit for boost serialization, however it is
  // is actually of the type OrbitQuaternionList.
  boost::shared_ptr<Orbit> orbit_underlying_;
  blitz::Array<double, 1> pos_off_;
  void init();
  OrbitQuaternionListOffset() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitQuaternionListOffset);
#endif
