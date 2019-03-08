// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "iterative_morphological_dilation.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::IterativeMorphologicalDilation);
namespace GeoCal {
class IterativeMorphologicalDilation : public GenericObject {
public:
  enum FrontierFillOrder {C_ORDER=0, MOST_NEIGHBORS_FIRST=1, RANDOM_ORDER=2};
  enum PredictionType { FLAT_WEIGHTED_AVERAGE=0,
			GAUSSIAN_WEIGHTED_AVERAGE=1,
			NEIGBORHOOD_MEDIAN=2 };
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
				 const blitz::Array<bool, 2>& Mask,
		 int Window_size = 3,
		 double Sigma = -1,	 
		 PredictionType Prediction_type = GAUSSIAN_WEIGHTED_AVERAGE,
		 FrontierFillOrder Frontier_fill_order = RANDOM_ORDER);
  std::string print_to_string() const;
  %python_attribute(filled_image, blitz::Array<double, 2>);
  %python_attribute(filled_mask, blitz::Array<bool, 2>);
  %python_attribute(kernel, blitz::Array<double, 2>);
  %python_attribute(frontier_fill_order, FrontierFillOrder);
  %python_attribute(iteration_count, int);
  %python_attribute(window_size, int);
  %python_attribute(sigma, double);
  %python_attribute(prediction_type, PredictionType);
  static void set_random_seed(unsigned int S);
  void fill_missing_data();
  double predicted_value(int i, int j) const;
  blitz::Array<unsigned short int, 2> frontier_pixel_neighbor_count() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("IterativeMorphologicalDilation")
