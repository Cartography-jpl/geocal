#ifndef IMAGE_MATCHER_H
#define IMAGE_MATCHER_H
#include "raster_image.h"
#include "image_coordinate.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This class performs image matching, finding the location of a point
  in a reference image in a new image.
*******************************************************************/

class ImageMatcher : public Printable<ImageMatcher> {
public:

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageMatcher() {}

//-----------------------------------------------------------------------
/// Match a point found in the reference image with a point in the new
/// image. A initial guess in the new image is supplied. We return an
/// improved image in the new image along with the uncertainty if
/// Success is true, otherwise the match failed.
///
/// The Diagnostic argument is used to indicate why a match failed.
/// For many purposes you can ignore this, because you need to know
/// detailed information about the specific ImageMatcher that a user passed
/// to a function and what the diagnostic codes mean. However, for
/// some purposes the failure reason can be useful (e.g., tuning
/// parameters of a LsmMatcher).
//-----------------------------------------------------------------------

  virtual void match(const RasterImage& Ref, const RasterImage&
		     New, const ImageCoordinate& Ref_loc, const
		     ImageCoordinate& New_guess, 
		     ImageCoordinate& New_res,
		     double& Line_sigma, double& Sample_sigma,
		     bool& Success,
		     int* Diagnostic = 0) const = 0;  

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};

}
#endif

