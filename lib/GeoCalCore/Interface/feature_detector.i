// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "feature_detector.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "raster_image.i"
%import "ground_mask.i"
%geocal_shared_ptr(GeoCal::InterestPoint);
%geocal_shared_ptr(GeoCal::FeatureDetector);
namespace GeoCal {
%rename("_interest_point_grid_raw") FeatureDetector::interest_point_grid_ptr;
class InterestPoint : public GenericObject {
public:
  InterestPoint();
  InterestPoint(const ImageCoordinate &Ic, double Weight);
  ImageCoordinate image_coordinate;
  double weight;
  std::string print_to_string() const;
  %extend {
// Ruby prefers defining this over defining operator<.
     int compare(const InterestPoint& I2) {
        if(*$self < I2)
         return -1;
        if(I2 < *$self)
         return 1;
        return 0;
     }
  }
  %pickle_serialization();
};

class FeatureDetector : public GenericObject {
public:
  FeatureDetector();
  virtual std::vector<InterestPoint>
                   interest_point_detect(const RasterImage& Img) const = 0;
  boost::shared_ptr<ImageCoordinate>
  interest_point_grid_ptr(const RasterImage& Img,
			  const GroundMask& M, int i, int j,
			  int Number_grid_line, int Number_grid_sample, 
			  int Border = 0) const;
  boost::shared_ptr<ImageCoordinate>
  interest_point_grid_ptr(const RasterImage& Img,int i, int j, 
	  int Number_grid_line, int Number_grid_sample, 
	  int Border = 0) const;
  std::string print_to_string() const;
};

}
%template(Vector_InterestPoint) std::vector<GeoCal::InterestPoint>;

// List of things "import *" will include
%python_export("InterestPoint", "Vector_InterestPoint", "FeatureDetector")

