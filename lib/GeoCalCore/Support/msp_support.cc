#include "geocal_internal_config.h"
#include "msp_support.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// This uses an image in a NITF file and the sensor attached to it
/// (e.g. RPC, RSM, SENSRB) to find the ground location for a
/// particular image point. This is meant for comparison with our own
/// GeoCal calculation, to make sure we are meeting whatever
/// assumptions BAE has in in MSP software.
//-----------------------------------------------------------------------

Ecr GeoCal::msp_terrain_point(const std::string& Fname,
			      const ImageCoordinate& Ic)
{
#ifdef HAVE_MSP
  std::cerr << "Hi there\n";
  return Ecr(1,2,3);
#else
  throw MspNotAvailableException();
#endif
}
