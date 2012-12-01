#ifndef CALC_RASTER_H
#define CALC_RASTER_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  Some classes most naturally generate a RasterImage by calculating 
  a results for an array of data (e.g., a tile). This class handles
  the common behavior for these classes.
*******************************************************************/
class CalcRaster : public RasterImageVariable {
public:
  virtual ~CalcRaster() {}
  virtual int unchecked_read(int Line, int Sample) const
  {
    return (int) swap(Line,Sample)(Line,Sample);
  }
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;

//-----------------------------------------------------------------------
/// Calculate the results as a blitz::Array of doubles.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const;
  virtual void unchecked_write(int Line, int Sample, int Val);
protected:
  mutable blitz::Array<double, 2> data;

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

  CalcRaster(int Number_tile = 4) : tile(Number_tile), lstart(-1), sstart(-1)
  {
    next_swap = tile.begin();
  }

//-----------------------------------------------------------------------
/// Constructor that we give a size to. The tile size is set to the
/// full size of the image, and map_info_ is set to null.
//-----------------------------------------------------------------------

  CalcRaster(int Nline, int Nsamp, int Number_tile = 4)
    : RasterImageVariable(Nline, Nsamp), data(0,0),
      tile(Number_tile), lstart(-1), sstart(-1)
  {
    next_swap = tile.begin();
  }

//-----------------------------------------------------------------------
/// Constructor that we give a size and tile size to. 
/// map_info_ is set to null.
//-----------------------------------------------------------------------

  CalcRaster(int Nline, int Nsamp, int Ntileln, int Ntilesmp, 
	     int Number_tile = 4)
    : RasterImageVariable(Nline, Nsamp, Ntileln, Ntilesmp), data(0,0),
      tile(Number_tile), lstart(-1), sstart(-1)
  {
    next_swap = tile.begin();
  }

//-----------------------------------------------------------------------
/// Constructor that takes the given MapInfo. The tile size is set to
/// the full size of the image.
//-----------------------------------------------------------------------

  CalcRaster(const MapInfo& Mi, int Number_tile = 4)
    : RasterImageVariable(Mi), data(0,0),
      tile(Number_tile), lstart(-1), sstart(-1)
  {
    next_swap = tile.begin();
  }

//-----------------------------------------------------------------------
/// Constructor that we find the size information from the given
/// sample image. We set the tile size to the same as Img,
/// and copy the map and RPC information. 
///
/// Note that this is *not* a copy constructor, we don't copy the
/// underlying data.
//-----------------------------------------------------------------------
  CalcRaster(const RasterImage& Img, int Number_tile = 4)
    : RasterImageVariable(Img),
      data(0,0),
      tile(Number_tile), lstart(-1), sstart(-1)
  {
    number_tile_line_ = Img.number_tile_line();
    number_tile_sample_ = Img.number_tile_sample();
    next_swap = tile.begin();
  }
private:
  mutable std::vector<blitz::Array<double, 2> > tile;
  mutable std::vector<blitz::Array<double, 2> >::iterator next_swap;
  const blitz::Array<double, 2>& swap(int Line, int Sample) const;

  mutable int lstart;
  mutable int sstart;
};
}
#endif
