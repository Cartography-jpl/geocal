#ifndef DEM_MAP_INFO_OFFSET_H
#define DEM_MAP_INFO_OFFSET_H
#include "dem_map_info.h"

namespace GeoCal {
/****************************************************************//**
  This applies a fixed height offset to a DemMapInfo.
*******************************************************************/

class DemMapInfoOffset : public DemMapInfo {
public:
//-----------------------------------------------------------------------
/// Constructor. Height_offset should be in meters, and is added to
/// the underlying DEM.
//-----------------------------------------------------------------------

  DemMapInfoOffset(const boost::shared_ptr<DemMapInfo>& Dem_underlying,
		   double Height_offset)
  { initialize(Dem_underlying, Height_offset); }

//-----------------------------------------------------------------------
/// Underlying DEM.
//-----------------------------------------------------------------------

  const boost::shared_ptr<DemMapInfo>& dem_underlying() const
  { return dem_; }

//-----------------------------------------------------------------------
/// Height offset in meters added to underlying DEM.
//-----------------------------------------------------------------------

  double height_offset() const {return height_offset_;}

  virtual void print(std::ostream& Os) const
  {
    Os << "DemMapInfoOffset\n"
       << "  Height offset: " << height_offset_ << "\n"
       << "  Underlying DEM: \n" << *dem_ << "\n";
  }
protected:
  virtual double elevation(int Y_index, int X_index) const
  { return dem_->elevation(Y_index, X_index) + height_offset_; }
private:
  void initialize(const boost::shared_ptr<DemMapInfo>& Dem_underlying,
		   double Height_offset)
  {
    DemMapInfo::initialize(Dem_underlying->datum_ptr(), 
			   Dem_underlying->map_info(),
			   Dem_underlying->outside_dem_is_error());
    dem_ = Dem_underlying;
    height_offset_ = Height_offset;
  }
  boost::shared_ptr<DemMapInfo> dem_;
  double height_offset_;
  DemMapInfoOffset() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& Ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(DemMapInfoOffset)
#endif
