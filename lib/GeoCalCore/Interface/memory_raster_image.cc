#include "memory_raster_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Copy another RasterImage into memory. This can be useful to read a
/// disk based RasterImage completely into memory if you are going to
/// be doing a lot of processing on it.
///
/// We read data in blocks of the given number of lines and sample. The 
/// default value of -1 means we read the data in the tile size of the 
/// image. 
//-----------------------------------------------------------------------

MemoryRasterImage::MemoryRasterImage(const RasterImage& Img, 
				     int Number_line_to_read, 
				     int Number_sample_to_read)
: RasterImageVariable(Img),
  data_(boost::extents[Img.number_line()][Img.number_sample()])
{
  if(Number_line_to_read == -1)
    Number_line_to_read = Img.number_tile_line();
  if(Number_sample_to_read == -1)
    Number_sample_to_read = Img.number_tile_sample();
  typedef boost::multi_array<int, 2>::index_range range;
  for(int i = 0; i < Img.number_line(); i+= Number_line_to_read) {
    for(int j = 0; j < Img.number_sample(); j+= Number_sample_to_read) {
      int nline = Number_line_to_read;
      if(i + nline >= Img.number_line())
	nline = Img.number_line() - i;
      int nsamp = Number_sample_to_read;
      if(j + nsamp >= Img.number_sample())
	nsamp = Img.number_sample() - j;
#ifdef DIAGNOSTIC
      std::cerr << "Memory raster image reading (" << i << ", " << j 
		<< ") to (" << i + nline << ", " << j + nsamp << ")\n";
#endif
      data_[boost::indices[range(i, i + nline)][range(j, j + nsamp)]]
	= Img.read_array(i, j, nline, nsamp);
    }
  }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void MemoryRasterImage::print(std::ostream& Os) const
{
  Os << "RasterImage of " << number_line() << " x " << number_sample() 
     << " all in memory\n";
}

