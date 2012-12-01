#include "raster_averaged.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

RasterAveraged::RasterAveraged(const boost::shared_ptr<RasterImage>& Data,
			       int Number_line_per_pixel, 
			       int Number_sample_per_pixel,
			       bool Ignore_zero)
: RasterImageVariable
  (Data->number_line() / Number_line_per_pixel,
   Data->number_sample() / Number_sample_per_pixel,
   std::max(Data->number_tile_line() / Number_line_per_pixel, 1),
   std::max(Data->number_tile_sample() / Number_sample_per_pixel, 1)),
  data_(Data),
  ignore_zero_(Ignore_zero),
  number_line_per_pixel_(Number_line_per_pixel),
  number_sample_per_pixel_(Number_sample_per_pixel)
{
  range_min_check(Number_line_per_pixel, 1);
  range_min_check(Number_sample_per_pixel, 1);
  if(Data->has_map_info())
    map_info_.reset(new MapInfo(Data->map_info().scale(Number_sample_per_pixel,
						       Number_line_per_pixel)));
  if(Data->has_rpc()) {
    rpc_.reset(new Rpc(Data->rpc()));
    rpc_->line_scale /= Number_line_per_pixel;
    rpc_->sample_scale /= Number_sample_per_pixel;
  }
}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

int RasterAveraged::unchecked_read(int Line, int Sample) const
{
  double tot = 0;
  int count = 0;
  for(int i = 0; i < number_line_per_pixel_; ++i)
    for(int j = 0; j < number_sample_per_pixel_; ++j) {
      int v = data_->unchecked_read(i + Line * number_line_per_pixel_, 
				    j + Sample * number_sample_per_pixel_);
      tot += v;
      if(!ignore_zero_ || v != 0)
	++count;
    }
  return (count == 0 ? 0 : (int) (tot / count));
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

void RasterAveraged::read_ptr(int Lstart, int Sstart, int Number_line, 
			      int Number_sample, int* Res) const
{
  range_min_check(Lstart, 0);
  range_min_check(Sstart, 0);
  range_max_check(Lstart + Number_line - 1, number_line());
  range_max_check(Sstart + Number_sample - 1, number_sample());
  blitz::Array<int, 2> res_high = 
    data_->read(Lstart * number_line_per_pixel(),
		      Sstart * number_sample_per_pixel(),
		      Number_line * number_line_per_pixel(),
		      Number_sample * number_sample_per_pixel());
  for(int i = 0; i < Number_line; ++i)
    for(int j = 0; j < Number_sample; ++j, ++Res) {
      blitz::Range r1(i * number_line_per_pixel(), 
		      (i + 1) * number_line_per_pixel() - 1);
      blitz::Range r2(j * number_sample_per_pixel(), 
		      (j + 1) * number_sample_per_pixel() - 1);
      blitz::Array<int, 2> res_high_sub(res_high(r1, r2));
      int cnt = (ignore_zero_ ? count(res_high_sub != 0) : 
		   res_high_sub.size());
      *Res = (cnt == 0 ? 0 : (int) sum(res_high_sub) / cnt);
    }
}

//-----------------------------------------------------------------------
/// Write a value. This isn't actually implemented for this type of 
/// RasterImage.
//-----------------------------------------------------------------------

void RasterAveraged::unchecked_write(int Line, int Sample, int Val)
{
  throw Exception("Write not implemented for RasterAveraged");
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RasterAveraged::print(std::ostream& Os) const
{
  Os << "RasterAverage:\n"
     << "  Number line per pixel: " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  RasterImage: " << high_resolution_image() << "\n";
}
