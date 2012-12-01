#ifndef SURFACE_IMAGE_TO_IMAGE_MATCH_H
#define SURFACE_IMAGE_TO_IMAGE_MATCH_H
#include "image_to_image_match.h"
#include "image_coordinate.h"
#include "image_matcher.h"

namespace GeoCal {
/****************************************************************//**
   This is an ImageToImageMatch that takes two image ground
   connections along with a MapInfo to project to. The data is matched
   at the surface, although the results are reported in image
   coordinates.

   The MapInfo is used to get the resolution and map projection, the
   actual coverage isn't important (e.g., the ulc and lrc). We make
   sure that the coverage works over the full image.

   Depending on your application, we can do the map projection only
   where we need an image patch for matching, or you can supply the
   full surface image.

   If you are collecting tiepoints over a large image, you usually
   want to do the map projection on the fly as needed, since we
   typically only map project a small part of the image.

   In some cases, you may be doing lots of points or simply already
   have the data map projected. In those cases, you can pass the
   RasterImage in. These should be map projected, with a MapInfo
   (e.g., geotiff label or whatever in the data).
*******************************************************************/

class SurfaceImageToImageMatch: public ImageToImageMatch {
public:
  SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const MapInfo& Map_info,
   const boost::shared_ptr<ImageMatcher>& Matcher);
  SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<RasterImage>& Surface_image1,
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const boost::shared_ptr<RasterImage>& Surface_image2,
   const boost::shared_ptr<ImageMatcher>& Matcher);
  virtual ~SurfaceImageToImageMatch() {}
  virtual void match(const ImageCoordinate& Ic1, ImageCoordinate& Ic2,
		     double& Line_sigma, double& Sample_sigma,
		     bool& Success, int* Diagnostic = 0) const;
  virtual void print(std::ostream& Os) const 
  { Os << "SurfaceImageToImageMatcher"; }

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

//-----------------------------------------------------------------------
/// Indicate if we are doing the map projection on demand, or if we 
/// already have the data map projected.
//-----------------------------------------------------------------------

  bool map_project_on_demand() const { return map_project_on_demand_; }

//-----------------------------------------------------------------------
/// Surface projected image 1.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>&
  surface_image1() const { return simg1; }

//-----------------------------------------------------------------------
/// Surface projected image 2.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>&
  surface_image2() const { return simg2; }

private:
  boost::shared_ptr<ImageGroundConnection> igc1, igc2;
  boost::shared_ptr<ImageMatcher> matcher_;
  bool map_project_on_demand_;
  boost::shared_ptr<RasterImage> simg1, simg2;
  // How many image pixels go into a surface pixel. Used to scale line
  // and sample sigma in image matcher.
  double pix_fact;
};
}
#endif
