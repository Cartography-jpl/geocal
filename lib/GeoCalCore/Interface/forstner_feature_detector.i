// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "forstner_feature_detector.h"
%}
%base_import(feature_detector)

%geocal_shared_ptr(GeoCal::ForstnerFeatureDetector);
namespace GeoCal {
class ForstnerFeatureDetector : public FeatureDetector {
public:
  ForstnerFeatureDetector(int Basic_ip_large_number = 1000, int
     Maximum_weight_threshold = 10000, int Forstner_max_window_large_size =
     9, int Forstner_max_window_small_size = 7, int Forstner_window_size = 3,
     double Roundness_threshold = 0.2, double Weight_threshold = 1.0, double
     Basic_robert_threshold = 1.2);
  virtual std::vector<InterestPoint>
  interest_point_detect(const RasterImage& Img) const;
  %python_attribute(basic_ip_large_number, int)  
  %python_attribute(maximum_weight_threshold, int)
  %python_attribute(forstner_max_window_large_size, int)
  %python_attribute(forstner_max_window_small_size, int)
  %python_attribute(forstner_window_size, int)
  %python_attribute(roundness_threshold, double)
  %python_attribute(weight_threshold, double)
  %python_attribute(basic_robert_threshold, double)
  %pickle_serialization();
};
}


