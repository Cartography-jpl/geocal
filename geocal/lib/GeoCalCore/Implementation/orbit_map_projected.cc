#include "orbit_map_projected.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the camera
/// resolution. 
//-----------------------------------------------------------------------

OrbitMapProjected::OrbitMapProjected(const MapInfo& Mi, 
				     const boost::shared_ptr<OrbitData>& Od,
				     const boost::shared_ptr<RasterImage>& R,
				     const boost::shared_ptr<Camera>& C,
				     const boost::shared_ptr<Dem>& D,
				     int Band,
				     int Avg_fact,
				     bool Read_into_memory)
  : CalcMapProjected(Mi), band(Band), od(Od), cam(C)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / 
			   od->resolution_meter(*C, band));
  initialize(R, D, Avg_fact, Read_into_memory);
}

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the camera
/// resolution. 
///
/// This is a variation that handles multiple bands at one time, you
/// give it the set of raster image to process.
//-----------------------------------------------------------------------

OrbitMapProjected::OrbitMapProjected(const MapInfo& Mi, 
				     const boost::shared_ptr<OrbitData>& Od,
		     const std::vector<boost::shared_ptr<RasterImage> >& R,
		     const boost::shared_ptr<Camera>& C,
		     const boost::shared_ptr<Dem>& D,
		     int Band,
				     int Avg_fact, bool Read_into_memory)
  : CalcMapProjected(Mi), band(Band), od(Od), cam(C)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / 
			   od->resolution_meter(*C, band));
  initialize(R, D, Avg_fact, Read_into_memory);
}

//-----------------------------------------------------------------------
/// Print out a description of the object.
//-----------------------------------------------------------------------

void OrbitMapProjected::print(std::ostream& Os) const
{
  Os << "OrbitMapProjected:\n"
     << "  Map info:   " << map_info() << "\n"
     << "  Orbit data: " << *od << "\n"
     << "  Camera:     " << *cam << "\n"
     << "  Image:      " << img()[0] << "\n"
     << "  Dem:        " << dem() << "\n";
}

