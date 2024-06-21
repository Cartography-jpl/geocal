// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "misc_array_function.h"
%}

%import "image_ground_connection.i"
%import "coordinate_converter.i"

namespace GeoCal {
enum array_local_edge_handle {ARRAY_LOCAL_MEDIAN_TRUNCATE=0,
				ARRAY_LOCAL_MEDIAN_ZEROPAD=1,
				ARRAY_LOCAL_MEDIAN_REPEAT=2};
blitz::Array<double, 2> array_local_median(const blitz::Array<double, 2>& In,
	     int Window_nrow, int Window_ncol,
	     array_local_edge_handle Edge_handle=ARRAY_LOCAL_MEDIAN_TRUNCATE,
	     int Number_thread=1);
blitz::Array<bool, 2> linear_gradient_bad_pixel_detection
  (const blitz::Array<double, 2>& Img,
   int Window_size = 7,
   double Percentile = 90.0,
   int Thresh_fact = 2,
   double Nfail_thresh_percentage = 75.0,
   array_local_edge_handle Edge_handle=ARRAY_LOCAL_MEDIAN_TRUNCATE,
   int Number_thread=1);
blitz::Array<double, 2> ray_cast_ground_coordinate
(const boost::shared_ptr<ImageGroundConnection>& Igc,
 const CoordinateConverter& Cconv);
}

// List of things "import *" will include
%python_export("array_local_median", "ARRAY_LOCAL_MEDIAN_TRUNCATE",
	       "ARRAY_LOCAL_MEDIAN_ZEROPAD", "ARRAY_LOCAL_MEDIAN_REPEAT",
	       "linear_gradient_bad_pixel_detection",
	       "ray_cast_ground_coordinate")
