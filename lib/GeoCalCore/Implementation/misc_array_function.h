#include <blitz/array.h>

namespace GeoCal {
  blitz::Array<double, 2> array_local_median(const blitz::Array<double, 2>& In,
					     int Window_nrow, int Window_ncol);
}
