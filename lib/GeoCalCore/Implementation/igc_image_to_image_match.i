// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_image_to_image_match.h"
#include "image_ground_connection.h"
%}
%base_import(image_to_image_match)
%import "image_ground_connection.i"
%import "image_matcher.i"
%geocal_shared_ptr(GeoCal::IgcImageToImageMatch);
namespace GeoCal {
class IgcImageToImageMatch : public ImageToImageMatch {
public:
  IgcImageToImageMatch(const boost::shared_ptr<ImageGroundConnection>& Igc1, 
		       const boost::shared_ptr<ImageGroundConnection>& Igc2,
		       const boost::shared_ptr<ImageMatcher>& Match);
  virtual void match(const ImageCoordinate& Ic1, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const;  
  %python_attribute(image_ground_connection1, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(image_ground_connection2, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(matcher, boost::shared_ptr<ImageMatcher>)
  %pickle_init(1, self.image_ground_connection1,
	       self.image_ground_connection2, self.matcher)
};
}



