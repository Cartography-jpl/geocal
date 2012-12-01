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

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~DidDatum() {}
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "DID Datum\n"; }

  const std::string& file_name() const {return msl_.file_name();}
private:
  MapInfo mi_;			///< MapInfo describing how data is
				///stored.
  MemoryMapArray<char, 3> msl_;
                                ///< Data. We read this as raw bytes
                                ///so we don't have any endian problems.
};

}
#endif
