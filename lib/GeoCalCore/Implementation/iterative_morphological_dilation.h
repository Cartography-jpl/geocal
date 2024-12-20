#ifndef ITERATIVE_MORPHOLOGICAL_DILATION
#define ITERATIVE_MORPHOLOGICAL_DILATION
#include <blitz/array.h>
#include "printable.h"
#include <list>
#if __cplusplus >= 201103L
#include <random>
#else
#include <boost/random/mersenne_twister.hpp>
#endif

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
#if __cplusplus >= 201103L
  typedef std::mt19937 RandType;
#else
  typedef boost::random::mt19937 RandType;
#endif
  enum FrontierFillOrder {C_ORDER=0, RANDOM_ORDER=1, MOST_NEIGHBORS_FIRST=2};
  enum PredictionType { FLAT_WEIGHTED_AVERAGE=0,
			GAUSSIAN_WEIGHTED_AVERAGE=1,
			NEIGBORHOOD_MEDIAN=2 };
  struct FrontierPixel {
    FrontierPixel(int I, int J, int Count,
		  std::list<std::pair<int, int> >::const_iterator It)
      : i(I), j(J), count(Count), it(It) {}
    int i, j, count;
    std::list<std::pair<int, int> >::const_iterator it;
  };
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
		 const blitz::Array<bool, 2>& Mask,
		 int Window_size = 3,
		 double Sigma = -1,	 
		 PredictionType Prediction_type = GAUSSIAN_WEIGHTED_AVERAGE,
		 FrontierFillOrder Frontier_fill_order = RANDOM_ORDER);
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
  
  std::vector<FrontierPixel> frontier_pixel_find(int num) const;
  virtual void print(std::ostream& Os) const;
  static RandType rand_gen;
  static void set_random_seed(unsigned int S) {rand_gen.seed(S);}
private:
  bool fill_iteration_c_order(std::vector<FrontierPixel>& Fp);
  bool fill_iteration_most_neighbors_first(std::vector<FrontierPixel>& Fp);
  bool fill_iteration_random(std::vector<FrontierPixel>& Fp);
  blitz::Array<double, 2> filled_image_;
  blitz::Array<bool, 2> filled_mask_;
  mutable bool need_to_fill;
  mutable std::list<std::pair<int, int> > to_fill;
  blitz::Array<double, 2> kernel_;
  FrontierFillOrder frontier_fill_order_;
  PredictionType prediction_type_;
  int iteration_count_;
  double sigma_;
  IterativeMorphologicalDilation() : need_to_fill(true) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IterativeMorphologicalDilation);
#endif
  
  
