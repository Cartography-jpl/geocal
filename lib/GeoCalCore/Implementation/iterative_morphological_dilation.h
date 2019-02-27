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
  enum FillOrder {C_ORDER=0};
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
				 const blitz::Array<bool, 2>& Mask,
				 const blitz::Array<double, 2>& Kernel,
				 FillOrder Fill_order = C_ORDER);
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
/// Order that we fill in values for a iteration of the fill.
//-----------------------------------------------------------------------

  FillOrder fill_order() const { return fill_order_;}

//-----------------------------------------------------------------------
/// The iteration count for filling in the data.
//-----------------------------------------------------------------------
  int iteration_count() const {return iteration_count_;}

  bool fill_iteration();
  void fill_missing_data();
  double neighborhood_average(int i, int j) const;
  
  blitz::Array<unsigned short int, 2> masked_neighbor_count() const;
  virtual void print(std::ostream& Os) const;
private:
  bool fill_iteration_c_order(const blitz::Array<unsigned short int, 2>& mcount);
  blitz::Array<double, 2> filled_image_;
  blitz::Array<bool, 2> filled_mask_;
  blitz::Array<double, 2> kernel_;
  FillOrder fill_order_;
  int iteration_count_;
  IterativeMorphologicalDilation() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IterativeMorphologicalDilation);
#endif
  
  
