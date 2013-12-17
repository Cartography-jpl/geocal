#ifndef GEOMETRIC_MODEL_H
#define GEOMETRIC_MODEL_H
#include "printable.h"
#include "image_coordinate.h"

namespace GeoCal {
/****************************************************************//**
  This supplies a geometric model that can be used to deform an image,
  e.g., resample an image to match to geometry of another reference
  image. 
*******************************************************************/
class GeometricModel : public Printable<GeometricModel> {
public:
  virtual ~GeometricModel() {}

//-----------------------------------------------------------------------
/// Map image coordinates to an underlying set of image coordinates.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const ImageCoordinate& Resampled_ic)
    const = 0;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Geometric model"; }
};

}
#endif

