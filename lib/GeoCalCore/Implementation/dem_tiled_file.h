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
/// \param Nodata value. Treated like outside dem (so an error or
///   translated to 0.
//-----------------------------------------------------------------------

  DemTiledFile(const boost::shared_ptr<TiledFileBase<2> >& F,
	       const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	       bool Outside_dem_is_error = false, double Scale = 1.0,
	       double No_data_value = -1e99)
  { initialize(F, D, M, Outside_dem_is_error, Scale, No_data_value); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~DemTiledFile() {}

  virtual double elevation(int Y_index, int X_index) const;
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
/// \param Nodata value. Treated like outside dem (so an error or
///   translated to 0.
//-----------------------------------------------------------------------

  void initialize(const boost::shared_ptr<TiledFileBase<2> >& F,
		  const boost::shared_ptr<Datum>& D, const MapInfo& M, 
		  bool Outside_dem_is_error = false, double Scale = 1.0,
		  double No_data_value = -1e99)
  { 
    data_ = F;
    scale_ = Scale;
    no_data_value_ = No_data_value;
    DemMapInfo::initialize(D, M, Outside_dem_is_error);
  }
  boost::shared_ptr<TiledFileBase<2> > data_; ///< Underlying data.
  double scale_;			      ///< Scale to apply to
					      ///data to get meters.
  double no_data_value_;		      ///< No data value.
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(DemTiledFile);
#endif
