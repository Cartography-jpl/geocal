#include "misc_array_function.h"
#include "geocal_exception.h"
#include <vector>
#include <algorithm>

using namespace GeoCal;
using namespace blitz;

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
  std::vector<double> scratch(Window_nrow * Window_ncol);
  blitz::Array<double, 2> res(In.shape());
  for(int i = 0; i < res.rows(); ++i)
    for(int j = 0; j < res.cols(); ++j) {
      int sind = 0;
      for(int ii = i - hnr; ii <= i + hnr; ++ii)
	for(int ji = j - hnc; ji <= j + hnc; ++ji)
	  if(ii >= 0 && ii < In.rows() && ji >= 0 && ji < In.cols()) {
	    scratch[sind] = In(ii,ji);
	    ++sind;
	  } else {
	    // Handle edges according to user preference
	    if(Edge_handle == ARRAY_LOCAL_MEDIAN_ZEROPAD) {
	      scratch[sind] = 0;
	      ++sind;
	    }
	  }
      if(sind % 2 == 1) {
	int n = (sind - 1)  / 2;
	std::nth_element(scratch.begin(), scratch.begin() + n,
			 scratch.begin() + sind);
	res(i, j) = scratch[n];
      } else {
	int n = sind / 2 - 1;
	std::nth_element(scratch.begin(), scratch.begin() + n,
			 scratch.begin() + sind);
	double t1 = scratch[n];
	std::nth_element(scratch.begin(), scratch.begin() + n + 1,
			 scratch.begin() + sind);
	double t2 = scratch[n+1];
	res(i, j) = (t1 + t2) / 2;
      }
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
