#ifndef DEM_TILED_FILE_H
#define DEM_TILED_FILE_H
#include "dem_map_info.h"
#include "tiled_file.h"

namespace GeoCal {
/****************************************************************//**
  This is a DemMapInfo that is implemented as a TiledFile<T, 2>
*******************************************************************/

class DemTiledFile : public DemMapInfo {
public:
  typedef TiledFileBase<2>::index index;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  DemTiledFile(const boost::shared_ptr<TiledFileBase<2> >& F,
	       const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	       bool Outside_dem_is_error = false)
  { initialize(F, D, M, Outside_dem_is_error); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~DemTiledFile() {}

//-----------------------------------------------------------------------
/// Return height in meters relative to datum(). Note that the call is
/// in line, sample order, which means Y and then X.
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  {
    boost::array<index, 2> i = {{Y_index, X_index}};
    return data_->get_double(i);
  }

  unsigned int number_tile() const {return data_->number_tile();}
  int number_line_per_tile() const {return (int) data_->tile_size()[0];}
  int number_sample_per_tile() const {return (int) data_->tile_size()[1];}
  void reset_number_swap() { data_->reset_number_swap();}
  int number_swap() const {return data_->number_swap();}
protected:
//-----------------------------------------------------------------------
/// Default constructor. Derived classes should call initialize before
/// finishing their constructor.
//-----------------------------------------------------------------------

  DemTiledFile() {}

//-----------------------------------------------------------------------
/// Initialize data.
//-----------------------------------------------------------------------

  void initialize(const boost::shared_ptr<TiledFileBase<2> >& F,
		  const boost::shared_ptr<Datum>& D, const MapInfo& M, 
		  bool Outside_dem_is_error = false)
  { 
    data_ = F;
    DemMapInfo::initialize(D, M, Outside_dem_is_error);
  }
  boost::shared_ptr<TiledFileBase<2> > data_; ///< Underlying data.
};
}
#endif
