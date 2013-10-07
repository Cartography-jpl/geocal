#include "igc_map_projected.h"
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
  initialize(Igc, Avg_fact, Read_into_memory);
}


