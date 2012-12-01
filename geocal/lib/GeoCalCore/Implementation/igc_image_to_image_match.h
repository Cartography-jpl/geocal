#ifndef IGC_IMAGE_TO_IMAGE_MATCH_H
#define IGC_IMAGE_TO_IMAGE_MATCH_H
#include "image_to_image_match.h"
#include "image_coordinate.h"
#include "image_matcher.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  ImageToImageMatch where the images and prediction comes from
  ImageGroundConnection objects.
*******************************************************************/

class IgcImageToImageMatch: public ImageToImageMatch {
public:
  IgcImageToImageMatch(const boost::shared_ptr<ImageGroundConnection>& Igc1, 
		       const boost::shared_ptr<ImageGroundConnection>& Igc2,
		       const boost::shared_ptr<ImageMatcher>& Matcher)
    : igc1(Igc1), igc2(Igc2), matcher_(Matcher) {}
  virtual ~IgcImageToImageMatch() {}
  virtual void match(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
		     double& Line_sigma, double& Sample_sigma,
		     bool& Success, int* Diagnostic = 0) const;
  virtual void print(std::ostream& Os) const { Os << "IgcImageToImageMatcher"; }

//-----------------------------------------------------------------------
/// ImageGroundConnection for image 1.
//-----------------------------------------------------------------------

  const boost::shared_ptr<ImageGroundConnection>&
  image_ground_connection1() const { return igc1; }

//-----------------------------------------------------------------------
/// ImageGroundConnection for image 2.
//-----------------------------------------------------------------------

  const boost::shared_ptr<ImageGroundConnection>&
  image_ground_connection2() const { return igc2; }

//-----------------------------------------------------------------------
/// Matcher we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<ImageMatcher>&
  matcher() const {return matcher_;}
private:
  boost::shared_ptr<ImageGroundConnection> igc1, igc2;
  boost::shared_ptr<ImageMatcher> matcher_;
};
}
#endif
