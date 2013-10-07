#include "calc_map_projected.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "null_deleter.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Initialize. We average the data by the factor given as
/// Avg_fact. This should be something like the
/// MapInfo.resolution_meter() / resolution of data on ground.
///
/// We usually want to read everything into memory to get a reasonable
/// performance. However, there may be times that you don't want this
/// done (e.g., data is already in memory, we are only using a small
/// part of the full image). You can turn this off by having 
/// Read_into_memory set to false.
//-----------------------------------------------------------------------

void CalcMapProjected::initialize
(const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Avg_fact, bool Read_into_memory)
{
  line_avg_ = Avg_fact;
  samp_avg_ = Avg_fact;
  if(line_avg_ > 1 || samp_avg_ > 1) {
    igc_.reset(new AveragedImageGroundConnection(Igc, line_avg_, samp_avg_,
						 Read_into_memory));
  } else {
    igc_ = Igc;
  }
  number_tile_line_ = number_line();
  number_tile_sample_ = number_sample();
}

//-----------------------------------------------------------------------
// Write output to a RasterImage. You can optionally pass a grid
// spacing to use. We calculate the image coordinates in the
/// input exactly at this grid spacing, and interpolate in between.
/// This is much faster than calculating ever point, and if the grid 
/// spacing is small compared to the Dem and any nonlinearities
/// give results very close to the full calculation.
//-----------------------------------------------------------------------

void CalcMapProjected::write_image(RasterImage& Out, 
				   int Grid_spacing) const
{
  std::vector<boost::shared_ptr<RasterImage> > t2;
  t2.push_back(pointer(Out));
  if(Grid_spacing == 1)
    write_multiple(t2);
  else
    write_multiple(t2, Grid_spacing);
}

//-----------------------------------------------------------------------
/// Write output to multiple RasterImage at once. The number should be
/// <= the number given to the constructor.
//-----------------------------------------------------------------------

void CalcMapProjected::write_multiple(const 
  std::vector<boost::shared_ptr<RasterImage> >& Out) const
{
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& outr, Out)
    if(number_line() != outr->number_line() ||
       number_sample() != outr->number_sample())
      throw Exception("Images need to be the same size");
  if(Out.size() <= 0 || Out.size() > 1)
    throw Exception("Out needs to have at least one file, and less than the number of images");
  int tnl = Out[0]->number_tile_line();
  int tns = Out[0]->number_tile_sample();
  for(int istart = 0; istart < number_line();
      istart += tnl)
    for(int jstart = 0; jstart < number_sample();
	jstart += tns)
      for(int i = istart; i < istart + tnl && i < number_line(); ++i)
	for(int j = jstart; j < jstart + tns && j < number_sample(); ++j) {
	  boost::shared_ptr<GroundCoordinate> gc = 
	    ground_coordinate(ImageCoordinate(i, j), igc_->dem());
	  ImageCoordinate ic = calc_image_coordinates(*gc);
	  if(ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
	     ic.sample < 0 || ic.sample >= igc_->number_sample() - 1) {
	    BOOST_FOREACH(const boost::shared_ptr<RasterImage>& outr, Out)
	      outr->unchecked_write(i, j, 0);
	  } else {
	    for(int k = 0; k < (int) Out.size(); ++k)
	      Out[k]->unchecked_write(i, j, 
      (int) round(igc_->image()->unchecked_interpolate(ic.line, ic.sample)));
	  }
	}
}

//-----------------------------------------------------------------------
/// Write output to multiple RasterImage at once. This variation takes
/// a grid spacing to use. We calculate the image coordinates in the
/// input exactly at this grid spacing, and interpolate in between.
/// This is much faster than calculating ever point, and if the grid 
/// spacing is small compared to the Dem and any Camera nonlinearities
/// give results very close to the full calculation.
//-----------------------------------------------------------------------

void CalcMapProjected::write_multiple(const 
  std::vector<boost::shared_ptr<RasterImage> >& Out,
  int Grid_spacing) const
{
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& outr, Out)
    if(number_line() != outr->number_line() ||
       number_sample() != outr->number_sample())
      throw Exception("Images need to be the same size");
  if(Out.size() <= 0 || Out.size() > 1)
    throw Exception("Out needs to have at least one file, and less than the number of images");
  int tnl = Out[0]->number_tile_line();
  int tns = Out[0]->number_tile_sample();
  boost::multi_array<double, 2>::extent_gen extents;
  boost::multi_array<double, 2> ic_line(extents[Grid_spacing][Grid_spacing]); 
  boost::multi_array<double, 2> 
    ic_sample(extents[Grid_spacing][Grid_spacing]); 
  for(int istart = 0; istart < number_line();
      istart += tnl)
    for(int jstart = 0; jstart < number_sample();
	jstart += tns)
      for(int i = istart; i < istart + tnl && i < number_line(); 
	  i += Grid_spacing)
	for(int j = jstart; j < jstart + tns && j < number_sample(); 
	    j += Grid_spacing) {
	  int inum = std::min(Grid_spacing, 
			      std::min(istart + tnl - i, number_line() - i));
	  int jnum = std::min(Grid_spacing, 
			      std::min(jstart + tns - j, number_sample() - j));
	  interpolate_ic(i, j, inum, jnum, ic_line, ic_sample);
	  for(int ii = 0; ii < inum; ++ii)
	    for(int jj = 0; jj < jnum; ++jj)
	      if(ic_line[ii][jj] < 0 || 
		 ic_line[ii][jj] >= igc_->number_line() - 1 ||
		 ic_sample[ii][jj] < 0 || 
		 ic_sample[ii][jj] >= igc_->number_sample() - 1) {
		BOOST_FOREACH(const 
			      boost::shared_ptr<RasterImage>& outr, Out)
		  outr->unchecked_write(i + ii, j + jj, 0);
	      } else {
		for(int k = 0; k < (int) Out.size(); ++k)
		  Out[k]->unchecked_write(i + ii, j + jj, 
	  (int) round(igc_->image()->unchecked_interpolate(ic_line[ii][jj], 
					ic_sample[ii][jj])));
	      }
	}
}

//-----------------------------------------------------------------------
/// Read pixel value at given line and sample.
//-----------------------------------------------------------------------

int CalcMapProjected::unchecked_read(int Line, int Sample) const
{
  boost::shared_ptr<GroundCoordinate> gc = 
    ground_coordinate(ImageCoordinate(Line, Sample), igc_->dem());
  ImageCoordinate ic = calc_image_coordinates(*gc);
  if(ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
     ic.sample < 0 || ic.sample >= igc_->number_sample() - 1)
    return 0;			// Data outside of image, so return 0.
  return (int) round(igc_->image()->unchecked_interpolate(ic.line, ic.sample));
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

void CalcMapProjected::read_ptr(int Lstart, int Sstart, int Number_line, 
			         int Number_sample, int* Res) const
{
  range_min_check(Lstart, 0);
  range_min_check(Sstart, 0);
  range_max_check(Lstart + Number_line - 1, number_line());
  range_max_check(Sstart + Number_sample - 1, number_sample());
  for(int i = Lstart; i < Lstart + Number_line; ++i)
    for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
      *Res = (*this)(i, j);
}

//-----------------------------------------------------------------------
/// Write a value. This isn't actually implemented for this type of 
/// RasterImage.
//-----------------------------------------------------------------------

void CalcMapProjected::unchecked_write(int Line, int Sample, int Val)
{
  throw Exception("Write not implemented for CalcMapProjected");
}

//-----------------------------------------------------------------------
/// Do a bilinear interpolate at the four points to get the line and
/// sample.
//-----------------------------------------------------------------------

void CalcMapProjected::interpolate_ic(int Start_line, int Start_sample, 
    int Nline, int Nsamp, boost::multi_array<double, 2>& Ic_line,
    boost::multi_array<double, 2>& Ic_sample) const
{
  double line[2][2];
  double sample[2][2];
  boost::shared_ptr<GroundCoordinate> gc;
  gc = ground_coordinate(ImageCoordinate(Start_line, Start_sample),
			 igc_->dem());
  ImageCoordinate ic = calc_image_coordinates(*gc);
  line[0][0] = ic.line;
  sample[0][0] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line, Start_sample + Nsamp - 1),
			 igc_->dem());
  ic = calc_image_coordinates(*gc);
  line[0][1] = ic.line;
  sample[0][1] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line + Nline - 1, 
					 Start_sample), igc_->dem());
  ic = calc_image_coordinates(*gc);
  line[1][0] = ic.line;
  sample[1][0] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line + Nline - 1, 
					 Start_sample + Nsamp - 1), 
			 igc_->dem());
  ic = calc_image_coordinates(*gc);
  line[1][1] = ic.line;
  sample[1][1] = ic.sample;
  for(int ii = 0; ii < Nline; ++ii)
    for(int jj = 0; jj < Nsamp; ++jj) {
      double lfrac = (Nline > 1 ? ii / (Nline - 1.0) : 0.0);
      double sfrac = (Nsamp > 1 ? jj / (Nsamp - 1.0) : 0.0);
      Ic_line[ii][jj] = (1.0 - lfrac - sfrac + lfrac * sfrac) * line[0][0] +
	(sfrac - lfrac * sfrac) * line[0][1] +
	(lfrac - lfrac * sfrac) * line[1][0] +
	(lfrac * sfrac) * line[1][1];
      Ic_sample[ii][jj] = 
	(1.0 - lfrac - sfrac + lfrac * sfrac) * sample[0][0] +
	(sfrac - lfrac * sfrac) * sample[0][1] +
	(lfrac - lfrac * sfrac) * sample[1][0] +
	(lfrac * sfrac) * sample[1][1];
    }
}
