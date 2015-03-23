#ifndef CALC_RASTER_MULTI_BAND_H
#define CALC_RASTER_MULTI_BAND_H
#include "raster_image_multi_band_variable.h"

namespace GeoCal {
/****************************************************************//**
  Some classes most naturally generate a RasterImageMultiBand by
  calculating a results for an array of data (e.g., a tile). This
  class handles the common behavior for these classes.

  This is very similar to RasterImageMultiBand made upf of CalcRaster,
  the difference is that for this class it makes sense to calculate
  all the bands at once.
*******************************************************************/
class CalcRasterMultiBand: public RasterImageMultiBandVariable {
public:
  virtual blitz::Array<double, 3> read_double(int Lstart, int Sstart, 
					      int Nline, int Nsamp) const;
protected:
  blitz::Array<double, 3>& data_ptr() { return data; }
  mutable blitz::Array<double, 3> data;

//-----------------------------------------------------------------------
/// Calculate data for given array. Data will be written in data
/// member variable, which has already been resize to the correct
/// dimension.
//-----------------------------------------------------------------------

  virtual void calc(int Lstart, int Sstart) const = 0; 

//-----------------------------------------------------------------------
/// Default constructor, derived classes should fill in protected
/// variables. 
//-----------------------------------------------------------------------

  CalcRasterMultiBand() {}

//-----------------------------------------------------------------------
/// Constructor that we find the size information from the given
/// sample image. We set the tile size to the given tile size, or to
/// the tile size Img and copy the map and RPC information. 
///
/// Note that this is *not* a copy constructor, we don't copy the
/// underlying data.
//-----------------------------------------------------------------------

  CalcRasterMultiBand(const RasterImage& Img, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4)
  {
    initialize(Img, Nband, Number_tile_line, Number_tile_sample,
	       Number_tile);
  }      

  CalcRasterMultiBand(int Nline, int Nsamp, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4)
  {
    initialize(Nline, Nsamp, Nband, Number_tile_line, Number_tile_sample,
	       Number_tile);
  }

  CalcRasterMultiBand(const MapInfo& Mi, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4)
  {
    initialize(Mi, Nband, Number_tile_line, Number_tile_sample, Number_tile);
  }

  void initialize(const RasterImage& Img, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
  void initialize(int Nline, int Nsamp, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
  void initialize(const MapInfo&, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
private:
  mutable int lstart;
  mutable int sstart;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}
GEOCAL_EXPORT_KEY(CalcRasterMultiBand);
#endif
