#ifndef RESAMPLER_H
#define RESAMPLER_H
#include "raster_image.h"
#include "dem.h"
#include <limits>

namespace GeoCal {
/****************************************************************//**
  This is used to take the L1B_GEO latitude and longitude fields
  (or other coordinate X and Y) and project data to a given MapInfo.

  This is a bit brute force, and we don't worry about memory usage. 
  The arrays are something like 10Kx10K floating point, so we are
  talking GB but not 10's of GB. But if this
  becomes an issue, we can revisit this and make this code more
  efficient - but for now this doesn't seem to be worth the effort.
*******************************************************************/

class Resampler : public Printable<Resampler> {
public:
  Resampler(const boost::shared_ptr<RasterImage>& X_coor,
	    const boost::shared_ptr<RasterImage>& Y_coor,
	    const MapInfo& Mi, int Num_sub_pixel = 2,
	    bool Exactly_match_mi = false, double Mark_missing=-1000.0);
  Resampler(const blitz::Array<double, 2>& X_coor_interpolated,
	    const blitz::Array<double, 2>& Y_coor_interpolated,
	    const MapInfo& Mi, int Num_sub_pixel = 2,
	    bool Exactly_match_mi = false, double Mark_missing=-1000.0);
  virtual ~Resampler() {}
  void clear();
  bool empty_resample() const;
  bool empty_resample(const boost::shared_ptr<RasterImage>& Data,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  const MapInfo& map_info() const { return mi; }
  int number_sub_pixel() const {return nsub; }
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
		  blitz::Array<double, 2>& Lat, blitz::Array<double, 2>& Lon,
		  blitz::Array<double, 2>& Height) const;
  virtual void print(std::ostream& Os) const
  { Os << "Resampler";}
  static void determine_range(const blitz::Array<double, 2>& X_coor_interpolated,
			      const blitz::Array<double, 2>& Y_coor_interpolated,
			      const MapInfo& Mi, int Num_sub_pixel,
			      int& lstart, int& lend, int& sstart, int& send);
private:
  void init(const blitz::Array<double, 2>& X_coor,
	    const blitz::Array<double, 2>& Y_coor,
	    const MapInfo& Mi, bool Exactly_match_mi,
	    double Mark_missing);
  MapInfo mi;
  int nsub;
  blitz::Array<int, 3> data_index;
  Resampler() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(Resampler);
#endif
