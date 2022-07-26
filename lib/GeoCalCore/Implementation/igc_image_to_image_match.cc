#include "igc_image_to_image_match.h"
#include "image_ground_connection.h"
using namespace GeoCal;

void IgcImageToImageMatch::match
(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  ImageCoordinate ic2_guess;
  if(igc1->image_mask()->mask((int) Ic1.line, (int) Ic1.sample)) {
    Success = false;
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }
  try {
    ic2_guess =
      igc2->image_coordinate(*igc1->ground_coordinate(Ic1));
  } catch (const Exception& E) {
    // Allow to fail, the image_coordinate calculate might have
    // problems (e.g., out of range). In that case, we just have a
    // failed image matching rather than an error.
    Success = false;
    if(Diagnostic)
      *Diagnostic = IMAGE_COOR_FAILED;
    return;
  }
  if(igc2->image_mask()->mask((int) ic2_guess.line, (int) ic2_guess.sample)) {
    Success = false;
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }
  matcher_->match_mask(*igc1->image(), *igc1->image_mask(), *igc2->image(), 
		       *igc2->image_mask(),
		       Ic1, 
		       ic2_guess, Ic2, Line_sigma, Sample_sigma, Success,
		       Diagnostic);
}
