#include "igc_simulated.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "memory_multi_band.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

IgcSimulated::IgcSimulated(
const boost::shared_ptr<ImageGroundConnection>& Igc,
const boost::shared_ptr<RasterImage>& Map_projected_image, 
int Avg_fact,
bool Read_into_memory,
int Number_tile_line,
int Number_tile_sample,
double Fill_value
)
  : CalcRaster(Igc->number_line(), Igc->number_sample()),
    igc_(Igc),
    avg_factor_(Avg_fact),
    fill_value_(Fill_value),
    read_into_memory_(Read_into_memory),
    img_(Map_projected_image)
{
  if(avg_factor_ < 0)
    avg_factor_ = 
      (int) floor(Igc->resolution_meter() / Map_projected_image->map_info().resolution_meter());
  if(avg_factor_ > 1)
    img_avg_.reset(new RasterAveraged(img_, avg_factor_, avg_factor_));
  else
    img_avg_ = img_;
  if(Read_into_memory)
    img_avg_.reset(new MemoryRasterImage(*img_avg_));
  if(Number_tile_line < 0)
    number_tile_line_ = number_line();
  else
    number_tile_line_ = Number_tile_line;
  if(Number_tile_sample < 0)
    number_tile_sample_ = number_sample();
  else
    number_tile_sample_ = Number_tile_sample;
}

// See base class for description
void IgcSimulated::calc(int Lstart, int Sstart) const
{
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	igc_->ground_coordinate(ImageCoordinate(Lstart + i, Sstart + j));
      ImageCoordinate ic = img_avg_->coordinate(*gc);
      if(ic.line < 0 || ic.line > img_avg_->number_line() - 1 ||
	 ic.sample < 0 || ic.sample > img_avg_->number_sample() - 1)
	data(i, j) = fill_value_;
      else
	data(i, j) = img_avg_->unchecked_interpolate(ic.line, ic.sample);
    }
}

// See base class for description
void IgcSimulated::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcSimulated:\n"
     << "  Avg_factor:       " << avg_factor() << "\n"
     << "  Read into memory: " << (read_into_memory() ? "true" : "false") 
     << "\n"
     << "  Fill value:   " << fill_value() << "\n";
  Os << "  Image ground connection:\n" ;
  opad << *igc_ << "\n";
  opad.strict_sync();
  Os << "  Map projected image:\n" ;
  opad << *img_ << "\n";
  opad.strict_sync();
}
