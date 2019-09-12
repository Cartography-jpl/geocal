#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "memory_multi_band.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterAveraged::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(ignore_zero)
    & GEOCAL_NVP_(number_line_per_pixel)
    & GEOCAL_NVP_(number_sample_per_pixel);
}

template<class Archive>
void RasterAveragedMultiBand::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRasterMultiBand)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(ignore_zero)
    & GEOCAL_NVP_(number_line_per_pixel)
    & GEOCAL_NVP_(number_sample_per_pixel);
}

template<class Archive>
void ImageMaskAveraged::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageMask)
    & GEOCAL_NVP_(data)
    & GEOCAL_NVP_(number_line_per_pixel)
    & GEOCAL_NVP_(number_sample_per_pixel);
}

template<class Archive>
void AveragedImageGroundConnection::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(ImageGroundConnection, WithParameter);
  GEOCAL_BASE(AveragedImageGroundConnection, ImageGroundConnection);
  ar & GEOCAL_NVP_(ig)
   & GEOCAL_NVP_(number_line_per_pixel)
   & GEOCAL_NVP_(number_sample_per_pixel)
   & GEOCAL_NVP_(in_memory)
   & GEOCAL_NVP_(ignore_zero);
}

template<class Archive>
void AveragedImageGroundConnection::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(ImageGroundConnection, WithParameter);
  GEOCAL_BASE(AveragedImageGroundConnection, ImageGroundConnection);
  ar & GEOCAL_NVP_(ig)
   & GEOCAL_NVP_(number_line_per_pixel)
   & GEOCAL_NVP_(number_sample_per_pixel)
   & GEOCAL_NVP_(in_memory)
   & GEOCAL_NVP_(ignore_zero);
  init();
}

GEOCAL_IMPLEMENT(RasterAveraged);
GEOCAL_IMPLEMENT(RasterAveragedMultiBand);
GEOCAL_IMPLEMENT(ImageMaskAveraged);
GEOCAL_SPLIT_IMPLEMENT(AveragedImageGroundConnection);
#endif

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
  // Just leave RSM off, we don't have a way to reduce size
  // if(Data->has_rsm())
  //   throw Exception("Don't support RSM yet");
}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

RasterAveragedMultiBand::RasterAveragedMultiBand(
const boost::shared_ptr<RasterImageMultiBand>& Data,
int Number_line_per_pixel, 
int Number_sample_per_pixel,
bool Ignore_zero)
: raw_data_(Data),
  ignore_zero_(Ignore_zero),
  number_line_per_pixel_(Number_line_per_pixel),
  number_sample_per_pixel_(Number_sample_per_pixel)
{
  range_min_check(Number_line_per_pixel, 1);
  range_min_check(Number_sample_per_pixel, 1);
  RasterAveraged sample_rimg(Data->raster_image_ptr(0),Number_line_per_pixel,
			     Number_sample_per_pixel, Ignore_zero);
  initialize(sample_rimg, Data->number_band(),
	     std::max(Data->raster_image(0).number_tile_line() / 
		      Number_line_per_pixel, 1),
	     std::max(Data->raster_image(0).number_tile_sample() / 
		      Number_sample_per_pixel, 1));
}

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

void RasterAveragedMultiBand::calc(int Lstart, int Sstart) const
{
  blitz::Array<double, 3> res_high = 
    raw_data_->read_double(Lstart * number_line_per_pixel(),
			   Sstart * number_sample_per_pixel(),
			   data.cols() * number_line_per_pixel(),
			   data.depth() * number_sample_per_pixel());
  for(int i = 0; i < data.cols(); ++i)
    for(int j = 0; j < data.depth(); ++j) {
      blitz::Range r1(i * number_line_per_pixel(), 
		      (i + 1) * number_line_per_pixel() - 1);
      blitz::Range r2(j * number_sample_per_pixel(), 
		      (j + 1) * number_sample_per_pixel() - 1);
      for(int k = 0; k < data.rows(); ++k) {
	blitz::Array<double, 2> res_high_sub(res_high(k, r1, r2));
	int cnt = (ignore_zero_ ? count(res_high_sub != 0) : 
		   res_high_sub.size());
	data(k,i,j) = (cnt == 0 ? 0 : (int) sum(res_high_sub) / cnt);
      }
    }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RasterAveraged::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RasterAverage:\n"
     << "  Number line per pixel: " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  RasterImage:\n";
  opad << high_resolution_image() << "\n";
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RasterAveragedMultiBand::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RasterAverageMultiBand:\n"
     << "  Number line per pixel: " << number_line_per_pixel() << "\n"
     << "  Number sample per pixel: " << number_sample_per_pixel() << "\n"
     << "  RasterImageMultiBand:\n";
  opad << high_resolution_image() << "\n";
  opad.strict_sync();
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
  init();
}

void AveragedImageGroundConnection::init()
{
  dem_ = ig_->dem_ptr();
  if(ig_->image()) {
    boost::shared_ptr<RasterImage> ra
      (new RasterAveraged(ig_->image(), number_line_per_pixel_, 
			  number_sample_per_pixel_, ignore_zero_));
    if(in_memory_)
      image_.reset(new MemoryRasterImage(*ra));
    else
      image_ = ra;
  }
  if(ig_->image_multi_band()) {
    boost::shared_ptr<RasterImageMultiBand> ra
      (new RasterAveragedMultiBand(ig_->image_multi_band(), 
				   number_line_per_pixel_, 
				   number_sample_per_pixel_, ignore_zero_));
    if(in_memory_)
      image_mb_.reset(new MemoryMultiBand(*ra));
    else
      image_mb_ = ra;
  }
  image_mask_.reset
    (new ImageMaskAveraged(ig_->image_mask(), number_line_per_pixel_,
			   number_sample_per_pixel_));
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

