#include "frame_coordinate.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void FrameCoordinate::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(FrameCoordinate);
  ar & GEOCAL_NVP(line) & GEOCAL_NVP(sample);
}

template<class Archive>
void FrameCoordinateWithDerivative::serialize(Archive & ar, 
					      const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(FrameCoordinateWithDerivative);
  ar & GEOCAL_NVP(line) & GEOCAL_NVP(sample);
}

GEOCAL_IMPLEMENT(FrameCoordinate);
GEOCAL_IMPLEMENT(FrameCoordinateWithDerivative);
#endif

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void FrameCoordinate::print(std::ostream& Os) const
{
  Os << "Frame Coordinate: (" << line << ", " << sample << ")";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void FrameCoordinateWithDerivative::print(std::ostream& Os) const
{
  Os << "Frame Coordinate: (" << line << ", " << sample << ")";
}
