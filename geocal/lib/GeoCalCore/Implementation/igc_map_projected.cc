#include "igc_map_projected.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the camera
/// resolution. 
///
/// Most of the time, you want to use the raster image associated with
/// the Igc. This constructor allows you to use a different
/// RasterImage (perhaps a related band, or something like that).
//-----------------------------------------------------------------------

IgcMapProjected::IgcMapProjected
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 const boost::shared_ptr<RasterImage>& R,
 int Avg_fact,
 bool Read_into_memory)
  : CalcMapProjected(Mi), igc(Igc)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / 
			   igc->resolution_meter(ImageCoordinate(R->number_line() / 2.0, R->number_sample() / 2.0)));
  initialize(R, Igc->dem_ptr(), Avg_fact, Read_into_memory);
}

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
  : CalcMapProjected(Mi), igc(Igc)
{
  boost::shared_ptr<RasterImage> R = igc->image();
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / 
			   igc->resolution_meter(ImageCoordinate(R->number_line() / 2.0, R->number_sample() / 2.0)));
  initialize(R, Igc->dem_ptr(), Avg_fact, Read_into_memory);
}

//-----------------------------------------------------------------------
/// Print out a description of the object.
//-----------------------------------------------------------------------

void IgcMapProjected::print(std::ostream& Os) const
{
  Os << "IgcMapProjected:\n"
     << "  Map info:   " << map_info() << "\n"
     << "  Image ground connection: " << *igc << "\n"
     << "  Image:      " << img()[0] << "\n";
}

