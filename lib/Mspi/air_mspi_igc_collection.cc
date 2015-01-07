#include "air_mspi_igc_collection.h"

using namespace GeoCal;



// see base class for description.
boost::shared_ptr<ImageGroundConnection> 
AirMspiIgcCollection::image_ground_connection(int Image_index) const
{
  range_check(Image_index, 0, number_image());
}

// see base class for description.
boost::shared_ptr<IgcCollection> 
AirMspiIgcCollection::subset(const std::vector<int> Index_set) const
{
}



// see base class for description.
void AirMspiIgcCollection::print(std::ostream& Os) const 
{
  Os << "AirMspiIgcCollection:\n";
     // << "  Master config file: " << master_config_file() << "\n"
     // << "  Orbit file name:    " << orbit_file_name() << "\n"
     // << "  Base directory:     " << base_directory() << "\n";
}


