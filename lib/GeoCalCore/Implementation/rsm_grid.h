#ifndef RSM_GRID_H
#define RSM_GRID_H
#include "rsm_base.h"
#include <limits>

namespace GeoCal {
  class ImageGroundConnection;
  class CoordinateConverter;

/****************************************************************//**
  Use a interpolation grid to map from ground to image. Right now we
  only do a linear interpolation. The RSM documentation suggests doing
  Lagrange interpolation of various orders, we can implement that in the
  future.

  This can contain missing data, either because it is outside of the
  image or because the initial ImageGroundConnection failed for a
  particular point. We record these as NaNs.

  Right now this just carries through, we should probably add some
  kind of a status for image_coordinate.

  We plan on using the same grid for a full ground to image map, or
  for doing a correction.
*******************************************************************/

class RsmGrid : public boost::noncopyable, public RsmBase {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmGrid(int N_x, int N_y, int N_z,
	  bool Ignore_igc_error_in_fit = false,
	  int Total_number_row_digit = 11,
	  int Total_number_col_digit = 11,
	  int Number_fractional_row_digit = 2,
	  int Number_fractional_col_digit = 2,
	  int Row_section_number = 1,
	  int Col_section_number = 1,
	  const std::string& Image_identifier="",
	  const std::string& Rsm_support_data_edition="fake-1")
    : RsmBase(Image_identifier, Rsm_support_data_edition),
      have_z_range_(false),
      line_(N_x, N_y, N_z),
      sample_(N_x, N_y, N_z),
      ignore_igc_error_in_fit_(Ignore_igc_error_in_fit),
      total_number_row_digit_(Total_number_row_digit),
      total_number_col_digit_(Total_number_col_digit),
      number_fractional_row_digit_(Number_fractional_row_digit),
      number_fractional_col_digit_(Number_fractional_col_digit),
      row_section_number_(Row_section_number),
      col_section_number_(Col_section_number)
  {}
  virtual ~RsmGrid() {}
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>
      (new RsmGrid(line_.rows(), line_.cols(), line_.depth(),
		   ignore_igc_error_in_fit_, total_number_row_digit_,
		   total_number_col_digit_, number_fractional_row_digit_,
		   number_fractional_col_digit_));
  }
  virtual bool check_zero_crossing(double Grid_spacing = 0.01) const
  { 
    return false;
  }
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z,
					   bool Extrapolate_ok) const;
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z)
    const
  { return image_coordinate(X, Y, Z, false); }

  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const
  { return image_coordinate(X, Y, Z, false); }

  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const blitz::Array<double, 2>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const blitz::Array<double, 2>& Z) const
  { return image_coordinate(X, Y, Z, false); }

  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z) const
  { return image_coordinate(X, Y, Z, false); }

  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const
  { return image_coordinate_jacobian(X, Y, Z, false); }

  virtual void initial_guess(double Line, double Sample, double Z,
			     double& X_guess, double& Y_guess) const
  {
    int z_index = (int) floor((Z - z_start()) / z_delta() + 0.5);
    X_guess = x_start() + x_delta() * (x_offset(z_index) +
				       number_x(z_index) / 2.0);
    Y_guess = y_start() + y_delta() * (y_offset(z_index) +
				       number_y(z_index) / 2.0);
  }
  virtual double initial_guess_z(double Line, double Sample) const
  {
    return z_start() + z_delta() * number_z() / 2.0;
  }
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  virtual void fit_data(const blitz::Array<double, 2>& Data,
			int Min_line, int Max_line, int Min_sample,
			int Max_sample)
  { throw Exception("Not implemented"); }
  void fit_corr(const ImageGroundConnection& IGc,
		const CoordinateConverter& Cconv,
		const RsmBase& Rb);

  void extrapolate_x_direction();
  void extrapolate_y_direction();
  void extrapolate_z_direction();
  
//-----------------------------------------------------------------------
/// Number of X values in grid. This can potentially depend on the z
/// axis value.
//-----------------------------------------------------------------------
  
  int number_x(int Zindex) const { return line_.rows(); }

//-----------------------------------------------------------------------
/// Number of Y values in grid. This can potentially depend on the z
/// axis value.
//-----------------------------------------------------------------------
  
  int number_y(int Zindex) const { return line_.cols(); }

//-----------------------------------------------------------------------
/// Number of Z values in grid
//-----------------------------------------------------------------------
  
  int number_z() const { return line_.depth(); }


//-----------------------------------------------------------------------
/// Offset in X pixels of particular z_index grid relative to the
/// initial grid
//-----------------------------------------------------------------------

  int x_offset(int Zindex) const {return 0; }

//-----------------------------------------------------------------------
/// Offset in Y pixels of particular z_index grid relative to the
/// initial grid
//-----------------------------------------------------------------------

  int y_offset(int Zindex) const {return 0; }
  
//-----------------------------------------------------------------------
/// First X value in grid. This is for the first index, use x_offset
/// for other z index values.
//-----------------------------------------------------------------------

  double x_start() const {return x_start_;}

//-----------------------------------------------------------------------
/// First Y value in grid.
//-----------------------------------------------------------------------

  double y_start() const {return y_start_;}

//-----------------------------------------------------------------------
/// First Z value in grid.
//-----------------------------------------------------------------------

  double z_start() const {return z_start_;}

//-----------------------------------------------------------------------
/// Spacing of X in grid.
//-----------------------------------------------------------------------

  double x_delta() const {return x_delta_;}

//-----------------------------------------------------------------------
/// Spacing of Y in grid.
//-----------------------------------------------------------------------

  double y_delta() const {return y_delta_;}

//-----------------------------------------------------------------------
/// Spacing of Z in grid.
//-----------------------------------------------------------------------

  double z_delta() const {return z_delta_;}

//-----------------------------------------------------------------------
/// Grid of Line values
//-----------------------------------------------------------------------
  
  const blitz::Array<double, 3>& line_grid() const {return line_;}

//-----------------------------------------------------------------------
/// Grid of sample values
//-----------------------------------------------------------------------

  const blitz::Array<double, 3>& sample_grid() const {return sample_;};
  
  virtual int min_line() const {return min_line_;}
  virtual int max_line() const {return max_line_;}
  virtual int min_sample() const {return min_sample_;}
  virtual int max_sample() const {return max_sample_;}
  virtual double min_x() const {return x_start();}
  virtual double max_x() const {return x_start()+x_delta()*number_x(0);}
  virtual double min_y() const {return y_start();}
  virtual double max_y() const {return y_start()+y_delta()*number_y(0);}
  virtual double min_z() const {return z_start();}
  virtual double max_z() const {return z_start()+z_delta()*number_z();}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  bool ignore_igc_error_in_fit() const { return ignore_igc_error_in_fit_;}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  void ignore_igc_error_in_fit(bool V) { ignore_igc_error_in_fit_ = V;}

//-----------------------------------------------------------------------
/// Row section number.  
//-----------------------------------------------------------------------
  virtual int row_section_number() const {return row_section_number_; }
  virtual void row_section_number(int V) {row_section_number_ = V; }
  
//-----------------------------------------------------------------------
/// Column section number.
//-----------------------------------------------------------------------
  virtual int col_section_number() const {return col_section_number_; }
  virtual void col_section_number(int V) {col_section_number_ = V; }

  int total_number_row_digit() const {return total_number_row_digit_; }
  void total_number_row_digit(int v) {total_number_row_digit_ = v; }
  int total_number_col_digit() const {return total_number_col_digit_; }
  void total_number_col_digit(int v) {total_number_col_digit_ = v; }
  int number_fractional_row_digit() const
  { return number_fractional_row_digit_; }
  void number_fractional_row_digit(int v) { number_fractional_row_digit_ = v; }
  int number_fractional_col_digit() const
  { return number_fractional_col_digit_; }
  void number_fractional_col_digit(int v) { number_fractional_col_digit_ = v; }
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmGrid>
  read_tre_string(const std::string& Tre_in);

//-----------------------------------------------------------------------
/// Calculate the TRE size that we will try to write, so we can check
/// if it is too big. This is the size that will go into the CEL
/// field (so the total TRE size - 11).  
//-----------------------------------------------------------------------
  int tre_size() const
  { int res =  80 + 40 + 3 + 3 + 21 + 21 + 1 + 3 + 21 + 21 + 21 + 21
      + 21 + 21 + 9 + 9 + 2 + 2 + 1 + 1 +
      (number_z() - 1) * (4 + 4) +
      (number_z()) * ( 3 + 3 );
    for(int i = 0; i < number_z(); ++i)
      res += number_x(i) * number_y(i) *
	(total_number_row_digit_ + total_number_col_digit_);
    return res;
  }

//-----------------------------------------------------------------------
/// Based on examples, the MSP library seems to prefer that all
/// sections have the same z start and delta. This isn't actually
/// required from the RSM standard, but seems to be one of the those
/// "unstated" requirements. This gives access  for RsmMultiSection to
/// pass the z values to use, rather than computing this in fit.
//-----------------------------------------------------------------------

  virtual void set_z_range(double Z_min, double Z_max)
  {
    z_start_ = Z_min;
    z_delta_ = (Z_max - Z_min) / (number_z() - 1);
    have_z_range_ = true;
  }
protected:
  RsmGrid() : have_z_range_(false) {}
private:
  bool have_z_range_;
  // Note, by convention we *don't* include the x_offset, y_offset in
  // line_ and sample_. So we always have things relative to the
  // initial plane. The x_offset and y_offset is only used when we
  // store line_ and sample_, we instead use a larger array to fully
  // fit the data and fill the edges in with NaN. This speeds up the
  // code. We can revisit this if needed, this is really just an
  // internal convention we use and we could change this if desired.
  blitz::Array<double, 3> line_, sample_;
  int min_value_handle_nan(const blitz::Array<double, 3>& D) const
  {
    double r = blitz::min(blitz::where(blitz::blitz_isnan(D),
				       std::numeric_limits<double>::max(), D));
    if(r == std::numeric_limits<double>::max())
      r = 0; //Handling for all nan data
    return int(floor(r));
  } 
  double line_value_or_nan(int i, int j, int k) const
  { if(i < 0 || j < 0 || k < 0 || i >= line_.rows() || j >= line_.cols() ||
       k >= line_.depth())
      return std::numeric_limits<double>::quiet_NaN();
    return line_(i,j,k);
  }
  double sample_value_or_nan(int i, int j, int k) const
  { if(i < 0 || j < 0 || k < 0 || i >= sample_.rows() || j >= sample_.cols() ||
       k >= sample_.depth())
      return std::numeric_limits<double>::quiet_NaN();
    return sample_(i,j,k);
  }
  void extrapolate_line_helper(blitz::Array<double, 1>& d)
  {
    const int max_diff = 1000;
    for(int i = 2; i < d.rows(); ++i)
      if(std::isnan(d(i)) && !std::isnan(d(i-2)) && ! std::isnan(d(i-1))) {
	// For some cases we can get extreme behavior when
	// extrapolating. Since anything outside of the line/sample
	// range is invalid anyways, we switch to a single pixel
	// spacing when we get too far out of the range.
	if(d(i-1) < min_line_ - max_diff ||
	   d(i-1) > max_line_ + max_diff) {
	  if((d(i-1) - d(i - 2)) < 0)
	    d(i) = d(i-1) - 1;
	  else
	    d(i) = d(i-1) + 1;
	} else {
	  d(i) = d(i-1) + (d(i-1) - d(i - 2));
	}
      }
  }
  void extrapolate_sample_helper(blitz::Array<double, 1>& d)
  {
    const int max_diff = 1000;
    for(int i = 2; i < d.rows(); ++i)
      if(std::isnan(d(i)) && !std::isnan(d(i-2)) && ! std::isnan(d(i-1))) {
	// For some cases we can get extreme behavior when
	// extrapolating. Since anything outside of the line/sample
	// range is invalid anyways, we switch to a single pixel
	// spacing when we get too far out of the range.
	if(d(i-1) < min_sample_ - max_diff ||
	   d(i-1) > max_sample_ + max_diff) {
	  if((d(i-1) - d(i - 2)) < 0)
	    d(i) = d(i-1) - 1;
	  else
	    d(i) = d(i-1) + 1;
	} else {
	  d(i) = d(i-1) + (d(i-1) - d(i - 2));
	}
      }
  }
  double x_start_, y_start_, z_start_, x_delta_, y_delta_, z_delta_;
  int min_line_,max_line_,min_sample_,max_sample_;
  bool ignore_igc_error_in_fit_;
  int total_number_row_digit_, total_number_col_digit_,
    number_fractional_row_digit_, number_fractional_col_digit_;
  int row_section_number_, col_section_number_;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}

GEOCAL_EXPORT_KEY(RsmGrid);
GEOCAL_CLASS_VERSION(RsmGrid, 2);
#endif
