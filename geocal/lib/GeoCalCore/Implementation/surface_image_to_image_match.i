// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "surface_image_to_image_match.h"
%}

%geocal_shared_ptr(GeoCal::SurfaceImageToImageMatch);
namespace GeoCal {

class SurfaceImageToImageMatch : public ImageToImageMatch {
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
  virtual void match(const ImageCoordinate& Ic1, 
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
  %pythoncode {
@classmethod
def pickle_format_version(cls):
  return 1

def __reduce__(self):
    if(self.map_project_on_demand):
      return _new_from_init, (self.__class__, 1, self.image_ground_connection1,
			      self.image_ground_connection2, 
			      self.surface_image1.map_info(),
			      self.matcher)
    else:
      return _new_from_init, (self.__class__, 1, self.image_ground_connection1,
			      self.surface_image1,
			      self.image_ground_connection2, 
			      self.surface_image2,
			      self.matcher)
  }
};
}



