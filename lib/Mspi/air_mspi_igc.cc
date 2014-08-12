#include "air_mspi_igc.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. This takes the master config file and uses it to
/// create a AirMspiIgc.
///
/// You can optionally add the base directory that file names in the
/// Master_config_file are relative to. The default is the current
/// directory. 
///
/// \todo Do we want to allow the camera to be passed in, so we can
/// share one in the IgcCollection?
//-----------------------------------------------------------------------

AirMspiIgc::AirMspiIgc(const std::string& Master_config_file,
		       const std::string& Base_directory)
: bdir(Base_directory),
  mconfig(Master_config_file)
{
}

void AirMspiIgc::print(std::ostream& Os) const 
{
  Os << "AirMspiIgc:\n"
     << "  Master config file: " << master_config_file() << "\n"
     << "  Base directory:     " << base_directory() << "\n";
}
