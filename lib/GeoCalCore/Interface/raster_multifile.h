#ifndef RASTER_MULTIFILE_H
#define RASTER_MULTIFILE_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This is a single file used by RasterMultifile
*******************************************************************/

  struct RasterMultifileTile {
//-----------------------------------------------------------------------
/// Default constructor
//-----------------------------------------------------------------------

    RasterMultifileTile() 
  : data((RasterImage*)0), line_offset(-1), sample_offset(-1) {}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

    RasterMultifileTile(const boost::shared_ptr<RasterImage>& d,
			      int loff, int soff)
      : data(d), line_offset(loff), sample_offset(soff) {}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

    boost::shared_ptr<RasterImage> data;

//-----------------------------------------------------------------------
/// Offset from the start of the whole image to the data in this file
//-----------------------------------------------------------------------

    int line_offset;

//-----------------------------------------------------------------------
/// Offset from the start of the whole image to the data in this file
//-----------------------------------------------------------------------

    int sample_offset;

//-----------------------------------------------------------------------
/// Return true if Line and Sample is in this file, false otherwise.
//-----------------------------------------------------------------------

    bool in_tile(int Line, int Sample) const
    {
      if(!data)
	return false;
      if(Line < line_offset ||
	 Sample < sample_offset)
	return false;
      if(Line - line_offset < data->number_line() &&
	 Sample - sample_offset < data->number_sample())
	return true;
      return false;
    }

//-----------------------------------------------------------------------
/// Read data.
//-----------------------------------------------------------------------

    int unchecked_read(int Line, int Sample) const
    { return data->unchecked_read(Line - line_offset, Sample - sample_offset); }

    double unchecked_read_double(int Line, int Sample) const
    { return data->unchecked_read_double(Line - line_offset, 
					 Sample - sample_offset); }

//-----------------------------------------------------------------------
/// Write data.
//-----------------------------------------------------------------------

    void unchecked_write(int Line, int Sample, int Val) const
    { data->unchecked_write(Line - line_offset, Sample - sample_offset, Val); }
    void unchecked_write(int Line, int Sample, double Val) const
    { data->unchecked_write(Line - line_offset, Sample - sample_offset, Val); }
  };

/****************************************************************//**
  There are certain larger data sets that are made up of a number of
  separate files all in the same map projection that cover a large
  area. For example, the SRTM data available through AFIDS. This class
  captures the common functionality needed to deal with those files.
  The data is stitched together by either reading one file or the
  other, there is no smoothing done by this class between 
  neighboring files.

  Note that if you are creating a derived class, LocationToFile may be
  a useful class to use.

  In general, the area covered by this RasterMultifile will not
  be fully covered by the separate files (e.g., there are tiles
  missing in the SRTM). Depending on the application, trying to read
  data where we don't have a file will either be an error, or
  acceptable and we want to return a fill value instead. To handle
  this, derived classes return a RasterMultifileTile with data =
  0. Then, depending on the value of No_coverage_is_error we either
  let the exception be thrown or we catch it and substitute a fill
  value for the attempted read.
*******************************************************************/

class RasterMultifile : public RasterImageVariable {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RasterMultifile() {}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  { RasterMultifileTile& mi = swap(Line, Sample);
    if(mi.data.get())
      return mi.unchecked_read(Line, Sample);
    if(no_coverage_is_error_) {
      NoCoverage e("Attempt to read data where we don't have a file in RasterMultifile.");
      e << " Location: " << *ground_coordinate(ImageCoordinate(Line,Sample));
      throw e;
    }
    return no_coverage_fill_value_;
  }

  virtual double unchecked_read_double(int Line, int Sample) const
  { RasterMultifileTile& mi = swap(Line, Sample);
    if(mi.data.get())
      return mi.unchecked_read_double(Line, Sample);
    if(no_coverage_is_error_) {
      NoCoverage e("Attempt to read data where we don't have a file in RasterMultifile.");
      e << " Location: " << *ground_coordinate(ImageCoordinate(Line,Sample));
      throw e;
    }
    return no_coverage_fill_value_;
  }

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void     unchecked_write(int Line, int Sample, int Val)
  {
    RasterMultifileTile& mi = swap(Line, Sample);
    if(mi.data.get())
      mi.unchecked_write(Line, Sample, Val);
    else {
      NoCoverage e("Attempt to write data where we don't have a file in RasterMultifile.");
      e << " Location: " << *ground_coordinate(ImageCoordinate(Line,Sample));
      throw e;
    }
  }
  virtual void     unchecked_write(int Line, int Sample, double Val)
  {
    RasterMultifileTile& mi = swap(Line, Sample);
    if(mi.data.get())
      mi.unchecked_write(Line, Sample, Val);
    else {
      NoCoverage e("Attempt to write data where we don't have a file in RasterMultifile.");
      e << " Location: " << *ground_coordinate(ImageCoordinate(Line,Sample));
      throw e;
    }
  }

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "RasterMultifile " << number_line() << " x " << number_sample() 
       << "\n"
       << "  Map info: " << map_info() << "\n";
  }

//-----------------------------------------------------------------------
/// If true, we throw an error if we try to read an area with no data.
//-----------------------------------------------------------------------

  bool no_coverage_is_error() const {return no_coverage_is_error_; }

//-----------------------------------------------------------------------
/// Fill value to return for areas with no coverage.
//-----------------------------------------------------------------------

  int no_coverage_fill_value() const {return no_coverage_fill_value_; }

//-----------------------------------------------------------------------
/// Number of tiles we have open at one time.
//-----------------------------------------------------------------------
  int number_tile() const {return (int) tile.size(); }

//-----------------------------------------------------------------------
/// Number of files we have read.
//-----------------------------------------------------------------------

  int number_file_read() const { return number_file_read_; }

//-----------------------------------------------------------------------
/// Set number of files we have read back to zero.
//-----------------------------------------------------------------------

  void reset_number_file_read() { number_file_read_ = 0; }

protected:
//-----------------------------------------------------------------------
/// Default constructor. Make sure to update map_info_ yourself if you
/// use this.
//-----------------------------------------------------------------------

  RasterMultifile(int Number_tile,
		  bool No_coverage_is_error = true, 
		  int No_coverage_fill_value = -1)
  {
    init(Number_tile, No_coverage_is_error, No_coverage_fill_value);
  }

  RasterMultifile()
    : number_file_read_(0)
  {
  }

  void init(int Number_tile = 4,
	    bool No_coverage_is_error = true, 
	    int No_coverage_fill_value = -1)
  {
    no_coverage_is_error_  = No_coverage_is_error;
    no_coverage_fill_value_ = No_coverage_fill_value;
    tile.resize(Number_tile);
    next_swap = tile.begin();
    number_file_read_ = 0;
  }

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  RasterMultifile(const MapInfo& M, int Number_tile = 4, 
			bool No_coverage_is_error = true, 
			int No_coverage_fill_value = -1)
    : RasterImageVariable(M), 
      no_coverage_is_error_(No_coverage_is_error),
      no_coverage_fill_value_(No_coverage_fill_value),
      number_file_read_(0),
      tile(Number_tile)
  {
    next_swap = tile.begin();
  }

//-----------------------------------------------------------------------
/// Return file for given line and sample.
//-----------------------------------------------------------------------

  virtual  RasterMultifileTile get_file(int Line, int Sample) const = 0;

//-----------------------------------------------------------------------
/// If true, we throw an error if we try to read an area with no data.
//-----------------------------------------------------------------------

  bool no_coverage_is_error_;

//-----------------------------------------------------------------------
/// Fill value to return for areas with no coverage.
//-----------------------------------------------------------------------

  int no_coverage_fill_value_;

//-----------------------------------------------------------------------
/// Number of files we have read.
//-----------------------------------------------------------------------

  mutable int number_file_read_;
private:
  mutable RasterMultifileTile mt_scratch;
  RasterMultifileTile& swap(int Line, int Sample) const;
  mutable std::vector<RasterMultifileTile> tile;
  mutable std::vector<RasterMultifileTile>::iterator next_swap;
				///< Next tile to be swapped.
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

}

GEOCAL_EXPORT_KEY(RasterMultifile);
#endif
