#ifndef DID_DATUM_H
#define DID_DATUM_H
#include "geocal_datum.h"
#include "map_info.h"
#include "memory_map_array.h"

namespace GeoCal {
/****************************************************************//**
  This is a Datum used for the the DID.
*******************************************************************/

class DidDatum : public Datum {
public:
  DidDatum(const std::string& Fname);
  virtual double undulation(const GroundCoordinate& Gc) const;
  virtual double undulation(const Geodetic& Gc) const;

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~DidDatum() {}
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "DID Datum\n"; }

  const std::string& file_name() const {return msl_->file_name();}
private:
  void initialize(const std::string& Fname);
  MapInfo mi_;			///< MapInfo describing how data is
				///stored.
  boost::shared_ptr<MemoryMapArray<char, 3> > msl_;
                                ///< Data. We read this as raw bytes
                                ///so we don't have any endian problems.
#ifdef USE_BOOST_SERIALIZATON
  DidDatum() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(Datum);
    std::string fname = file_name();
    ar << BOOST_SERIALIZATION_NVP(fname);
  }
  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(Datum);
    std::string fname;
    ar >> BOOST_SERIALIZATION_NVP(fname);
    initialize(fname);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER();
#endif
};

}
#endif
