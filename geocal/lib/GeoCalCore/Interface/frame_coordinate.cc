#include "frame_coordinate.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void FrameCoordinate::print(std::ostream& Os) const
{
  Os << "Frame Coordinate: (" << line << ", " << sample << ")";
}
