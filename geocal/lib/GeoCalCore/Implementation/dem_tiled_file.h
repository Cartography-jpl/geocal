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
/// \param F Underlying file
/// \param D Datum to use when translating to elevation about
///    ellipsoid
/// \param M MapInfo going with file F
/// \param Outside_dem_is_error If true, we treat requests for
///   elevation outside of the MapInfo M as error. Otherwise,
///   we just return 0.
/// \param Scale Value to multiple underlying data by to get meters
///   (e.g., DEM might be in US survey foot).
//-----------------------------------------------------------------------

  DemTiledFile(const boost::shared_ptr<TiledFileBase<2> >& F,
	       const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	       bool Outside_dem_is_error = false, double Scale = 1.0)
  { initialize(F, D, M, Outside_dem_is_error, Scale); }

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
    return scale_ * data_->get_double(i);
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
/// \param F File
/// \param D Datum to use when translating to elevation about
///    ellipsoid
/// \param M MapInfo going with file F
/// \param Outside_dem_is_error If true, we treat requests for
///   elevation outside of the MapInfo M as error. Otherwise,
///   we just return 0.
/// \param Scale Value to multiple underlying data by to get meters
///   (e.g., DEM might be in US survey foot).
//-----------------------------------------------------------------------

  void initialize(const boost::shared_ptr<TiledFileBase<2> >& F,
		  const boost::shared_ptr<Datum>& D, const MapInfo& M, 
		  bool Outside_dem_is_error = false, double Scale = 1.0)
  { 
    data_ = F;
    scale_ = Scale;
    DemMapInfo::initialize(D, M, Outside_dem_is_error);
  }
  boost::shared_ptr<TiledFileBase<2> > data_; ///< Underlying data.
  double scale_;			      ///< Scale to apply to
					      ///data to get meters.
};
}
#endif
