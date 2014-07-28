#include "air_mspi_orbit.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read the given file.
//-----------------------------------------------------------------------

AirMspiOrbit::AirMspiOrbit(const std::string& Fname)
  : data(Fname)
{
}
