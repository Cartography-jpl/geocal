#include "calc_raster_multi_band.h"
#include "calc_raster.h"

using namespace GeoCal;
using namespace blitz;

// Suppress Doxgyen documentation
/// @cond

class RasterImageWrap: public CalcRaster {
public:
  RasterImageWrap(const CalcRasterMultiBand& Mband,
		  const RasterImage& Ri,
		  int Number_tile_line,
		  int Number_tile_sample,
		  int Number_tile,
		  int Band)
    : CalcRaster(Ri, Number_tile), mband(Mband), band(Band) 
  {
    number_tile_line_ = Number_tile_line;
    number_tile_sample_ = Number_tile_sample;
  }
  RasterImageWrap(const CalcRasterMultiBand& Mband,
		  int Nline, int Nsamp,
		  int Number_tile_line,
		  int Number_tile_sample,
		  int Number_tile,
		  int Band)
    : CalcRaster(Nline, Nsamp, Number_tile), mband(Mband), band(Band) 
  {
    number_tile_line_ = Number_tile_line;
    number_tile_sample_ = Number_tile_sample;
  }
  RasterImageWrap(const CalcRasterMultiBand& Mband,
		  const MapInfo& Mi,
		  int Number_tile_line,
		  int Number_tile_sample,
		  int Number_tile,
		  int Band)
    : CalcRaster(Mi, Number_tile), mband(Mband), band(Band) 
  {
    number_tile_line_ = Number_tile_line;
    number_tile_sample_ = Number_tile_sample;
  }
protected:
  void calc(int Lstart, int Sstart) const
  {
    data = mband.read_double(Lstart, Sstart, data.rows(), data.cols())
      (band, Range::all(), Range::all());
  }
private:
  const CalcRasterMultiBand& mband;
  int band;
};

/// @endcond

// See base class for description
Array<double, 3> CalcRasterMultiBand::read_double(int Lstart, int Sstart, 
						  int Nline, int Nsamp) const
{
  if(Lstart == lstart &&
     Sstart == sstart &&
     data.cols() ==Nline &&
     data.depth() == Nsamp)
    return data;
  lstart = Lstart;
  sstart = Sstart;
  if(Lstart < 0 || Lstart + Nline > raster_image(0).number_line() ||
     Sstart < 0 || Sstart + Nsamp > raster_image(0).number_sample())
    throw Exception("Data out of range in CalcRasterMultiBand::read_double");
  data.resize(number_band(), Nline, Nsamp);
  calc(Lstart, Sstart);
  return data;
}

//-----------------------------------------------------------------------
/// Initialize data. This uses the given tile size if supplied, or it
/// gets the tile size from the Img it not.
//-----------------------------------------------------------------------

void CalcRasterMultiBand::initialize(
const RasterImage& Img, int Nband, int Number_tile_line,
int Number_tile_sample, int Number_tile)
{
  lstart = -1;
  sstart = -1;
  if(Number_tile_line == -1)
    Number_tile_line = Img.number_tile_line();
  if(Number_tile_sample == -1)
    Number_tile_sample = Img.number_tile_sample();
  for(int i = 0; i < Nband; ++i)
    add_raster_image(boost::shared_ptr<RasterImage>(new 
    RasterImageWrap(*this, Img, Number_tile_line, Number_tile_sample, 
		    Number_tile, i)));
}

void CalcRasterMultiBand::initialize(
int Nline, int Nsamp, int Nband, int Number_tile_line,
int Number_tile_sample, int Number_tile)
{
  lstart = -1;
  sstart = -1;
  if(Number_tile_line == -1)
    Number_tile_line = Nline;
  if(Number_tile_sample == -1)
    Number_tile_sample = Nsamp;
  for(int i = 0; i < Nband; ++i)
    add_raster_image(boost::shared_ptr<RasterImage>(new 
    RasterImageWrap(*this, Nline, Nsamp, Number_tile_line, Number_tile_sample, 
		    Number_tile, i)));
}

void CalcRasterMultiBand::initialize(
const MapInfo& Mi, int Nband, int Number_tile_line,
int Number_tile_sample, int Number_tile)
{
  lstart = -1;
  sstart = -1;
  if(Number_tile_line == -1)
    Number_tile_line = Mi.number_y_pixel();
  if(Number_tile_sample == -1)
    Number_tile_sample = Mi.number_x_pixel();
  for(int i = 0; i < Nband; ++i)
    add_raster_image(boost::shared_ptr<RasterImage>(new 
    RasterImageWrap(*this, Mi, Number_tile_line, Number_tile_sample, 
		    Number_tile, i)));
}


