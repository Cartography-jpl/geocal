#include "misc_array_function.h"
#include "geocal_exception.h"
#include "igc_ray_caster.h"
#include "geocal_static_sort.h"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
// The median calculation in array_local_median is usually a small
// number of values that we need to sort/find median. This function handles
// this case, falling back to the standard std::nth_element if we have
// a larger value
//-----------------------------------------------------------------------

inline double median_calc(std::vector<double>& d)
{
  if(d.size() <= 30) {
    static_sort(d);
    if(d.size() % 2 == 1) {
      int n = (d.size() - 1)  / 2;
      return d[n];
    } else {
      int n = d.size() / 2 - 1;
      return (d[n] + d[n+1]) / 2;
    }
  } else {
    if(d.size() % 2 == 1) {
      int n = (d.size() - 1)  / 2;
      std::nth_element(d.begin(), d.begin() + n,
		       d.end());
      return d[n];
    } else {
      int n = d.size() / 2 - 1;
      std::nth_element(d.begin(), d.begin() + n,
		       d.end());
      double t1 = d[n];
      // This is the n+1 element. Data is partially sorted, to all
      // the values in d.begin() + n + 1, d.end()
      // are >= t1
      double t2 = *std::min_element(d.begin() + n + 1, d.end());
      return (t1 + t2) / 2;
    }
  }
}

//-----------------------------------------------------------------------
/// This calculates the local median of a 2d array, using a filter of
/// the given number of rows and cols. The number of rows and columns
/// should be an odd number.  At the edges of the array, we just go as
/// far as we can - so the window is a smaller size at the edges.
///
/// Edge handling describes how to handle data at the edges of the
/// array. For ARRAY_LOCAL_MEDIAN_TRUNCATE (the default), then at the
/// edges of the array we just go as far as we can - so the window is
/// a smaller size at the edges. For ARRAY_LOCAL_MEDIAN_ZEROPAD, we
/// instead zero fill the edges (this matches the behavior of the
/// matlab routine medfilt_1d.
///
/// For ARRAY_LOCAL_MEDIAN_REPEAT we repeat the first/last median
/// value for the truncated windows. So for example with a window size
/// of 7 the first 3 lines ((7-1)/2 = 3) are set to the same value as
/// the fourth line.
///
/// This is the sort of thing that we would normally do in python, but
/// it turns out there is no efficient code in python that I could
/// find to do this. So we have the function in C++ where it runs much
/// faster.
//-----------------------------------------------------------------------

blitz::Array<double, 2> GeoCal::array_local_median
(const blitz::Array<double, 2>& In,
 int Window_nrow, int Window_ncol,
 array_local_edge_handle Edge_handle)
{
  if(Window_nrow % 2 != 1 ||
     Window_ncol % 2 != 1)
    throw Exception("Window size needs to be odd");
  int hnr = (Window_nrow - 1) / 2;
  int hnc = (Window_ncol - 1) / 2;
  std::vector<double> scratch;
  scratch.reserve(Window_nrow * Window_ncol);
  blitz::Array<double, 2> res(In.shape());
  for(int i = 0; i < res.rows(); ++i)
    for(int j = 0; j < res.cols(); ++j) {
      scratch.clear();
      if(Edge_handle == ARRAY_LOCAL_MEDIAN_ZEROPAD) {
	for(int ii = i - hnr; ii <= i + hnr; ++ii)
	  for(int ji = j - hnc; ji <= j + hnc; ++ji)
	    if(ii >= 0 && ii < In.rows() && ji >= 0 && ji < In.cols())
	      scratch.push_back(In(ii,ji));
	    else
	      scratch.push_back(0);
      } else {
	int iimin = std::max(i - hnr,0);
	int iimax = std::min(i + hnr, In.rows()-1);
	int jjmin = std::max(j - hnc, 0);
	int jjmax = std::min(j + hnc, In.cols()-1);
	for(int ii = iimin; ii <= iimax; ++ii)
	  for(int ji = jjmin; ji <= jjmax; ++ji)
	    scratch.push_back(In(ii,ji));
      }
      res(i,j) = median_calc(scratch);
    }
  if(Edge_handle == ARRAY_LOCAL_MEDIAN_REPEAT) {
    for(int i = 0; i < res.rows(); ++i)
      for(int j = 0; j < res.cols();++j) {
	if(i - hnr < 0)
	  res(i,j) = res(hnr, j);
	if(i+hnr >= res.rows())
	  res(i,j)=res(res.rows()-hnr-1,j);
	if(j - hnc < 0)
	  res(i,j)=res(i,hnc);
	if(j+hnc >= res.cols())
	  res(i,j) = res(i,res.cols()-hnc-1);
      }
  }
  return res;
}

//-----------------------------------------------------------------------
/// This use a IgcRayCaster to determine the ground location of every
/// point in a Igc, and the convert it to the coordinate given by a
/// Cconv. We return an array that has 5 columns, the image line,
/// image sample, coordinate x, coordinate y and coordinate z.
///
/// This is little more than a loop, but we have this because this is
/// useful in python but slow to generate there.
//-----------------------------------------------------------------------

blitz::Array<double, 2> GeoCal::ray_cast_ground_coordinate
(const boost::shared_ptr<ImageGroundConnection>& Igc,
 const CoordinateConverter& Cconv)
{
  blitz::Array<double, 2> res(Igc->number_line() * Igc->number_sample(),
			      5);
  // We use a really large resolution, to force the number of
  // subpixels to to 1.
  IgcRayCaster rcast(Igc, 1, 0, -1, 1, 1e6);
  int resind = 0;
  while(!rcast.last_position()) {
    blitz::Array<double, 6> d = rcast.next_position();
    for(int s = 0; s < d.cols(); ++s) {
      Ecr pt(d(0,s,0,0,0,0), d(0,s,0,0,0,1), d(0,s,0,0,0,2));
      res(resind,0) = rcast.current_position();
      res(resind,1) = s;
      Cconv.convert_to_coordinate(pt, res(resind,2), res(resind,3),
				  res(resind,4));
      ++resind;
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// This determines bad pixels. This is the exact same algorithm as
/// found in
/// LinearGradientBadPixelDetection._bad_pixel_detection_python,
/// but rewritten in C++ for speed. See the python code to see what
/// this algorithm is doing.
//-----------------------------------------------------------------------

blitz::Array<bool, 2> GeoCal::linear_gradient_bad_pixel_detection
(const blitz::Array<double, 2>& Img,
 int Window_size,
 double Percentile,
 int Thresh_fact,
 double Nfail_thresh_percentage,
 array_local_edge_handle Edge_handle
)
{
  range_check_inclusive(Percentile, 0.0, 100.0);
  range_check_inclusive(Nfail_thresh_percentage, 0.0, 100.0);
  
  // Difference right, left, up, down. This trims the edges, we'll add
  // that back in shortly. Note that other than a sign up and down are the
  // same, as is left and right. We take an abs value in next step, so we only
  // need to calculate one of these.
  blitz::Array<double, 2> down_diff(Img.rows()-1,Img.cols());
  blitz::Array<double, 2> right_diff(Img.rows(),Img.cols()-1);
  for(int i = 0; i < down_diff.rows(); ++i)
    for(int j = 0; j < down_diff.cols(); ++j)
      down_diff(i,j) = Img(i,j) - Img(i+1,j);
  for(int i = 0; i < right_diff.rows(); ++i)
    for(int j = 0; j < right_diff.cols(); ++j)
      right_diff(i,j) = Img(i,j) - Img(i,j+1);
  
  // Local median
  blitz::Array<double, 2> down_diff_local_med(down_diff.shape());
  blitz::Array<double, 2> right_diff_local_med(right_diff.shape());
  down_diff_local_med = blitz::abs(down_diff -
			   array_local_median(down_diff, 1, Window_size, Edge_handle));
  right_diff_local_med = blitz::abs(right_diff -
		           array_local_median(right_diff, Window_size, 1, Edge_handle));

  // Calculate threshold. Note this is slightly different than what we
  // do in the python, we don't bother handling the small difference
  // in Percentile*size not exactly matching a integer value - this is
  // a pretty small effect and not worth the extra handling for this.
  int ndwn = (int) round((int) down_diff_local_med.size() * Percentile / 100.0);
  int nrgt = (int) round((int) right_diff_local_med.size() * Percentile / 100.0);
  range_check(ndwn, 0, (int) down_diff_local_med.size());
  range_check(nrgt, 0, (int) right_diff_local_med.size());
  // nth_element rearranges the data, so we need to make a copy here
  // for working with
  blitz::Array<double, 2> dwn(down_diff_local_med.copy());
  blitz::Array<double, 2> rgt(right_diff_local_med.copy());
  std::nth_element(dwn.data(), dwn.data() + ndwn, dwn.data() + dwn.size());
  std::nth_element(rgt.data(), rgt.data() + nrgt, rgt.data() + rgt.size());
  double down_thresh = dwn.data()[ndwn] * Thresh_fact;
  double right_thresh = rgt.data()[nrgt] * Thresh_fact;

  // Determine the number of failures in the 4 directions. We use
  // doubles where because we convert this to a percentage in a
  // following step
  blitz::Array<double, 2> nfail_down(down_diff_local_med.shape());
  blitz::Array<double, 2> nfail_right(right_diff_local_med.shape());
  nfail_down = blitz::where(down_diff_local_med > down_thresh, 1.0, 0.0);
  nfail_right = blitz::where(right_diff_local_med > right_thresh, 1.0, 0.0);
  blitz::Array<double, 2> nfail(Img.shape());
  nfail = 0;
  // A little faster to unroll this and calculate in a loop.
  //nfail(blitz::Range(1,Img.rows()-1), blitz::Range::all()) += nfail_down;
  //nfail(blitz::Range(0,Img.rows()-2), blitz::Range::all()) += nfail_down;
  //nfail(blitz::Range::all(), blitz::Range(1,Img.cols()-1)) += nfail_right;
  //nfail(blitz::Range::all(), blitz::Range(0,Img.cols()-2)) += nfail_right;

  for(int i = 0; i < nfail.rows(); ++i)
    for(int j = 0; j < nfail.rows(); ++j) {
      nfail(i,j) += nfail_down(i,j);
      if(i < nfail_down.rows())
	nfail(i+1,j) += nfail_down(i,j);
      nfail(i,j) += nfail_right(i,j);
      if(j < nfail_right.cols())
	nfail(i,j+1) += nfail_right(i,j);
    }
  
  // Convert to percentage
  blitz::Array<double, 2> npix(Img.shape());
  npix = 4;
  for(int i = 0; i < npix.cols(); ++i) {
    npix(0,i) = 3;
    npix(Img.rows()-1,i) = 3;
  }
  for(int i = 0; i < npix.rows(); ++i) {
    npix(i,0) = 3;
    npix(i,Img.cols()-1) = 3;
  }
  npix(0,0) = 2;
  npix(0,Img.cols()-1) = 2;
  npix(Img.rows()-1,0) = 2;
  npix(Img.rows()-1,Img.cols()-1) = 2;

  nfail *= 100.0 / npix;
  blitz::Array<bool, 2> res(Img.shape());
  res = (nfail >= Nfail_thresh_percentage);
  return res;
}
