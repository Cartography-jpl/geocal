#include "igc_map_projected.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the camera
/// resolution. 
///
/// This uses the Igc.image().
//-----------------------------------------------------------------------

IgcMapProjected::IgcMapProjected
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Avg_fact,
 bool Read_into_memory)
  : CalcMapProjected(Mi)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / 
			   Igc->resolution_meter(ImageCoordinate(Igc->number_line() / 2.0, Igc->number_sample() / 2.0)));
  if(Avg_fact > 1)
    igc_.reset(new AveragedImageGroundConnection(Igc, Avg_fact, Avg_fact,
						 Read_into_memory));
  else {
    if(Read_into_memory) {
      igc_.reset(new ImageGroundConnectionCopy(Igc));
      igc_->image(boost::shared_ptr<RasterImage>
		  (new MemoryRasterImage(*Igc->image())));
    } else
      igc_ = Igc;
  }
  number_tile_line_ = number_line();
  number_tile_sample_ = number_sample();
}


