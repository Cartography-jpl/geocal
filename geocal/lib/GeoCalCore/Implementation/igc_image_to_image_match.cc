#include "igc_image_to_image_match.h"
#include "image_ground_connection.h"
using namespace GeoCal;

void IgcImageToImageMatch::match
(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  ImageCoordinate ic2_guess =
    igc2->image_coordinate(*igc1->ground_coordinate(Ic1));
  matcher_->match(*igc1->image(), *igc2->image(), Ic1, 
		  ic2_guess, Ic2, Line_sigma, Sample_sigma, Success,
		  Diagnostic);
}
