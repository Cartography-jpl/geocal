#include "surface_image_to_image_match.h"
#include "image_ground_connection.h"
#include "igc_map_projected.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor, where we set up for doing map projection on
/// demand. Note that we only get the resolution and map projection
/// from the Map_info, we adjust the cover to work over the full
/// images Igc1 and Igc2.
//-----------------------------------------------------------------------

SurfaceImageToImageMatch::SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const MapInfo& Map_info,
   const boost::shared_ptr<ImageMatcher>& Matcher)
    : igc1(Igc1), igc2(Igc2), matcher_(Matcher), map_project_on_demand_(true)
{
  MapInfo mi = igc1->cover(Map_info).map_union(igc2->cover(Map_info));
  simg1.reset(new IgcMapProjected(mi, igc1, -1, false));
  simg2.reset(new IgcMapProjected(mi, igc2, -1, false));
  pix_fact = mi.resolution_meter() / igc1->resolution_meter();
}

//-----------------------------------------------------------------------
/// Constructor, where we supply the already map projected data.
//-----------------------------------------------------------------------
      
SurfaceImageToImageMatch::SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<RasterImage>& Surface_image1,
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const boost::shared_ptr<RasterImage>& Surface_image2,
   const boost::shared_ptr<ImageMatcher>& Matcher)
    : igc1(Igc1), igc2(Igc2), matcher_(Matcher), map_project_on_demand_(false),
      simg1(Surface_image1), simg2(Surface_image2)
{
  pix_fact = simg1->map_info().resolution_meter() / igc1->resolution_meter();
}

//-----------------------------------------------------------------------
/// Match point Ic1 in the first image with the second image. We
/// return the location in the second image along with the uncertainty
/// if Success is true, otherwise the match failed. 
///
/// Note that the line and sample sigma are only approximately
/// calculated. We have the line and sample sigma on the surface, and
/// we scale this by the approximate number of image pixels in each
/// surface pixel.
///
/// The Diagnostic argument is used to indicate why a match failed.
/// For many purposes you can ignore this, because you need to know
/// detailed information about the specific ImageMatcher that a user passed
/// to a function and what the diagnostic codes mean. However, for
/// some purposes the failure reason can be useful (e.g., tuning
/// parameters of a LsmMatcher).
//-----------------------------------------------------------------------

void SurfaceImageToImageMatch::match
(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  boost::shared_ptr<GroundCoordinate> gc = igc1->ground_coordinate(Ic1);
  ImageCoordinate sic1 = simg1->coordinate(*gc);
  ImageCoordinate sic2_guess = simg2->coordinate(*gc);
  ImageCoordinate sic2;
  matcher_->match(*simg1, *simg2, sic1, 
		  sic2_guess, sic2, Line_sigma, Sample_sigma, Success,
		  Diagnostic);
  if(Success) {
    Line_sigma = std::max(Line_sigma, Sample_sigma) * pix_fact;
    Sample_sigma = Line_sigma;
    Ic2 = igc2->image_coordinate(*simg2->ground_coordinate(sic2, igc2->dem()));
  }
}

//-----------------------------------------------------------------------
/// Variation of match where we supply the ground location to start
/// with. This can be faster than match if the particular
/// ImageGroundConnection has a ground_coordinate that is expensive
/// (typically ground_coordinate takes more time than
/// image_coordinate).
///
/// Note that you don't need to worry about including height in Gc,
/// only the latitude and longitude is important (however the height
/// doesn't hurt either).
//-----------------------------------------------------------------------

void SurfaceImageToImageMatch::match_surf
(const GroundCoordinate& Gc, 
 ImageCoordinate& Ic1, ImageCoordinate& Ic2,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  ImageCoordinate sic1 = simg1->coordinate(Gc);
  ImageCoordinate sic2_guess = simg2->coordinate(Gc);
  ImageCoordinate sic2;
  matcher_->match(*simg1, *simg2, sic1, 
		  sic2_guess, sic2, Line_sigma, Sample_sigma, Success,
		  Diagnostic);
  if(Success) {
    Line_sigma = std::max(Line_sigma, Sample_sigma) * pix_fact;
    Sample_sigma = Line_sigma;
    Ic1 = igc1->image_coordinate(*simg1->ground_coordinate(sic1, igc1->dem()));
    Ic2 = igc2->image_coordinate(*simg2->ground_coordinate(sic2, igc2->dem()));
  }
}
