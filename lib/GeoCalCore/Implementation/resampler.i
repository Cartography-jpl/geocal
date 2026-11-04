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
	    double Fill_value_threshold = std::numeric_limits<double>::lowest(),
	    int Number_task = -1);
  Resampler(const blitz::Array<double, 2>& X_coor_interpolated,
	    const blitz::Array<double, 2>& Y_coor_interpolated,
	    const MapInfo& Mi, int Num_sub_pixel = 2,
	    bool Exactly_match_mi = false,
	    double Fill_value_threshold = std::numeric_limits<double>::lowest(),
	    int Number_task = -1);
  static boost::shared_ptr<Resampler> create_from_lat_lon
  (const blitz::Array<double, 2>& Latitude,
   const blitz::Array<double, 2>& Longitude,
   const MapInfo& Mi, int Num_sub_pixel, int Boundary = 1,
   double Fill_value_threshold = std::numeric_limits<double>::lowest(),
   int Number_task = -1);
  void clear();
  static void determine_range(const blitz::Array<double, 2>& X_coor_interpolated,
			      const blitz::Array<double, 2>& Y_coor_interpolated,
			      const MapInfo& Mi, int Num_sub_pixel,
			      int& OUTPUT, int& OUTPUT, int& OUTPUT, int& OUTPUT,
			      double Fill_value_threshold = std::numeric_limits<double>::lowest());
  static double max_step_ratio();
  static void set_max_step_ratio(double V);
  bool empty_resample() const;
  bool empty_resample(const boost::shared_ptr<RasterImage>& Data,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  bool empty_resample(const blitz::Array<double, 2>& Data,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  blitz::Array<double, 2> resample_field
  (const boost::shared_ptr<RasterImage>& Data,
   double Scale_data=1.0,
   bool Negative_to_zero=false, double Fill_value = 0.0,
   bool Use_smallest_ic = false,
   double Fill_value_threshold = std::numeric_limits<double>::lowest(),
   int Number_task = -1) const;
  blitz::Array<double, 2> resample_field
  (const blitz::Array<double, 2>& Data,
   double Scale_data=1.0,
   bool Negative_to_zero=false, double Fill_value = 0.0,
   bool Use_smallest_ic = false,
   double Fill_value_threshold = std::numeric_limits<double>::lowest(),
   int Number_task = -1) const;
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
  %python_attribute(swath_line_start, int);
  %python_attribute(swath_number_line, int);
  %python_attribute(swath_sample_start, int);
  %python_attribute(swath_number_sample, int);
  %python_attribute(has_swath_range, bool);
%pythoncode {
@property
def line_range(self) -> slice:
    '''Slice of the swath lines that the data passed to resample_field
    should be subset to. This is all the lines if we don't have a
    swath range.'''
    if not self.has_swath_range:
        return slice(0, None)
    return slice(self.swath_line_start,
                 self.swath_line_start + self.swath_number_line)

@property
def sample_range(self) -> slice:
    '''Slice of the swath samples that the data passed to resample_field
    should be subset to. This is all the samples if we don't have a
    swath range.'''
    if not self.has_swath_range:
        return slice(0, None)
    return slice(self.swath_sample_start,
                 self.swath_sample_start + self.swath_number_sample)
}
  std::string print_to_string() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("Resampler")
