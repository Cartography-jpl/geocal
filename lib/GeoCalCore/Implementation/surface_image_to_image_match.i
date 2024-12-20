// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "surface_image_to_image_match.h"
#include "image_ground_connection.h"
%}
%base_import(image_to_image_match)
%import "image_ground_connection.i"
%import "map_info.i"
%import "image_matcher.i"
%geocal_shared_ptr(GeoCal::SurfaceImageToImageMatch);
namespace GeoCal {

class SurfaceImageToImageMatch : public ImageToImageMatch {
public:
  SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const MapInfo& Map_info,
   const boost::shared_ptr<ImageMatcher>& Matcher,
   int Grid_spacing = 1);
  SurfaceImageToImageMatch
  (const boost::shared_ptr<ImageGroundConnection>& Igc1, 
   const boost::shared_ptr<RasterImage>& Surface_image1,
   const boost::shared_ptr<ImageGroundConnection>& Igc2,
   const boost::shared_ptr<RasterImage>& Surface_image2,
   const boost::shared_ptr<ImageMatcher>& Matcher);
  virtual void match(const ImageCoordinate& Ic1, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const;  
  void match_surf(const GroundCoordinate& Gc, 
		  ImageCoordinate &OUTPUT,
		  ImageCoordinate &OUTPUT,
		  double &OUTPUT, double &OUTPUT,
		  bool &OUTPUT, int *OUTPUT) const;  
  %python_attribute(image_ground_connection1, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(image_ground_connection2, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(matcher, boost::shared_ptr<ImageMatcher>)
  %python_attribute(map_project_on_demand, bool)
  %python_attribute(surface_image1, boost::shared_ptr<RasterImage>)
  %python_attribute(surface_image2, boost::shared_ptr<RasterImage>)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("SurfaceImageToImageMatch")
