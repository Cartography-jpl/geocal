#include "raster_multifile.h"
#include <boost/foreach.hpp>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read a subset of the data.
//-----------------------------------------------------------------------

void RasterMultifile::read_ptr(int Lstart, int Sstart, int Number_line, 
				     int Number_sample, int* Res) const
{
// See if data is all in one file. If it is, then get the data from
// that file.
  const RasterMultifileTile& mt = swap(Lstart, Sstart);
  if(mt.data.get() &&
     mt.in_tile(Lstart + Number_line - 1, Sstart + Number_sample - 1)) {
    mt.data->read_ptr(Lstart - mt.line_offset, Sstart - mt.sample_offset,
		      Number_line, Number_sample, Res);
    return;
  }
  // Otherwise just read each line and sample
  for(int i = Lstart; i < Lstart + Number_line; ++i)
    for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
      *Res = (*this)(i, j);
}

//-----------------------------------------------------------------------
/// Find the RasterMultifileTile that contains a given Line and 
/// Sample.
//-----------------------------------------------------------------------

RasterMultifileTile& RasterMultifile::swap(int Line, 
						       int Sample) const
{
  range_check(Line, 0, number_line());
  range_check(Sample, 0, number_sample());
  BOOST_FOREACH(RasterMultifileTile& mt, tile)
    if(mt.in_tile(Line, Sample))
      return mt;
  mt_scratch = get_file(Line, Sample);
  if(mt_scratch.data.get()) {
    *next_swap = mt_scratch;
    ++next_swap;
    if(next_swap == tile.end())
      next_swap = tile.begin();
  }
  return mt_scratch;
}
