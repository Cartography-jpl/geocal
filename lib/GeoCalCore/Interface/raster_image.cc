#include "raster_image.h"
#include <iostream>
#include <boost/progress.hpp>
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterImage::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImage);
}

template<class Archive>
void ArrayRasterImage::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(ArrayRasterImage);
  ar & GEOCAL_NVP_(img);
}

GEOCAL_IMPLEMENT(RasterImage);
GEOCAL_IMPLEMENT(ArrayRasterImage);
#endif

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

RasterImageTileIterator::RasterImageTileIterator(const RasterImage& Ri)
  : ri(Ri), istart_(0), jstart_(0), 
    iend_(std::min(Ri.number_tile_line(), Ri.number_line())),
    jend_(std::min(Ri.number_tile_sample(), Ri.number_sample())),
    ntl(Ri.number_tile_line()),
    nts(Ri.number_tile_sample())
{
}

//-----------------------------------------------------------------------
/// Constructor, where we supply a tile size to use (which may be
/// different than Ri's tile size).
//-----------------------------------------------------------------------

RasterImageTileIterator::RasterImageTileIterator(const RasterImage& Ri,
						 int Number_tile_line,
						 int Number_tile_sample)
  : ri(Ri), istart_(0), jstart_(0), 
    iend_(std::min(Number_tile_line, Ri.number_line())),
    jend_(std::min(Number_tile_sample, Ri.number_sample())),
    ntl(Number_tile_line),
    nts(Number_tile_sample)
{
  range_min_check(Number_tile_line, 1);
  range_min_check(Number_tile_sample, 1);
}

RasterImageTileIterator& RasterImageTileIterator::operator++()
{
  if(end())
    return *this;
  if(jend() == ri.number_sample()) {
    istart_ = std::min(istart_ + ntl, ri.number_line());
    iend_ = std::min(istart() + ntl, ri.number_line());
    jstart_ = 0;
    jend_ = std::min(jstart() + nts, ri.number_sample());
  } else {
    jstart_ += nts;
    jend_ = std::min(jstart() + nts, ri.number_sample());
  }
  return *this;
}

//-----------------------------------------------------------------------
/// Interpolate a region, starting with the given fractional line and
/// sample. This is a bilinear interpolation. This does padding with
/// the given fill value.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RasterImage::interpolate(double Line, double Sample, 
	 int Number_line, int Number_sample, double Fill_value) const
{
  int lint = (int) floor(Line);
  int sint = (int) floor(Sample);
  double lfrac = Line - lint;
  double sfrac = Sample - sint;
  blitz::Array<double, 2> data = 
    read_double_with_pad(lint, sint, Number_line + 1, 
			 Number_sample + 1, Fill_value);
  blitz::Array<double, 2> res(Number_line, Number_sample);
  blitz::Range r1(0, Number_line - 1);
  blitz::Range r1p1(1, Number_line);
  blitz::Range r2(0, Number_sample - 1);
  blitz::Range r2p1(1, Number_sample);
  res = (1.0 - lfrac - sfrac + lfrac * sfrac) * data(r1, r2) +
    (sfrac - lfrac * sfrac) * data(r1, r2p1) +
    (lfrac - lfrac * sfrac) * data(r1p1, r2) +
    (lfrac * sfrac) * data(r1p1, r2p1);
  return res;
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

boost::multi_array<int, 2>
RasterImage::read_array(int Lstart, int Sstart, int Number_line, 
			int Number_sample) const 
{
  boost::multi_array<int, 2> res(boost::extents[Number_line][Number_sample]);
  read_ptr(Lstart, Sstart, Number_line, Number_sample, res.data());
  return res;
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

blitz::Array<int, 2>
RasterImage::read(int Lstart, int Sstart, int Number_line, 
			int Number_sample) const
{
  blitz::Array<int, 2> res(Number_line, Number_sample);
  read_ptr(Lstart, Sstart, Number_line, Number_sample, res.data());
  return res;
}


//-----------------------------------------------------------------------
/// Return a subset of the image. This variation of read allows the
/// data read to extend past the actual RasterImage (e.g., negative
/// start line). In this case, we return the given fill value for the
/// outside area.
//-----------------------------------------------------------------------

blitz::Array<int, 2>
RasterImage::read_with_pad(int Lstart, int Sstart, int Number_line, 
			   int Number_sample, int Fill_value) const
{
  range_min_check(Number_line, 1);
  range_min_check(Number_sample, 1);
  blitz::Array<int, 2> res(Number_line, Number_sample);
  res = Fill_value;
  if(Lstart >= number_line() ||
     Sstart >= number_sample() ||
     Lstart + Number_line <= 0 ||
     Sstart + Number_sample <= 0)
    return res;
  int lind = 0;
  int rline = Lstart;
  int sind = 0;
  int rsamp = Sstart;
  int nlind = Number_line;
  int nsind = Number_sample;
  if(Lstart < 0) {
    lind = -Lstart;
    nlind -= lind;
    rline = 0;
  }
  if(Sstart < 0) {
    sind = -Sstart;
    nsind -= sind;
    rsamp = 0;
  }
  if(nlind > number_line() - rline)
    nlind = number_line() - rline;
  if(nsind > number_sample() - rsamp)
    nsind = number_sample() - rsamp;
  res(blitz::Range(lind, lind + nlind-1), blitz::Range(sind, sind + nsind-1)) =
    read(rline, rsamp, nlind, nsind);
  return res;
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

blitz::Array<int, 2>
RasterImage::read(const RasterImageTileIterator& Ti) const
{
  return read(Ti.istart(), Ti.jstart(), Ti.number_line(),
		    Ti.number_sample()); 
}

//-----------------------------------------------------------------------
// Write a subset of data, coming from a blitz::Array.
//-----------------------------------------------------------------------

void RasterImage::write(int Lstart, int Sstart, const blitz::Array<int, 2>& A) 
{
  Array<int, 2> data;
  if(A.isStorageContiguous() &&
     A.ordering(0) == 1 &&
     A.ordering(1) == 2 &&
     A.isRankStoredAscending(0) &&
     A.isRankStoredAscending(1))
    data.reference(A);
  else {
    data.reference(Array<int, 2>(A.shape()));
    data = A;
  }
  write_ptr(Lstart, Sstart, A.rows(), A.cols(), A.dataFirst());
}

void RasterImage::write(int Lstart, int Sstart, const blitz::Array<double, 2>& A) 
{
  Array<double, 2> data;
  if(A.isStorageContiguous() &&
     A.ordering(0) == 1 &&
     A.ordering(1) == 2 &&
     A.isRankStoredAscending(0) &&
     A.isRankStoredAscending(1))
    data.reference(A);
  else {
    data.reference(Array<double, 2>(A.shape()));
    data = A;
  }
  write_ptr(Lstart, Sstart, A.rows(), A.cols(), A.dataFirst());
}

//-----------------------------------------------------------------------
/// This calculates the grid resolution in meters for the center of
/// the grid in the line direction. This is just the distance between
/// The center pixel number_line() / 2, number_sample() / 2 and the
/// pixel number_line() / 2 + 1, number_sample() / 2.
//-----------------------------------------------------------------------

double RasterImage::grid_center_line_resolution() const
{
  ImageCoordinate ic(number_line() / 2, number_sample() / 2);
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(ic);
  ic.line += 1;
  boost::shared_ptr<GroundCoordinate> gc2 = ground_coordinate(ic);
  return distance(*gc1, *gc2);
}

//-----------------------------------------------------------------------
/// This calculates the grid resolution in meters for the center of
/// the grid in the sample direction. This is just the distance between
/// The center pixel number_line() / 2, number_sample() / 2 and the
/// pixel number_line() / 2, number_sample() / 2 + 1.
//-----------------------------------------------------------------------

double RasterImage::grid_center_sample_resolution() const
{
  ImageCoordinate ic(number_line() / 2, number_sample() / 2);
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(ic);
  ic.sample += 1;
  boost::shared_ptr<GroundCoordinate> gc2 = ground_coordinate(ic);
  return distance(*gc1, *gc2);
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// This copies one image to another. The images should be the same
/// size. Setting Diagnostic to true causes messages to be printed as
/// we do the copying.
//-----------------------------------------------------------------------

void GeoCal::copy(const RasterImage& Img_in, RasterImage& Img_out, bool Diagnostic, int Tile_nline, int Tile_nsamp)
{
  if(Img_out.number_line() != Img_in.number_line() ||
     Img_out.number_sample() != Img_in.number_sample())
    throw Exception("Images need to be the same size");
  int tnl = std::min(Img_in.number_tile_line(), Img_out.number_tile_line());
  int tns = std::min(Img_in.number_tile_sample(), Img_out.number_tile_sample());
  if(Tile_nline > 0)
    tnl = Tile_nline;
  if(Tile_nsamp > 0)
    tns = Tile_nsamp;
  int ntile = int(ceil(double(Img_out.number_line()) / tnl) *
		  ceil(double(Img_out.number_sample()) / tns));
  boost::shared_ptr<boost::progress_display> dp;
  if(Diagnostic)
    dp = boost::make_shared<boost::progress_display>(ntile);
  std::vector<int> buf;
  if(!Img_in.copy_needs_double())
    buf.resize(tnl * tns);
  for(int istart = 0; istart < Img_out.number_line();
      istart += tnl)
    for(int jstart = 0; jstart < Img_out.number_sample();
	jstart += tns) {
      if(dp)
	++(*dp);
      int nl = std::min(tnl, Img_out.number_line() - istart);
      int ns = std::min(tns, Img_out.number_sample() - jstart);
      if(Img_in.copy_needs_double())
	Img_out.write(istart, jstart,
		      Img_in.read_double(istart, jstart, nl, ns));
      else {
	Img_in.read_ptr(istart, jstart, nl, ns, &(*buf.begin()));
	Img_out.write_ptr(istart, jstart, nl, ns, &(*buf.begin()));
      }
    }
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// This copies one image to another. The images should be the same
/// size. Setting Diagnostic to true causes messages to be printed as
/// we do the copying.
///
/// This differs from copy by skipping copying all values that are
/// equal to the Fill_value.
///
/// This can be used to create a simple mosaic by copying multiple
/// images to a common image.
//-----------------------------------------------------------------------

void GeoCal::copy_no_fill(const RasterImage& Img_in, RasterImage& Img_out, 
			  int Fill_value, bool Diagnostic)
{
  if(Img_out.number_line() != Img_in.number_line() ||
     Img_out.number_sample() != Img_in.number_sample())
    throw Exception("Images need to be the same size");
  int tnl = std::min(Img_in.number_tile_line(), Img_out.number_tile_line());
  int tns = std::min(Img_in.number_tile_sample(), Img_out.number_tile_sample());
  for(int istart = 0; istart < Img_out.number_line();
      istart += tnl)
    for(int jstart = 0; jstart < Img_out.number_sample();
	jstart += tns) {
      if(Diagnostic)
	std::cerr << "Starting tile (" << istart << ", " << jstart 
		  << ") of size (" << tnl << ", "
		  << tns << ")\n";
      for(int i = istart; i < Img_out.number_line() && i < istart + tnl; ++i)
	for(int j = jstart; j < Img_out.number_sample() && j < jstart + tns; 
	    ++j) {
	  if(Img_in.copy_needs_double()) {
	    double v = Img_in.read_double(i, j, 1, 1)(0,0);
	    if((int) v != Fill_value)
	      Img_out.write(i, j, v);
	  } else {
	    int v = Img_in(i, j);
	    if(v != Fill_value)
	      Img_out.write(i, j, v);
	  }
	}
    }
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// This copies one image to another. The images should be the same
/// size. Setting Diagnostic to true causes messages to be printed as
/// we do the copying.
///
/// This differs from copy by skipping copying all values that are
/// equal to the Fill_value, and only writing to areas in the new
/// image that aren't fill value. This is useful to do a fast mosaic
/// without any feathering, which just lay image on top of an existing
/// mosaic and crop wherever there is already data.
//-----------------------------------------------------------------------

void GeoCal::copy_only_to_fill(const RasterImage& Img_in, RasterImage& Img_out, 
			       int Fill_value, bool Diagnostic)
{
  if(Img_out.number_line() != Img_in.number_line() ||
     Img_out.number_sample() != Img_in.number_sample())
    throw Exception("Images need to be the same size");
  int tnl = std::min(Img_in.number_tile_line(), Img_out.number_tile_line());
  int tns = std::min(Img_in.number_tile_sample(), Img_out.number_tile_sample());
  for(int istart = 0; istart < Img_out.number_line();
      istart += tnl)
    for(int jstart = 0; jstart < Img_out.number_sample();
	jstart += tns) {
      if(Diagnostic)
	std::cerr << "Starting tile (" << istart << ", " << jstart 
		  << ") of size (" << tnl << ", "
		  << tns << ")\n";
      for(int i = istart; i < Img_out.number_line() && i < istart + tnl; ++i)
	for(int j = jstart; j < Img_out.number_sample() && j < jstart + tns; 
	    ++j) {
	  if(Img_out.read(i, j) == Fill_value) {
	    if(Img_in.copy_needs_double())
	      Img_out.write(i, j, Img_in.read_double(i,j,1,1)(0,0));
	    else
	      Img_out.write(i, j, Img_in(i,j));
	  }
	}
    }
}

//-----------------------------------------------------------------------
/// Some raster images actually are floating point values (e.g., a
/// CalcRaster). This function gives access to the underlying floating
/// point numbers. The default function here just returns the result
/// of read as a double, but derived classes can override this.
//-----------------------------------------------------------------------

blitz::Array<double, 2> 
RasterImage::read_double(int Lstart, int Sstart, int Number_line, 
			 int Number_sample) const
{
  Array<int, 2> dataint = read(Lstart, Sstart, Number_line, Number_sample);
  Array<double, 2> res(dataint.shape());
  res = cast<double>(dataint);
  return res;
}

//-----------------------------------------------------------------------
/// Return a subset of the image. This variation of read allows the
/// data read to extend past the actual RasterImage (e.g., negative
/// start line). In this case, we return the given fill value for the
/// outside area.
//-----------------------------------------------------------------------

blitz::Array<double, 2>
RasterImage::read_double_with_pad(int Lstart, int Sstart, int Number_line, 
				  int Number_sample, double Fill_value) const
{
  range_min_check(Number_line, 1);
  range_min_check(Number_sample, 1);
  blitz::Array<double, 2> res(Number_line, Number_sample);
  res = Fill_value;
  if(Lstart >= number_line() ||
     Sstart >= number_sample() ||
     Lstart + Number_line <= 0 ||
     Sstart + Number_sample <= 0)
    return res;
  int lind = 0;
  int rline = Lstart;
  int sind = 0;
  int rsamp = Sstart;
  int nlind = Number_line;
  int nsind = Number_sample;
  if(Lstart < 0) {
    lind = -Lstart;
    nlind -= lind;
    rline = 0;
  }
  if(Sstart < 0) {
    sind = -Sstart;
    nsind -= sind;
    rsamp = 0;
  }
  if(nlind > number_line() - rline)
    nlind = number_line() - rline;
  if(nsind > number_sample() - rsamp)
    nsind = number_sample() - rsamp;
  res(blitz::Range(lind, lind + nlind-1), blitz::Range(sind, sind + nsind-1)) =
    read_double(rline, rsamp, nlind, nsind);
  return res;
}

//-----------------------------------------------------------------------
/// Translate a number of points at once. This is really meant for use
/// with python, where this is faster than the normal interface.
//-----------------------------------------------------------------------

blitz::Array<double, 2> 
RasterImage::coordinate(const blitz::Array<double, 1>& Lat,
			const blitz::Array<double, 1>& Lon) const
{
  if(Lat.rows() != Lon.rows())
    throw Exception("Lat and Lon need to be the sampe size.");
  Array<double, 2> res(Lat.rows(), 2);
  for(int i = 0; i < Lat.rows(); ++i) {
    ImageCoordinate ic = coordinate(Geodetic(Lat(i), Lon(i)));
    res(i, 0) = ic.line;
    res(i, 1) = ic.sample;
  }
  return res;
}


