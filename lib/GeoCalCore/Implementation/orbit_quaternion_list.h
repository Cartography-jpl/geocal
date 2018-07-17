#ifndef ORBIT_QUATERNION_LIST_H
#define ORBIT_QUATERNION_LIST_H
#include "orbit.h"
#include "time_table.h"
#include <map>

namespace GeoCal {
  class OrbitQuaternionListOffset; // Forward Declaration.
  
/****************************************************************//**
  This is an implementation of an Orbit that is a list of
  QuaternionOrbitData values. For times that fall between these
  values, we interpolate to get the OrbitData.

  Note that for some classes the calculation of the full list of
  QuaternionOrbitData might not be needed, for example an Orbit file
  covering a full day of which we are only using a subset of the
  data. To support this, we allow a lazy evaluation of the
  QuaternionOrbitData. It can initially be supplied as a null
  boost::shared_ptr, and when we encounter a null we call the function 
  orbit_data_create. This function should be overriden by a derived
  class to supply the calculation of a QuaternionOrbitData for a
  particular time on demand.
*******************************************************************/

class OrbitQuaternionList : public Orbit {
public:
//-----------------------------------------------------------------------
/// Constructor that takes a list of QuaternionOrbitData values. Note
/// that the data doesn't need to be sorted, we handle sorting as we
/// ingest the data.
///
/// QuaternionOrbitData pointer can be null if we want to do a lazy
/// evaluation of the data (see description of class for details).
//-----------------------------------------------------------------------

  OrbitQuaternionList(const 
       std::vector<boost::shared_ptr<QuaternionOrbitData> >& Data)
  { initialize(Data.begin(), Data.end()); }

//-----------------------------------------------------------------------
/// Constructor that takes a list of QuaternionOrbitData values. Note
/// that the data doesn't need to be sorted, we handle sorting as we
/// ingest the data.
///
/// QuaternionOrbitData pointer can be null if we want to do a lazy
/// evaluation of the data (see description of class for details).
//-----------------------------------------------------------------------

  template<class iterator>
  OrbitQuaternionList(iterator ibeg, iterator iend)
  { initialize(ibeg, iend); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OrbitQuaternionList() {}

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;

  std::vector<boost::shared_ptr<QuaternionOrbitData> > 
  quaternion_orbit_data() const;
  // Specialization of sc_look_vector, which turns out to be a bottle
  // neck in the Ipi code
  using Orbit::sc_look_vector; // Only override one of these functions
  virtual ScLookVector sc_look_vector(Time T, 
				      const CartesianFixed& Pt) const;
  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const;
protected:
  friend class OrbitQuaternionListOffset;
  virtual void interpolate_or_extrapolate_data
  (Time T, boost::shared_ptr<GeoCal::QuaternionOrbitData>& Q1,
   boost::shared_ptr<GeoCal::QuaternionOrbitData>& Q2) const;
  
//-----------------------------------------------------------------------
/// Derived classes can override this to create orbit data on demand
/// to support lazy evaluation.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<QuaternionOrbitData> orbit_data_create(Time T) const
  {
    // Derived classes should fill this in
    throw Exception("orbit_data_create not implemented");
  }

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
  mutable time_map orbit_data_map;

//-----------------------------------------------------------------------
/// Check a iterator to see if we need to do a lazy evalutation
//-----------------------------------------------------------------------
  void check_lazy_evaluation(time_map::iterator i) const
  {
    if(!i->second)
      i->second = orbit_data_create(i->first);
  }

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

/****************************************************************//**
  Some orbits are expensive to calculate (e.g., SpicePlanetOrbit).
  Often it is perfectly fine to only calculate this at some fixed set
  of times and then interpolate, which can be significantly faster.
  This class supports this.
*******************************************************************/

class OrbitListCache: public OrbitQuaternionList, public Observer<Orbit> {
public:
//-----------------------------------------------------------------------
/// Create a OrbitListCache that calculates the orbit at each time
/// found for image coordinate line from Tt.min_line() to
/// tt.max_line() for the given sample. We cache the orbit data. If
/// the underlying orbit changes, we throw away the cached values.
//-----------------------------------------------------------------------

  OrbitListCache(const boost::shared_ptr<Orbit>& Orbit_underlying,
		 const boost::shared_ptr<TimeTable>& Tt,
		 double Sample = 0.0)
    : orbit_underlying_(Orbit_underlying), tt(Tt), sample_(Sample)
  { init(); }

  virtual ~OrbitListCache() {};
  virtual void notify_update(const Orbit& Orb);
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Underlying orbit.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Orbit>& orbit_underlying() const
  {return orbit_underlying_;}

//-----------------------------------------------------------------------
/// Time table we sample orbit at.
//-----------------------------------------------------------------------

  const boost::shared_ptr<TimeTable>& time_table() const
  { return tt;}
  
//-----------------------------------------------------------------------
/// Sample we use with time table.
//-----------------------------------------------------------------------
  double sample() const { return sample_; }
protected:
  virtual boost::shared_ptr<QuaternionOrbitData> orbit_data_create(Time T)
    const;
  OrbitListCache() {}
private:
  void init();
  boost::shared_ptr<Orbit> orbit_underlying_;
  boost::shared_ptr<TimeTable> tt;
  double sample_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitListCache);
#endif
