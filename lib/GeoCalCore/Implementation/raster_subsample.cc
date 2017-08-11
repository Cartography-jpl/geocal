#include "raster_subsample.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

RasterSubSample::RasterSubSample(const boost::shared_ptr<RasterImage>& Data,
			       int Number_line_per_pixel, 
			       int Number_sample_per_pixel)
: RasterImageVariable
  (Data->number_line() / Number_line_per_pixel,
   Data->number_sample() / Number_sample_per_pixel,
   std::max(Data->number_tile_line() / Number_line_per_pixel, 1),
   std::max(Data->number_tile_sample() / Number_sample_per_pixel, 1)),
  data_(Data),
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
/// Return a subset of the image.
//-----------------------------------------------------------------------

void RasterSubSample::read_ptr(int Lstart, int Sstart, int Number_line, 
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
    for(int j = 0; j < Number_sample; ++j, ++Res)
      *Res = res_high(i * number_line_per_pixel(),
		      j * number_sample_per_pixel());
}

//-----------------------------------------------------------------------
/// Write a value. This isn't actually implemented for this type of 
/// RasterImage.
//-----------------------------------------------------------------------

void RasterSubSample::unchecked_write(int Line, int Sample, int Val)
{
  throw Exception("Write not implemented for RasterSubSample");
}

void RasterSubSample::unchecked_write(int Line, int Sample, double Val)
{
  throw Exception("Write not implemented for RasterSubSample");
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RasterSubSample::print(std::ostream& Os) const
{
  Os << "RasterSubSample:\n"
     << "  Number line per pixel: " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  RasterImage: " << high_resolution_image() << "\n";
}

