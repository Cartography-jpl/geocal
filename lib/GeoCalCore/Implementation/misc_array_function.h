#include <blitz/array.h>

namespace GeoCal {
  enum array_local_edge_handle {ARRAY_LOCAL_MEDIAN_TRUNCATE=0,
				ARRAY_LOCAL_MEDIAN_ZEROPAD=1,
				ARRAY_LOCAL_MEDIAN_REPEAT=2};
  blitz::Array<double, 2> array_local_median(const blitz::Array<double, 2>& In,
	     int Window_nrow, int Window_ncol,
             array_local_edge_handle Edge_handle=ARRAY_LOCAL_MEDIAN_TRUNCATE);
}
