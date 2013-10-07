#include "igc_map_projected.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the Igc
/// resolution. 
///
/// You can optionally pass a grid spacing to use. We calculate
/// image coordinates in the input exactly at the grid spacing, and
/// interpolate in betweeen. This is much faster than calculating
/// every point, and if the grid spacing is small compared to the Dem
/// and any nonlinearities then it gives results very close to the
/// full calculation.
//-----------------------------------------------------------------------

IgcMapProjected::IgcMapProjected
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Grid_spacing,
 int Avg_fact,
 bool Read_into_memory,
 int Number_tile_line, int Number_tile_sample)
  : CalcMapProjected(Mi), 
    igc_original_(Igc), 
    avg_factor_(Avg_fact),
    grid_spacing_(Grid_spacing),
    read_into_memory_(Read_into_memory)
{
  if(avg_factor_ < 0)
    avg_factor_ = (int) round(Mi.resolution_meter() / 
			   Igc->resolution_meter(ImageCoordinate(Igc->number_line() / 2.0, Igc->number_sample() / 2.0)));
  if(avg_factor_ > 1)
    igc_.reset(new AveragedImageGroundConnection(Igc, avg_factor_, avg_factor_,
						 Read_into_memory));
  else {
    if(Read_into_memory) {
      igc_.reset(new ImageGroundConnectionCopy(Igc));
      igc_->image(boost::shared_ptr<RasterImage>
		  (new MemoryRasterImage(*Igc->image())));
    } else
      igc_ = Igc;
  }
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
void IgcMapProjected::calc(int Lstart, int Sstart) const
{
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	ground_coordinate(ImageCoordinate(Lstart + i, Sstart + j), 
			  igc_->dem());
      ImageCoordinate ic = igc_->image_coordinate(*gc);
      if(ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
	 ic.sample < 0 || ic.sample >= igc_->number_sample() - 1)
	data(i, j) =  0;	// Data outside of image, so 0.
      else
	data(i, j) = igc_->image()->unchecked_interpolate(ic.line, ic.sample);
    }
}


