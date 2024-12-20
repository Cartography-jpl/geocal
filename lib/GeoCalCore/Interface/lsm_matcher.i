// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "lsm_matcher.h"
#include "image_ground_connection.h"
%}
%base_import(image_matcher)

%geocal_shared_ptr(GeoCal::LsmMatcher);
namespace GeoCal {

class LsmMatcher: public ImageMatcher {
public:
  LsmMatcher(int Number_line = 21, int Number_sample = 21, int
     Border_size = 3, double Precision_goal = 0.0625, 
     double Precision_requirement = 0.2, 
     double Max_sigma = 0.5, double Rad_uncertainty_factor = 2.0,
     double Precision_min_geo_goal = 0.15, double
     Precision_min_rad_goal = 1);
  virtual void match_mask(const RasterImage& Ref, 
			  const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, 
			  const ImageCoordinate& New_guess, 
			  ImageCoordinate &OUTPUT,
			  double &OUTPUT, double &OUTPUT,
			  bool &OUTPUT, int *OUTPUT) const;
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(border_size, int)
  %python_attribute(precision_goal, double)
  %python_attribute(precision_requirement, double)
  %python_attribute(max_sigma, double)
  %python_attribute(rad_uncertainty_factor, double)
  %python_attribute(precision_min_geo_goal, double)
  %python_attribute(precision_min_rad_goal, double)
  %pickle_serialization();
};
}


// List of things "import *" will include
%python_export("LsmMatcher")
