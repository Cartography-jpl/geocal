// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "resampler.h"
#include "image_ground_connection.h"
%}

%base_import(generic_object)
%import "raster_image.i"
%import "map_info.i"
%import "dem.i"

%geocal_shared_ptr(GeoCal::Resampler);
namespace GeoCal {
class Resampler : public GenericObject {
public:
  Resampler(const boost::shared_ptr<RasterImage>& X_coor,
	    const boost::shared_ptr<RasterImage>& Y_coor,
	    const MapInfo& Mi, int Num_sub_pixel = 2,
	    bool Exactly_match_mi = false,
	    double Mark_missing=-1000.0);
  Resampler(const blitz::Array<double, 2>& X_coor_interpolated,
	    const blitz::Array<double, 2>& Y_coor_interpolated,
	    const MapInfo& Mi, int Num_sub_pixel = 2,
	    bool Exactly_match_mi = false,
	    double Mark_missing=-1000.0);
  void clear();
  static void determine_range(const blitz::Array<double, 2>& X_coor_interpolated,
			      const blitz::Array<double, 2>& Y_coor_interpolated,
			      const MapInfo& Mi, int Num_sub_pixel,
			      int& OUTPUT, int& OUTPUT, int& OUTPUT, int& OUTPUT);
  bool empty_resample() const;
  bool empty_resample(const boost::shared_ptr<RasterImage>& Data,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  blitz::Array<double, 2> resample_field
  (const boost::shared_ptr<RasterImage>& Data,
   double Scale_data=1.0,
   bool Negative_to_zero=false, double Fill_value = 0.0,
   bool Use_smallest_ic = false,
   double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  void resample_field(const std::string& Fname,
		      const boost::shared_ptr<RasterImage>& Data,
		      double Scale_data=1.0,
		      const std::string& File_type="REAL",
		      bool Negative_to_zero=false, double Fill_value = 0.0,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  void map_values(const Dem& d,
		  blitz::Array<double, 2>& OUTPUT,
		  blitz::Array<double, 2>& OUTPUT,
		  blitz::Array<double, 2>& OUTPUT) const;
  %python_attribute(map_info, const MapInfo&);
  %python_attribute(number_sub_pixel, int);
  std::string print_to_string() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("Resampler")
