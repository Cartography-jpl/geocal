#ifndef FORSTNER_FEATURE_DETECTOR_H
#define FORSTNER_FEATURE_DETECTOR_H
#include "feature_detector.h"

namespace GeoCal {

/****************************************************************//**
  This class detects interest points in an image using Forstner
  feature detection.
*******************************************************************/

class ForstnerFeatureDetector : public FeatureDetector {
public:
  ForstnerFeatureDetector(int Basic_ip_large_number = 1000, int
     Maximum_weight_threshold = 10000, int Forstner_max_window_large_size =
     9, int Forstner_max_window_small_size = 7, int Forstner_window_size = 3,
     double Roundness_threshold = 0.2, double Weight_threshold = 1.0, double
     Basic_robert_threshold = 1.2);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ForstnerFeatureDetector() { }
  virtual std::vector<InterestPoint>
                   interest_point_detect(const RasterImage& Img) const;
  virtual void print(std::ostream& Os) const;
  int basic_ip_large_number() const {return basic_ip_large_number_;}
  int maximum_weight_threshold() const {return maximum_weight_threshold_;}
  int forstner_max_window_large_size() const 
  {return forstner_max_window_large_size_;}
  int forstner_max_window_small_size() const 
  {return forstner_max_window_small_size_;}
  int forstner_window_size() const { return forstner_window_size_;}
  double roundness_threshold() const {return roundness_threshold_;}
  double weight_threshold() const {return weight_threshold_;}
  double basic_robert_threshold() const {return basic_robert_threshold_;}
protected:
private:
  /// Number of basic IP for determing local maximum suppression window
  /// size. 

  int basic_ip_large_number_; 

  /// Threshold for selecting basic interest points.

  double basic_robert_threshold_;

  /// Window size when searching for local maximum.

  int forstner_max_window_large_size_;

  /// Window size when searching for local maximum.

  int forstner_max_window_small_size_;

  /// Forstner operator window size. Used to compute interest from a set of
  /// basic interest points.

  int forstner_window_size_;

  /// Weight threshold for determing local maximum suppression window size.

  int maximum_weight_threshold_;

  /// Threshold for detecting points.

  double roundness_threshold_;

  /// Threshold for detecting points.

  double weight_threshold_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(ForstnerFeatureDetector);
#endif

