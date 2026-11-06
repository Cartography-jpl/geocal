#ifndef RESAMPLER_H
#define RESAMPLER_H
#include "raster_image.h"
#include "dem.h"
#include <limits>
#include <vector>

namespace GeoCal {
/****************************************************************/ /**
  This is used to take the L1B_GEO latitude and longitude fields
  (or other coordinate X and Y) and project data to a given MapInfo.

  This is a bit brute force, and we don't worry about memory usage. 
  The arrays are something like 10Kx10K floating point, so we are
  talking GB but not 10's of GB. But if this
  becomes an issue, we can revisit this and make this code more
  efficient - but for now this doesn't seem to be worth the effort.

  A Resampler created with create_from_lat_lon only covers part of
  the swath, the lines and samples that can contribute to the
  MapInfo (has_swath_range() is true). The data passed to
  resample_field and empty_resample must then already be subset to
  swath_line_start() to swath_line_start() + swath_number_line() - 1,
  and swath_sample_start() to swath_sample_start() +
  swath_number_sample() - 1 of the full swath. We do this rather than
  passing the full swath so that only the part of each field we use
  needs to be read from the file (e.g., for a PACE OCI granule going
  to a small tile, this is often a small part of the swath). In
  python, line_range and sample_range give this as a slice:

      r = Resampler.create_from_lat_lon(lat, lon, mi, 15)
      d = geo["solar_zenith"].isel(scans=r.line_range,
                                   pixels=r.sample_range)
      res = r.resample_field(d.values)

  For the other constructors, the range is the full swath (start 0,
  number std::numeric_limits<int>::max()), so the line_range and
  sample_range in python select all the data, and you can always cut
  with them.

  Note that resample_field replicates the data, so the Nsub subpixels
  k = i * Nsub, ..., i * Nsub + Nsub - 1 all get raw pixel i. For the
  result to be in the right place, the coordinates of those subpixels
  should be centered on raw pixel i, i.e., subpixel k at raw position
  (k + 0.5) / Nsub - 0.5. The RasterImage constructor and
  create_from_lat_lon do this. With the array constructor, this is up
  to the caller. See that constructor for a warning about
  scipy.ndimage.zoom, which doesn't do this.
*******************************************************************/

class Resampler : public Printable<Resampler> {
public:
  Resampler(const boost::shared_ptr<RasterImage>& X_coor,
            const boost::shared_ptr<RasterImage>& Y_coor, const MapInfo& Mi,
            int Num_sub_pixel = 2, bool Exactly_match_mi = false,
            double Fill_value_threshold = std::numeric_limits<double>::lowest(),
            int Number_task = -1);
  Resampler(const blitz::Array<double, 2>& X_coor_interpolated,
            const blitz::Array<double, 2>& Y_coor_interpolated,
            const MapInfo& Mi, int Num_sub_pixel = 2,
            bool Exactly_match_mi = false,
            double Fill_value_threshold = std::numeric_limits<double>::lowest(),
            int Number_task = -1);
  static boost::shared_ptr<Resampler> create_from_lat_lon(
      const blitz::Array<double, 2>& Latitude,
      const blitz::Array<double, 2>& Longitude, const MapInfo& Mi,
      int Num_sub_pixel, int Boundary = 1,
      double Fill_value_threshold = std::numeric_limits<double>::lowest(),
      int Number_task = -1);
  virtual ~Resampler() {}
  void clear();
  bool empty_resample() const;
  bool empty_resample(const boost::shared_ptr<RasterImage>& Data,
                      double Fill_value_threshold =
                          std::numeric_limits<double>::lowest()) const;
  bool empty_resample(const blitz::Array<double, 2>& Data,
                      double Fill_value_threshold =
                          std::numeric_limits<double>::lowest()) const;
  const MapInfo& map_info() const { return mi; }
  int number_sub_pixel() const { return nsub; }

  //-------------------------------------------------------------------------
  /// The first line of the swath that the data passed to resample_field
  /// should start at. See the class description, this is 0 unless we
  /// were created with create_from_lat_lon.
  //-------------------------------------------------------------------------

  int swath_line_start() const { return swath_line_start_; }

  //-------------------------------------------------------------------------
  /// The number of lines of the swath that the data passed to
  /// resample_field should have. See the class description, this is
  /// std::numeric_limits<int>::max() (i.e., all the data) unless we were
  /// created with create_from_lat_lon.
  //-------------------------------------------------------------------------

  int swath_number_line() const { return swath_number_line_; }

  //-------------------------------------------------------------------------
  /// The first sample of the swath that the data passed to
  /// resample_field should start at. See swath_line_start.
  //-------------------------------------------------------------------------

  int swath_sample_start() const { return swath_sample_start_; }

  //-------------------------------------------------------------------------
  /// The number of samples of the swath that the data passed to
  /// resample_field should have. See swath_number_line.
  //-------------------------------------------------------------------------

  int swath_number_sample() const { return swath_number_sample_; }

  //-------------------------------------------------------------------------
  /// True if we only cover part of the swath, see the class description.
  //-------------------------------------------------------------------------

  bool has_swath_range() const
  {
    return swath_number_line_ != std::numeric_limits<int>::max();
  }

  //-------------------------------------------------------------------------
  /// The output line and sample for each subpixel, as a
  /// number line x number sample x 2 array. Values outside of
  /// map_info() (e.g., -9999 for fill coordinates) aren't used. This is
  /// mostly useful for testing.
  //-------------------------------------------------------------------------

  const blitz::Array<int, 3>& data_index() const { return data_index_; }
  blitz::Array<double, 2> resample_field(
      const boost::shared_ptr<RasterImage>& Data, double Scale_data = 1.0,
      bool Negative_to_zero = false, double Fill_value = 0.0,
      bool Use_smallest_ic = false,
      double Fill_value_threshold = std::numeric_limits<double>::lowest(),
      int Number_task = -1) const;
  blitz::Array<double, 2> resample_field(
      const blitz::Array<double, 2>& Data, double Scale_data = 1.0,
      bool Negative_to_zero = false, double Fill_value = 0.0,
      bool Use_smallest_ic = false,
      double Fill_value_threshold = std::numeric_limits<double>::lowest(),
      int Number_task = -1) const;
  void resample_field(const std::string& Fname,
                      const boost::shared_ptr<RasterImage>& Data,
                      double Scale_data = 1.0,
                      const std::string& File_type = "REAL",
                      bool Negative_to_zero = false, double Fill_value = 0.0,
                      double Fill_value_threshold =
                          std::numeric_limits<double>::lowest()) const;
  void map_values(const Dem& d, blitz::Array<double, 2>& Lat,
                  blitz::Array<double, 2>& Lon,
                  blitz::Array<double, 2>& Height) const;
  virtual void print(std::ostream& Os) const { Os << "Resampler"; }

  //-------------------------------------------------------------------------
  /// When we use the default number of tasks, data smaller than this
  /// number of subpixels is resampled in a single thread, since the
  /// threading overhead isn't worth it.
  //-------------------------------------------------------------------------

  inline static const int min_parallel_size = 1000000;
  static void determine_range(
      const blitz::Array<double, 2>& X_coor_interpolated,
      const blitz::Array<double, 2>& Y_coor_interpolated, const MapInfo& Mi,
      int Num_sub_pixel, int& lstart, int& lend, int& sstart, int& send,
      double Fill_value_threshold = std::numeric_limits<double>::lowest());

  //-------------------------------------------------------------------------
  /// For a projected MapInfo (e.g., UTM, Albers), we check that the
  /// coordinates don't jump across a discontinuity of the projection
  /// (e.g., the cut at 180 degrees from the central meridian, or a
  /// pole). We throw an exception if a step between neighboring
  /// points is more than max_step_ratio() times the median step. Set
  /// this to 0 to turn the check off.
  //-------------------------------------------------------------------------

  static double max_step_ratio() { return max_step_ratio_; }
  static void set_max_step_ratio(double V) { max_step_ratio_ = V; }
private:
  // The sum and count of the data landing in each output pixel, for
  // one tile of the data_index. To keep the memory small, we only cover
  // the bounding box of the output pixels the tile touches. For
  // Use_smallest_ic, we also track the smallest data_index position
  // (i * number col + j) that sees each output pixel.
  struct ResampleTile {
    int istart, iend, jstart, jend;                   // Range in data_index
    int lstart = 0, nline = 0, sstart = 0, nsamp = 0; // Bounding box in output
    std::vector<double> sum;
    std::vector<int> cnt;
    std::vector<long> first;
  };
  void check_data_size(int Number_line, int Number_sample) const;
  inline static double max_step_ratio_ = 50;
  static int default_number_task(long Size, int Number_task);
  static void range_in_mi(const blitz::Array<double, 2>& X_coor_interpolated,
                          const blitz::Array<double, 2>& Y_coor_interpolated,
                          const MapInfo& Mi, int Num_sub_pixel, int& lstart,
                          int& lend, int& sstart, int& send,
                          double Fill_value_threshold, bool Check);
  template<class F>
  void init(int Number_row, int Number_col, F Coor, const MapInfo& Mi,
            bool Exactly_match_mi, double Fill_value_threshold,
            int Number_task);
  MapInfo mi;
  int nsub;
  blitz::Array<int, 3> data_index_;
  int swath_line_start_ = 0;
  int swath_number_line_ = std::numeric_limits<int>::max();
  int swath_sample_start_ = 0;
  int swath_number_sample_ = std::numeric_limits<int>::max();
  Resampler() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(Resampler);
GEOCAL_CLASS_VERSION(Resampler, 1);
#endif
