#include "raster_multifile.h"
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterMultifile::save(Archive & ar, const unsigned int UNUSED(version)) const
{
  int number_tile = (int) tile.size();
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageVariable)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP_(no_coverage_fill_value)
    & GEOCAL_NVP(number_tile);
}

template<class Archive>
void RasterMultifile::load(Archive & ar, const unsigned int UNUSED(version))
{
  int number_tile;
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageVariable)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP_(no_coverage_fill_value)
    & GEOCAL_NVP(number_tile);
  tile.resize(number_tile);
  next_swap = tile.begin();
}

GEOCAL_SPLIT_IMPLEMENT(RasterMultifile);
#endif

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
  BOOST_FOREACH(RasterMultifileTile& mt, tile)
    if(mt.in_tile(Line, Sample))
      return mt;
  // Avoid range check unless we need to get a new file, since in_tile
  // works fine with out of range data (just returning false)
  range_check(Line, 0, number_line());
  range_check(Sample, 0, number_sample());
  mt_scratch = get_file(Line, Sample);
  ++number_file_read_;
  if(mt_scratch.data.get()) {
    *next_swap = mt_scratch;
    ++next_swap;
    if(next_swap == tile.end())
      next_swap = tile.begin();
  }
  return mt_scratch;
}
