#ifndef RESAMPLER_H
#define RESAMPLER_H
#include "raster_image.h"
#include "dem.h"
#include <limits>
#include <vector>

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
  bool empty_resample(const blitz::Array<double, 2>& Data,
		      double Fill_value_threshold = std::numeric_limits<double>::lowest()) const;
  const MapInfo& map_info() const { return mi; }
  int number_sub_pixel() const {return nsub; }

//-------------------------------------------------------------------------
/// The output line and sample for each subpixel, as a
/// number line x number sample x 2 array. Values outside of
/// map_info() (e.g., -9999 for fill coordinates) aren't used. This is
/// mostly useful for testing.
//-------------------------------------------------------------------------

  const blitz::Array<int, 3>& data_index() const { return data_index_; }
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
		  blitz::Array<double, 2>& Lat, blitz::Array<double, 2>& Lon,
		  blitz::Array<double, 2>& Height) const;
  virtual void print(std::ostream& Os) const
  { Os << "Resampler";}

//-------------------------------------------------------------------------
/// When we use the default number of tasks, data smaller than this
/// number of subpixels is resampled in a single thread, since the
/// threading overhead isn't worth it.
//-------------------------------------------------------------------------

  inline static const int min_parallel_size = 1000000;
  static void determine_range(const blitz::Array<double, 2>& X_coor_interpolated,
			      const blitz::Array<double, 2>& Y_coor_interpolated,
			      const MapInfo& Mi, int Num_sub_pixel,
			      int& lstart, int& lend, int& sstart, int& send);
private:
  // The sum and count of the data landing in each output pixel, for
  // one tile of the data_index. To keep the memory small, we only cover
  // the bounding box of the output pixels the tile touches. For
  // Use_smallest_ic, we also track the smallest data_index position
  // (i * number col + j) that sees each output pixel.
  struct ResampleTile {
    int istart, iend, jstart, jend;		      // Range in data_index
    int lstart = 0, nline = 0, sstart = 0, nsamp = 0; // Bounding box in output
    std::vector<double> sum;
    std::vector<int> cnt;
    std::vector<long> first;
  };
  void check_data_size(int Number_line, int Number_sample) const;
  void init(const blitz::Array<double, 2>& X_coor,
	    const blitz::Array<double, 2>& Y_coor,
	    const MapInfo& Mi, bool Exactly_match_mi,
	    double Mark_missing);
  MapInfo mi;
  int nsub;
  blitz::Array<int, 3> data_index_;
  Resampler() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(Resampler);
#endif
