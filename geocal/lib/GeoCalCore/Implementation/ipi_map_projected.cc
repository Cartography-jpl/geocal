#include "ipi_map_projected.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the camera
/// resolution. 
//-----------------------------------------------------------------------

IpiMapProjected::IpiMapProjected(const MapInfo& Mi, 
				 const boost::shared_ptr<Ipi>& I,
				 const boost::shared_ptr<RasterImage>& R,
				 const boost::shared_ptr<Dem>& D,
				 int Avg_fact,
				 bool Read_into_memory)
: CalcMapProjected(Mi), ipi(I)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / ipi->resolution_meter());
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

IpiMapProjected::IpiMapProjected(const MapInfo& Mi, 
		 const boost::shared_ptr<Ipi>& I,
		 const std::vector<boost::shared_ptr<RasterImage> >& R,
		 const boost::shared_ptr<Dem>& D, 
		 int Avg_fact, bool Read_into_memory)
: CalcMapProjected(Mi), ipi(I)
{
  if(Avg_fact < 0)
    Avg_fact = (int) round(Mi.resolution_meter() / ipi->resolution_meter());
  initialize(R, D, Avg_fact, Read_into_memory);
}

//-----------------------------------------------------------------------
/// Print out a description of the object.
//-----------------------------------------------------------------------

void IpiMapProjected::print(std::ostream& Os) const
{
  Os << "IpiMapProjected:\n"
     << "  Map info: " << map_info() << "\n"
     << "  Ipi:      " << *ipi << "\n"
     << "  Image:    " << img()[0] << "\n"
     << "  Dem:      " << dem() << "\n";
}

// See base class for description

ImageCoordinate IpiMapProjected::calc_image_coordinates(const 
	GroundCoordinate& Gc) const
{
  ImageCoordinate ic;
  bool success;
  ipi->image_coordinate_extended(Gc, ic, success);
  if(success)
    return ic;
  else
    return ImageCoordinate(-1,-1);
}

