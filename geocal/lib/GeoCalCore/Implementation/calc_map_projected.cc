#include "calc_map_projected.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "null_deleter.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

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
  if(number_line() != Out.number_line() ||
     number_sample() != Out.number_sample())
    throw Exception("Images need to be the same size");
  int tnl = Out.number_tile_line();
  int tns = Out.number_tile_sample();
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
		  Out.unchecked_write(i + ii, j + jj, 0);
	      } else {
		Out.unchecked_write(i + ii, j + jj, 
	     (int) round(igc_->image()->unchecked_interpolate(ic_line[ii][jj], 
					ic_sample[ii][jj])));
	      }
	}
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
  ImageCoordinate ic = igc_->image_coordinate(*gc);
  line[0][0] = ic.line;
  sample[0][0] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line, Start_sample + Nsamp - 1),
			 igc_->dem());
  ic = igc_->image_coordinate(*gc);
  line[0][1] = ic.line;
  sample[0][1] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line + Nline - 1, 
					 Start_sample), igc_->dem());
  ic = igc_->image_coordinate(*gc);
  line[1][0] = ic.line;
  sample[1][0] = ic.sample;
  gc = ground_coordinate(ImageCoordinate(Start_line + Nline - 1, 
					 Start_sample + Nsamp - 1), 
			 igc_->dem());
  ic = igc_->image_coordinate(*gc);
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
