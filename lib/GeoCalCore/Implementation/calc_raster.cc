#include "calc_raster.h"
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CalcRaster::save(Archive & ar, const unsigned int version) const
{
  int number_tile = (int) tile.size();
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageVariable)
    & GEOCAL_NVP(number_tile);
}

template<class Archive>
void CalcRaster::load(Archive & ar, const unsigned int version)
{
  int number_tile;
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageVariable)
    & GEOCAL_NVP(number_tile);
  tile.resize(number_tile);
  next_swap = tile.begin();
}

GEOCAL_SPLIT_IMPLEMENT(CalcRaster);
#endif

// See base class for description of this.
void CalcRaster::read_ptr(int Lstart, int Sstart, int Number_line, 
			       int Number_sample, int* Res) const
{
  Array<double, 2> d = read_double(Lstart, Sstart, Number_line, Number_sample);
  for(Array<double, 2>::const_iterator i = d.begin(); i != d.end(); ++i, ++Res)
    *Res = (int) *i;
}

// See base class for description of this.
void CalcRaster::unchecked_write(int Line, int Sample, int Val)
{
  throw Exception("CalcRaster doesn't support writing to.");
}

//-----------------------------------------------------------------------
/// Swap tiles so that Line, Sample are in range. This calls read_double
/// if needed.
//-----------------------------------------------------------------------

const blitz::Array<double, 2>& CalcRaster::swap(int Line, int Sample) const
{
  range_check(Line, 0, number_line());
  range_check(Sample, 0, number_sample());
  typedef Array<double, 2> arr;
  BOOST_FOREACH(arr& a, tile)
    if(a.lbound(0) <= Line &&
       Line <= a.ubound(0) &&
       a.lbound(1) <= Sample &&
       Sample <= a.ubound(1))
      return a;
  TinyVector<int, 2> start;
  start(0) = (Line / number_tile_line()) * number_tile_line();
  start(1) = (Sample / number_tile_sample()) * number_tile_sample();
  int nl = std::min(number_tile_line(), number_line() - start(0));
  int ns = std::min(number_tile_sample(), number_sample() - start(1));
  next_swap->reference(read_double(start(0), start(1), nl, ns));
  next_swap->reindexSelf(start);
  const Array<double,2>& res = *next_swap;
  ++next_swap;
  if(next_swap == tile.end())
    next_swap = tile.begin();
  return res;
}


blitz::Array<double, 2> 
CalcRaster::read_double(int Lstart, int Sstart, int Nline, 
			int Nsamp) const
{
  if(Lstart == lstart &&
     Sstart == sstart &&
     data.rows() == Nline &&
     data.cols() == Nsamp)
    return data;
  lstart = Lstart;
  sstart = Sstart;
  if(Lstart < 0 || Lstart + Nline > number_line() ||
     Sstart < 0 || Sstart + Nsamp > number_sample()) {
    Exception e;
    e << "Data out of range in CalcRaster::read_double\n"
      << "  Lstart: " << Lstart << "\n"
      << "  Sstart: " << Sstart << "\n"
      << "  Nline:  " << Nline << "\n"
      << "  Nsamp:  " << Nsamp << "\n"
      << "  Raster number line: " << number_line() << "\n"
      << "  Raster number sample: " << number_sample() << "\n"
      << "  Underlying raster image: \n" << *this << "\n";
    throw e;
  }
  data.resize(Nline, Nsamp);
  calc(Lstart, Sstart);
  return data.copy();
}
