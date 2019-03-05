#include "iterative_morphological_dilation.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IterativeMorphologicalDilation::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IterativeMorphologicalDilation);
  ar & GEOCAL_NVP_(filled_image) & GEOCAL_NVP_(filled_mask)
    & GEOCAL_NVP_(kernel) & GEOCAL_NVP_(frontier_fill_order)
    & GEOCAL_NVP_(prediction_type) & GEOCAL_NVP_(iteration_count)
    & GEOCAL_NVP_(sigma);
}

GEOCAL_IMPLEMENT(IterativeMorphologicalDilation);
#endif

//-----------------------------------------------------------------------
/// Constructor.
///
/// The Mask is "true" where we don't have Image data and wish to fill
/// in data (i.e., this is the same sense as ImageMask).
///
/// Right now, this just works with blitz arrays. We could extend this
/// to work with RasterImage and ImageMask. But this is often called
/// from python, which interacts better with blitz arrays (which map
/// to numpy arrays in python).
//-----------------------------------------------------------------------

IterativeMorphologicalDilation::IterativeMorphologicalDilation
(const blitz::Array<double, 2>& Image,
 const blitz::Array<bool, 2>& Mask,
 int Window_size, double Sigma, PredictionType Prediction_type,
 FrontierFillOrder Frontier_fill_order)
: filled_image_(Image.copy()),
  filled_mask_(Mask.copy()),
  kernel_(Window_size, Window_size),
  frontier_fill_order_(Frontier_fill_order),
  prediction_type_(Prediction_type),
  iteration_count_(0),
  sigma_(Sigma)
{
  if(Image.rows() != Mask.rows() ||
     Image.cols() != Mask.cols())
    throw Exception("Image and Mask need to be the same size");
  if(kernel_.rows() % 2 != 1 || kernel_.cols() % 2 != 1)
    throw Exception("The Window_size needs to be an odd size");
  if(sigma_ < 0)
    sigma_ = window_size() / 6.4;
  if(prediction_type_ == FLAT_WEIGHTED_AVERAGE ||
     prediction_type_ == NEIGBORHOOD_MEDIAN)
    kernel_ = 1.0;
  else if(prediction_type_ == GAUSSIAN_WEIGHTED_AVERAGE) {
    int whs = (window_size() - 1) / 2;
    for(int i = -whs; i <= whs; ++i)
      for(int j = -whs; j <= whs; ++j)
	kernel_(i+whs,j+whs) = exp(-(i*i + j*j) / (2*sigma_*sigma_));
  } else
    throw Exception("Unrecognized prediction_type_");
}

//-----------------------------------------------------------------------
/// Dilate the mask with a simple 3x3 kernel of all 1's. Subtract the
/// original mask - so this returns nonzero for all the new "frontier"
/// pixels. Because it is useful, fill the nonzero values with a count
/// of neighbors in the original mask (this can be useful to fill in
/// points with the most neighbors first in an iteration). So this
/// returns all masked pixels that have at least one immediate neighbor.
//-----------------------------------------------------------------------

blitz::Array<unsigned short int, 2>
IterativeMorphologicalDilation::frontier_pixel_neighbor_count() const
{
  Array<unsigned short int, 2> res(filled_mask_.shape());
  for(int i = 0; i < res.rows(); ++i)
    for(int j = 0; j < res.cols(); ++j)
      if(filled_mask_(i,j)) {
	Range r1(std::max(i-1,0), std::min(i+1,res.rows()-1));
	Range r2(std::max(j-1,0), std::min(j+1,res.cols()-1));
	res(i, j) = blitz::count(!filled_mask_(r1,r2));
      } else
	res(i,j) = 0;
  return res;
}

//-----------------------------------------------------------------------
/// Do an iteration of filling in the data. Return true if there was
/// data to modify, false otherwise (e.g., we have completed filling
/// in the data).
//-----------------------------------------------------------------------

bool IterativeMorphologicalDilation::fill_iteration()
{
  blitz::Array<unsigned short int, 2>  mcount = frontier_pixel_neighbor_count();
  bool any_change = false;
  if(frontier_fill_order_ == C_ORDER)
    any_change = fill_iteration_c_order(mcount);
  if(any_change)
    ++iteration_count_;
  return any_change;
}

//-----------------------------------------------------------------------
/// Iteratively fill in missing data until everything is filled. This
/// just runs fill_iteration() until there is nothing left.
//-----------------------------------------------------------------------

void IterativeMorphologicalDilation::fill_missing_data()
{
  while(fill_iteration())
    ;
}

//-----------------------------------------------------------------------
/// Do fill_iteration in C order.
//-----------------------------------------------------------------------
  
bool IterativeMorphologicalDilation::fill_iteration_c_order
(const blitz::Array<unsigned short int, 2>& mcount)
{
  bool any_change = false;
  for(int i = 0; i < mcount.rows(); ++i)
    for(int j = 0; j < mcount.cols(); ++j)
      if(mcount(i,j) > 0) {
	filled_image_(i,j) = predicted_value(i,j);
	filled_mask_(i,j) = false;
	any_change = true;
      }
  return any_change;
}
  
//-----------------------------------------------------------------------
/// Predicted value for the given pixel. We only include data
/// that as filled_mask_ false, and we normalize by the portion of the
/// kernel included.
//-----------------------------------------------------------------------

double IterativeMorphologicalDilation::predicted_value(int i, int j) const
{
  int krhs = (kernel_.rows() - 1) / 2;
  int kchs = (kernel_.cols() - 1) / 2;
  std::vector<double> data;
  double sum = 0;
  double ksum = 0;
  int count = 0;
  for(int ii = -krhs; ii <= krhs; ++ii)
    if(i + ii > 0 && i + ii < filled_mask_.rows())
      for(int jj = -kchs; jj <= kchs; ++jj)
	if(j + jj > 0 && j + jj < filled_mask_.cols() &&
	   !filled_mask_(i+ii,j+jj)) {
	  ++count;
	  if(prediction_type_ == NEIGBORHOOD_MEDIAN) {
	    data.push_back(filled_image_(i+ii,j+jj));
	  } else {
	    sum += kernel_(ii+krhs,jj+kchs) * filled_image_(i+ii,j+jj);
	    ksum += kernel_(ii+krhs,jj+kchs);
	  }
	}
  if(count == 0)
    throw Exception("No data found in neighborhood");
  if(prediction_type_ == NEIGBORHOOD_MEDIAN) {
    std::sort(data.begin(), data.end());
    if(count % 2 == 0)
      return (data[count / 2 - 1] + data[count / 2]) / 2.0;
    return data[count / 2];
  }
  return sum / ksum;
}

void IterativeMorphologicalDilation::print(std::ostream& Os) const
{
  Os << "IterativeMorphologicalDilation:\n"
     << "  image:          " << filled_image_.rows() << " x "
     << filled_image_.cols() << "\n"
     <<  " kernel:          " << kernel_.rows() << " x "
     << kernel_.cols() << "\n"
     <<  " frontier_fill_order:      "
     << (frontier_fill_order_ == C_ORDER ? "C_ORDER" : "Unknown") << "\n"
     <<  " iteration_count: " << iteration_count_ << "\n";
}
