#include "image_coordinate.h"	
#include <cmath>		
#include <sstream>		
using namespace GeoCal;

//-----------------------------------------------------------------------
///  Print an ImageCoordinate to a stream
//-----------------------------------------------------------------------

void ImageCoordinate::print(std::ostream& Os) const
{
  Os << "(" << line << ", " << sample << ")";
}

//-----------------------------------------------------------------------
///  Print an ImageCoordinateWithDerivative to a stream
//-----------------------------------------------------------------------

void ImageCoordinateWithDerivative::print(std::ostream& Os) const
{
  Os << "ImageCoordinateWithDerivative:\n"
     << "  Line:   " << line
     << "  Sample: " << sample;
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Test for the equality between two ImageCoordinates. This is done
/// with a tolerance, if the line and sample are with 0.01 of each
/// other than we consider the ImageCoordinates as equal.
//-----------------------------------------------------------------------

bool GeoCal::operator==(const ImageCoordinate& C1, const ImageCoordinate& C2)
{
  const double coor_tol = 0.01;
  return (fabs(C1.line   - C2.line)   < coor_tol &&
	  fabs(C1.sample - C2.sample) < coor_tol);
}

//-----------------------------------------------------------------------
///  Print an VicarImageCoordinate to a stream
//-----------------------------------------------------------------------

void VicarImageCoordinate::print(std::ostream& Os) const
{
  Os << "VICAR image coordinate (" << line << ", " << sample << ")";
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Test for the equality between two VicarImageCoordinates. This is done
/// with a tolerance, if the line and sample are with 0.01 of each
/// other than we consider the VicarImageCoordinates as equal.
//-----------------------------------------------------------------------

bool GeoCal::operator==(const VicarImageCoordinate& C1, const VicarImageCoordinate& C2)
{
  const double coor_tol = 0.01;
  return (fabs(C1.line   - C2.line)   < coor_tol &&
	  fabs(C1.sample - C2.sample) < coor_tol);
}
