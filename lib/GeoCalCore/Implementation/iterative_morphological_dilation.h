#ifndef ITERATIVE_MORPHOLOGICAL_DILATION
#define ITERATIVE_MORPHOLOGICAL_DILATION
#include <blitz/array.h>
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple algorithm to fill in missing pixels in an image.
  This looks at all the pixels that are on the "edge" of the image -
  so pixels that aren't filled in but have at least one neighbor. We
  then fill in those pixels, and repeat the process iteratively. This
  is sometimes referred to as the "grass fire" algorithm.
*******************************************************************/
class IterativeMorphologicalDilation : public Printable<IterativeMorphologicalDilation> {
public:
  enum FrontierFillOrder {C_ORDER=0, MOST_NEIGHBORS_FIRST=1};
  enum PredictionType { FLAT_WEIGHTED_AVERAGE=0,
			GAUSSIAN_WEIGHTED_AVERAGE=1,
			NEIGBORHOOD_MEDIAN=2 };
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
		 const blitz::Array<bool, 2>& Mask,
		 int Window_size = 3,
		 double Sigma = -1,	 
		 PredictionType Prediction_type = GAUSSIAN_WEIGHTED_AVERAGE,
		 FrontierFillOrder Frontier_fill_order = C_ORDER);
  virtual ~IterativeMorphologicalDilation() {}

//-----------------------------------------------------------------------
/// Image that has been filled in for missing data.
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& filled_image() const
  {return filled_image_;}

//-----------------------------------------------------------------------
/// Mask for filled_image.
//-----------------------------------------------------------------------

  const blitz::Array<bool, 2>& filled_mask() const
  {return filled_mask_;}

//-----------------------------------------------------------------------
/// Kernel used for neighborhood averaging to fill in missing data.
//-----------------------------------------------------------------------
  const blitz::Array<double, 2>& kernel() const { return kernel_; }

//-----------------------------------------------------------------------
/// Order that we fill the frontier pixels in an iteration.
//-----------------------------------------------------------------------

  FrontierFillOrder frontier_fill_order() const { return frontier_fill_order_;}

//-----------------------------------------------------------------------
/// The iteration count for filling in the data.
//-----------------------------------------------------------------------
  int iteration_count() const {return iteration_count_;}

//-----------------------------------------------------------------------
/// Size of kernel to use
//-----------------------------------------------------------------------
  int window_size() const {return kernel_.rows(); }

//-----------------------------------------------------------------------
/// Gaussian sigma to use.  
//-----------------------------------------------------------------------
  double sigma() const { return sigma_;}

//-----------------------------------------------------------------------
/// Type of prediction to use
//-----------------------------------------------------------------------
  PredictionType prediction_type() const {return prediction_type_; }
  
  bool fill_iteration();
  void fill_missing_data();
  double predicted_value(int i, int j) const;
  
  blitz::Array<unsigned short int, 2> frontier_pixel_neighbor_count() const;
  virtual void print(std::ostream& Os) const;
private:
  bool fill_iteration_c_order(const blitz::Array<unsigned short int, 2>& mcount);
  bool fill_iteration_most_neighbors_first(const blitz::Array<unsigned short int, 2>& mcount);
  blitz::Array<double, 2> filled_image_;
  blitz::Array<bool, 2> filled_mask_;
  blitz::Array<double, 2> kernel_;
  FrontierFillOrder frontier_fill_order_;
  PredictionType prediction_type_;
  int iteration_count_;
  double sigma_;
  IterativeMorphologicalDilation() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IterativeMorphologicalDilation);
#endif
  
  
