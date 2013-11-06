#include "raster_averaged.h"
#include "memory_raster_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

RasterAveraged::RasterAveraged(const boost::shared_ptr<RasterImage>& Data,
			       int Number_line_per_pixel, 
			       int Number_sample_per_pixel,
			       bool Ignore_zero)
: CalcRaster
  (Data->number_line() / Number_line_per_pixel,
   Data->number_sample() / Number_sample_per_pixel,
   std::max(Data->number_tile_line() / Number_line_per_pixel, 1),
   std::max(Data->number_tile_sample() / Number_sample_per_pixel, 1)),
  raw_data_(Data),
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
/// Return a subset of the image.
//-----------------------------------------------------------------------

void RasterAveraged::calc(int Lstart, int Sstart) const
{
  blitz::Array<double, 2> res_high = 
    raw_data_->read_double(Lstart * number_line_per_pixel(),
			   Sstart * number_sample_per_pixel(),
			   data.rows() * number_line_per_pixel(),
			   data.cols() * number_sample_per_pixel());
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      blitz::Range r1(i * number_line_per_pixel(), 
		      (i + 1) * number_line_per_pixel() - 1);
      blitz::Range r2(j * number_sample_per_pixel(), 
		      (j + 1) * number_sample_per_pixel() - 1);
      blitz::Array<double, 2> res_high_sub(res_high(r1, r2));
      int cnt = (ignore_zero_ ? count(res_high_sub != 0) : 
		   res_high_sub.size());
      data(i,j) = (cnt == 0 ? 0 : (int) sum(res_high_sub) / cnt);
    }
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


bool ImageMaskAveraged::mask(int Line, int Sample) const
{
  return data_->area_any_masked(Line * number_line_per_pixel_,
				Sample * number_sample_per_pixel_,
				number_line_per_pixel_,
				number_sample_per_pixel_);
}

bool ImageMaskAveraged::area_any_masked
(int Line, int Sample, int Number_line,
 int Number_sample) const
{
  return data_->area_any_masked(Line * number_line_per_pixel_,
				Sample * number_sample_per_pixel_,
				Number_line * number_line_per_pixel_,
				Number_sample * number_sample_per_pixel_);
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void ImageMaskAveraged::print(std::ostream& Os) const
{
  Os << "ImageMaskAverage:\n"
     << "  Number line per pixel: " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  ImageMask: " << high_resolution_image_mask() << "\n";
}

//-----------------------------------------------------------------------
/// Constructor for average. You can specify your choice of doing the 
/// average calculation on the fly, or once and kept in memory.
///
/// Note that we set the title to the empty string, you can optionally 
/// set this to whatever after creating this object.
//-----------------------------------------------------------------------

AveragedImageGroundConnection::AveragedImageGroundConnection
(const boost::shared_ptr<ImageGroundConnection> Igc,
 int Number_line_per_pixel, 
 int Number_sample_per_pixel,
 bool In_memory,
 bool Ignore_zero)
  : ig_(Igc), number_line_per_pixel_(Number_line_per_pixel),
    number_sample_per_pixel_(Number_sample_per_pixel),
    in_memory_(In_memory), ignore_zero_(Ignore_zero) 
{ 
  dem_ = ig_->dem_ptr();
  boost::shared_ptr<RasterImage> ra
    (new RasterAveraged(ig_->image(), number_line_per_pixel_, 
			number_sample_per_pixel_, ignore_zero_));
  image_mask_.reset
    (new ImageMaskAveraged(ig_->image_mask(), number_line_per_pixel_,
			   number_sample_per_pixel_));
  if(in_memory_)
    image_.reset(new MemoryRasterImage(*ra));
  else
    image_ = ra;
  ground_mask_ = ig_->ground_mask();
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void AveragedImageGroundConnection::print(std::ostream& Os) const
{ 
  Os << "AveragedImageGroundConnection\n"
     << "  Number line per pixel:   " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  Ignore zero:             " << ignore_zero() << "\n"
     << "  In memory:               " << in_memory() << "\n"
     << "  Original ImageGroundConnection:\n"
     << *ig_ << "\n";
}

