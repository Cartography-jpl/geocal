#include "forstner_feature_detector.h"
#include <blitz/array.h>

using namespace GeoCal;

//-----------------------------------------------------------------------
// Stencils used to calculate Roberts gradient.
//-----------------------------------------------------------------------

BZ_DECLARE_STENCIL2(roberts_a, A, B)
  A = B(0, 0) - B(1, 1);
BZ_END_STENCIL

BZ_DECLARE_STENCIL2(roberts_b, A, B)
  A = B(0, 1) - B(1, 0);
BZ_END_STENCIL

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

ForstnerFeatureDetector::ForstnerFeatureDetector(
     int Basic_ip_large_number, int
     Maximum_weight_threshold, int Forstner_max_window_large_size, 
     int Forstner_max_window_small_size, int Forstner_window_size,
     double Roundness_threshold, double Weight_threshold, double
     Basic_robert_threshold)
: basic_ip_large_number_(Basic_ip_large_number),
  basic_robert_threshold_(Basic_robert_threshold),
  forstner_max_window_large_size_(Forstner_max_window_large_size),
  forstner_max_window_small_size_(Forstner_max_window_small_size),
  forstner_window_size_(Forstner_window_size),
  maximum_weight_threshold_(Maximum_weight_threshold),
  roundness_threshold_(Roundness_threshold),
  weight_threshold_(Weight_threshold)
{
}

//-----------------------------------------------------------------------
/// Go through a RasterImage, and return a list of InterstPoints for
/// it. 
//-----------------------------------------------------------------------

std::vector<InterestPoint>
ForstnerFeatureDetector::interest_point_detect(const RasterImage& Img) const
{
  using namespace blitz;
  firstIndex i1;
  secondIndex i2;

//-----------------------------------------------------------------------
/// Put all of the raster data into a blitz::array.
//-----------------------------------------------------------------------

  Array<int, 2> idata = Img.read(0, 0, Img.number_line(), 
				      Img.number_sample());
  Array<double, 2> data(idata.shape());
  data = cast<double>(idata);

//-----------------------------------------------------------------------
/// Compute the Robert gradient
//-----------------------------------------------------------------------

  Array<double, 2> grad_a(data.shape());
  Array<double, 2> grad_b(data.shape());
  grad_a = 0;
  grad_b = 0;
  blitz::applyStencil(roberts_a(), grad_a, data);
  blitz::applyStencil(roberts_b(), grad_b, data);

//-----------------------------------------------------------------------
// Compute normal of gradients.
//-----------------------------------------------------------------------

  Array<double, 2> normal(sqrt(sqr(grad_a) + sqr(grad_b)));
  double normal_mean = mean(normal);

//-----------------------------------------------------------------------
// Apply Roberts threshold to detect basic interest points.
//-----------------------------------------------------------------------

  double threshold = normal_mean * basic_robert_threshold_;
  Array<bool, 2> ip_mask(normal >= threshold);
  int number_basic_ip = count(ip_mask);

//-----------------------------------------------------------------------
// Calculate Forstner interest value.
//-----------------------------------------------------------------------

  int offset_line = forstner_window_size_ / 2;
  int offset_sample = offset_line;
  Array<double, 2> iline(grad_a.shape());
  Array<double, 2> isample(grad_a.shape());
  Array<double, 2> weight(grad_a.shape());
  Array<double, 2> roundness(grad_a.shape());
  iline = 0; isample = 0; weight = 0; roundness = 0;
  double weight_mean = 0;
  int weight_count = 0;
  for(int i = 0; i < grad_a.extent(0) - (forstner_window_size_ - 1); ++i)
    for(int j = 0; j < grad_b.extent(1) - (forstner_window_size_ - 1); ++j) {
      int line = i + offset_line;
      int sample = j + offset_sample;
      if(ip_mask(line, sample)) {
	Range r1(i, i + forstner_window_size_ - 1);
	Range r2(j, j + forstner_window_size_ - 1);
	Array<double, 2> ga(grad_a(r1, r2));
	Array<double, 2> gb(grad_b(r1, r2));
	double n11 = sum(sqr(ga));
	double n12 = sum(ga * gb);
	double n22 = sum(sqr(gb));
	double c1 = sum(sqr(ga) * (i1 + i) + ga * gb * (i2 + j));
	double c2 = sum(sqr(gb) * (i2 + j) + ga * gb * (i1 + i));
	double trn = n11 + n22;
	double detn = n11 * n22 - n12 * n12;
	weight(line, sample) = detn / trn;
	roundness(line, sample) = weight(line, sample) / trn;
	weight_mean += weight(line, sample);
	++weight_count;
	if(detn != 0) {
	  iline(line, sample)  = (n22 * c1 - n12 * c2) / detn;
	  isample(line, sample) = (n11 * c2 - n12 * c1) / detn;
	}
      }
    }
  if(weight_count != 0)
    weight_mean /= weight_count;

//-----------------------------------------------------------------------
// If we have enough points and weight, use a large max window size,
// otherwise use a smaller one.
//-----------------------------------------------------------------------

  int local_max_win = (weight_mean > maximum_weight_threshold_ &&
		       number_basic_ip > basic_ip_large_number_ ?
		       forstner_max_window_large_size_ :
		       forstner_max_window_small_size_);
  offset_line = local_max_win / 2;
  offset_sample = local_max_win / 2;

//-----------------------------------------------------------------------
// Toss out points that don't meet the weight and roundness criteria.
//-----------------------------------------------------------------------
  
  weight = where(weight < weight_threshold_ * weight_mean ||
		 roundness < roundness_threshold_, 0.0, weight);

//-----------------------------------------------------------------------
// Now, go through all of the points and pick out the points that have
// the maximum weight in the local neighborhood. These become our
// interest points. 
//-----------------------------------------------------------------------

  std::vector<InterestPoint> res;
  for(int i = 0; i < weight.extent(0) - (local_max_win - 1); ++i)
    for(int j = 0; j < weight.extent(1) - (local_max_win - 1); ++j) {
      int line = i + offset_line;
      int sample = j + offset_sample;
      if(weight(line, sample) > 0.0) {
	Range r1(i, i + local_max_win - 1);
	Range r2(j, j + local_max_win - 1);
	TinyVector<int, 2> m = maxIndex(weight(r1, r2));
	if(m[0] == offset_line && m[1] ==offset_sample) {
	  res.push_back(InterestPoint(
            ImageCoordinate(iline(line, sample), isample(line, sample)),
	    weight(line, sample) / weight_mean));
	}
      }
    }
  return res;
}


//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void ForstnerFeatureDetector::print(std::ostream& Os) const
{
  Os << "Forstner Feature Detector\n"
     << "  Basic IP Large Number: " << basic_ip_large_number_ << "\n"
     << "  Basic Robert Threshold: " << basic_robert_threshold_ << "\n"
     << "  Max window large size:  " << forstner_max_window_large_size_ << "\n"
     << "  Max window small size:  " << forstner_max_window_small_size_ << "\n"
     << "  Window size:            " << forstner_window_size_ << "\n"
     << "  Maximum Weight Threshold: " << maximum_weight_threshold_ << "\n"
     << "  Roundness Threshold:    " << roundness_threshold_ << "\n"
     << "  Weight Threshold:       " << weight_threshold_ << "\n";
}
