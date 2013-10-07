#include "sub_raster_image.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

SubRasterImage::SubRasterImage(const boost::shared_ptr<RasterImage>& 
      Raster_image, int Start_line, int Start_sample,
      int Number_line, int Number_sample)
: RasterImageVariable(Number_line, Number_sample, 
		      Raster_image->number_tile_line(),
		      Raster_image->number_tile_sample()),
  data_(Raster_image),
  start_line_(Start_line),
  start_sample_(Start_sample)
{
  range_check(Start_line, 0, data_->number_line() - Number_line + 1);
  range_check(Start_sample, 0, data_->number_sample() - Number_sample + 1);
  if(Raster_image->has_map_info())
    map_info_.reset
      (new MapInfo(Raster_image->map_info().subset(Start_sample, Start_line, 
				   Number_sample, Number_line)));
  if(Raster_image->has_rpc()) {
    rpc_.reset(new Rpc(Raster_image->rpc()));
    rpc_->line_offset -= Start_line;
    rpc_->sample_offset -= Start_sample;
  }
}

//-----------------------------------------------------------------------
/// Create a subset of Raster_image that covers the given set of points,
/// plus a optional boundary. Note that the area given by Pt may be
/// large than the area covered by Raster_image. In that
/// case, we only give the portion that falls within this image.
//-----------------------------------------------------------------------

SubRasterImage::SubRasterImage(
const boost::shared_ptr<RasterImage>& Raster_image,
const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
int boundary)
:  data_(Raster_image)
{
  number_tile_line_ = Raster_image->number_tile_line();
  number_tile_sample_ = Raster_image->number_tile_sample();

  MapInfo m = Raster_image->map_info().cover(Pt, boundary);
  ImageCoordinate ulc = Raster_image->coordinate(*m.ground_coordinate(0,0));
  ImageCoordinate lrc = Raster_image->coordinate
    (*m.ground_coordinate(m.number_x_pixel() - 1,
			  m.number_y_pixel() - 1));
  start_line_ = (int) round(std::max(ulc.line, 0.0));
  int eline = (int) round(std::min(lrc.line, 
				   Raster_image->number_line() - 1.0));
  start_sample_ = (int) round(std::max(ulc.sample, 0.0));
  int esamp = (int) round(std::min(lrc.sample, 
				   Raster_image->number_sample() - 1.0));
  number_line_ = eline - start_line_ + 1;
  number_sample_ = esamp - start_sample_ + 1;
  if(number_line_ <= 0 || number_sample_ <= 0)
    throw NoCoverage("RasterImage doesn't cover any of the area given by the list of ground points");

  map_info_.reset
    (new MapInfo(Raster_image->map_info().subset(start_sample_, start_line_, 
	 number_sample_, number_line_)));
  if(Raster_image->has_rpc()) {
    rpc_.reset(new Rpc(Raster_image->rpc()));
    rpc_->line_offset -= start_line_;
    rpc_->sample_offset -= start_sample_;
  }
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

void SubRasterImage::read_ptr(int Lstart, int Sstart, int Number_line, 
			      int Number_sample, int* Res) const
{
  range_check(Lstart, 0, number_line() - Number_line + 1);
  range_check(Sstart, 0, number_sample() - Number_sample + 1);
  return data_->read_ptr(Lstart + start_line_, Sstart + start_sample_,
			Number_line, Number_sample, Res);
}

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

void SubRasterImage::unchecked_write(int Line, int Sample, int Val)
{
  return data_->write(Line + start_line_, Sample + start_sample_, Val);
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void SubRasterImage::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "SubRasterImage from (" << start_line_ << ", " << start_sample_ 
     << ") to (" << start_line_ + number_line() - 1 << ", "
     << start_sample_ + number_sample() - 1 << ")\n"
     << "  Underlying RasterImage: \n";
  opad << full_raster_image();
  opad.strict_sync();
}

