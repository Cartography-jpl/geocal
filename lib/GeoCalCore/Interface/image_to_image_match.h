#ifndef IMAGE_TO_IMAGE_MATCH_H
#define IMAGE_TO_IMAGE_MATCH_H
#include "raster_image.h"
#include "image_coordinate.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This class find matches between two images. This is similar to the
  class ImageMatcher, but in addition to doing the matching it also
  keeps track of which two images we are matching, and how we do the
  initial guess for the location in the second image.
*******************************************************************/
class ImageToImageMatch: public Printable<ImageToImageMatch> {
public:
  virtual ~ImageToImageMatch() {}
//-----------------------------------------------------------------------
/// Match point Ic1 in the first image with the second image. We
/// return the location in the second image along with the uncertainty
/// if Success is true, otherwise the match failed. 
///
/// The Diagnostic argument is used to indicate why a match failed.
/// For many purposes you can ignore this, because you need to know
/// detailed information about the specific ImageMatcher that a user passed
/// to a function and what the diagnostic codes mean. However, for
/// some purposes the failure reason can be useful (e.g., tuning
/// parameters of a LsmMatcher).
//-----------------------------------------------------------------------

  virtual void match(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
		     double& Line_sigma, double& Sample_sigma,
		     bool& Success, int* Diagnostic = 0) const = 0;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const { Os << "ImageToImageMatcher"; }
};
}
#endif
