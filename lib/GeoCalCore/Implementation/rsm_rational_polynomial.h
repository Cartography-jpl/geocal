#ifndef RSM_RATIONAL_POLYNOMIAL_H
#define RSM_RATIONAL_POLYNOMIAL_H
#include "rsm_polynomial.h"
#include "rsm_base.h"
#include "image_coordinate.h"
#include <boost/optional.hpp>

namespace GeoCal {
  class Rpc;
  class ImageGroundConnection;
  class CoordinateConverter;
  
/****************************************************************//**
  This is a RsmRationalPolynomial.
*******************************************************************/

class RsmRationalPolynomial : public RsmBase {
public:
  RsmRationalPolynomial(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y,
			int Dp_z, int N_max_order = -1, int D_max_order = -1,
			int Nline_fit = 20, int Nsample_fit = 20,
			int Nheight_fit = 20, int Nsecond_pass_fit = 20,
			bool Ignore_igc_error_in_fit = false,
			int Row_section_number = 1,
			int Col_section_number = 1,
			const std::string& Image_identifier="",
			const std::string& Rsm_support_data_edition="fake-1");
  virtual ~RsmRationalPolynomial() {}
  virtual void print(std::ostream& Os) const;
  
//-----------------------------------------------------------------------
/// Check for zero crossing in the denominator, i.e., do we have poles
/// in our fit.
//-----------------------------------------------------------------------
  
  virtual bool check_zero_crossing(double Grid_spacing = 0.01) const
  { return line_den_.check_zero_crossing(Grid_spacing) ||
      sample_den_.check_zero_crossing(Grid_spacing); }
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>(new RsmRationalPolynomial(*this)); }
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X,
   const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X,
   const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  void set_rpc_coeff(const Rpc& R);
  void fit_offset_and_scale(double Min_line, double Max_line, double Min_sample,
			    double Max_sample, double Min_x, double Max_x,
			    double Min_y, double Max_y, double Min_z,
			    double Max_z);
  void fit_data(const std::vector<double>& Line,
		const std::vector<double>& Sample,
		const std::vector<double>& X,
		const std::vector<double>& Y,
		const std::vector<double>& Z);
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);

//-----------------------------------------------------------------------
/// Line fit error. This is a boost::optional, it may or may not be
/// available
//-----------------------------------------------------------------------

  const boost::optional<double>& line_fit_error() const
  {return line_fit_error_;}
  void line_fit_error(const boost::optional<double>& V)
  { line_fit_error_ = V; }

//-----------------------------------------------------------------------
/// Sample fit error. This is a boost::optional, it may or may not be
/// available
//-----------------------------------------------------------------------

  const boost::optional<double>& sample_fit_error() const
  {return sample_fit_error_;}
  void sample_fit_error(const boost::optional<double>& V)
  { sample_fit_error_ = V; }
  
  
//-----------------------------------------------------------------------
/// Initial guess to use when inverting the ground to image
/// function. Not sure if this belongs here or not, but for now we'll
/// try having this here.  Currently our initial guess is just the
/// middle of our X,Y range, but we can do something more intelligent
/// in the future if needed (e.g., a linear approximation to the
/// rational polynomial, or something like that.
//-----------------------------------------------------------------------
  
  virtual void initial_guess(double Line, double Sample, double Z,
		     double& X_guess, double& Y_guess) const
  { X_guess = x_offset_; Y_guess = y_offset_; }

//-----------------------------------------------------------------------
/// Initial guess at Z, if we need to find that also. This is just the
/// middle Z value.
//-----------------------------------------------------------------------

  virtual double initial_guess_z(double Line, double Sample) const
  { return z_offset_;}

//-----------------------------------------------------------------------
/// Number of lines in the grid we fit for.
//-----------------------------------------------------------------------

  int number_line_fit() const {return nline_fit_;}

//-----------------------------------------------------------------------
/// Number of lines in the grid we fit for.
//-----------------------------------------------------------------------

  void number_line_fit(int V) { nline_fit_ = V;}

//-----------------------------------------------------------------------
/// Number of samples in the grid we fit for.
//-----------------------------------------------------------------------

  int number_sample_fit() const {return nsample_fit_;}

//-----------------------------------------------------------------------
/// Number of samples in the grid we fit for.
//-----------------------------------------------------------------------

  void number_sample_fit(int V) { nsample_fit_ = V;}
  
//-----------------------------------------------------------------------
/// Number of heights in the grid we fit for.
//-----------------------------------------------------------------------

  int number_height_fit() const {return nheight_fit_;}

//-----------------------------------------------------------------------
/// Number of heights in the grid we fit for.
//-----------------------------------------------------------------------

  void number_height_fit(int V) { nheight_fit_ = V;}

//-----------------------------------------------------------------------
/// Number of rows/col/depth in x,y,z grid we fit for in the second
/// pass of generating fit data.
//-----------------------------------------------------------------------

  int number_second_pass_fit() const {return nsecond_pass_fit_;}

//-----------------------------------------------------------------------
/// Number of rows/col/depth in x,y,z grid we fit for in the second
/// pass of generating fit data.
//-----------------------------------------------------------------------

  void number_second_pass_fit(int V) { nsecond_pass_fit_ = V;}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  bool ignore_igc_error_in_fit() const { return ignore_igc_error_in_fit_;}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  void ignore_igc_error_in_fit(bool V) { ignore_igc_error_in_fit_ = V;}
  
  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  double line_offset() const {return line_offset_;}
  double line_scale() const {return line_scale_;}
  double sample_offset() const {return sample_offset_;}
  double sample_scale() const {return sample_scale_;}
  double x_offset() const {return x_offset_;}
  double x_scale() const {return x_scale_;}
  double y_offset() const {return y_offset_;}
  double y_scale() const {return y_scale_;}
  double z_offset() const {return z_offset_;}
  double z_scale() const {return z_scale_;}
  virtual int min_line() const {return int(line_offset_-line_scale_);}
  virtual int max_line() const {return int(line_offset_+line_scale_);}
  virtual int min_sample() const {return int(sample_offset_-sample_scale_);}
  virtual int max_sample() const {return int(sample_offset_+sample_scale_);}
  virtual double min_x() const {return x_offset_-x_scale_;}
  virtual double max_x() const {return x_offset_+x_scale_;}
  virtual double min_y() const {return y_offset_-y_scale_;}
  virtual double max_y() const {return y_offset_+y_scale_;} 
  virtual double min_z() const {return z_offset_-z_scale_;}
  virtual double max_z() const {return z_offset_+z_scale_;}
  const RsmPolynomial& line_numerator() const {return line_num_;}
  const RsmPolynomial& line_denominator() const {return line_den_;}
  const RsmPolynomial& sample_numerator() const {return sample_num_;}
  const RsmPolynomial& sample_denominator() const {return sample_den_;}
  std::string tre_string() const;
  static boost::shared_ptr<RsmRationalPolynomial>
  read_tre_string(const std::string& Tre_in);
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

protected:
  RsmRationalPolynomial() {}
private:
  int row_section_number_, col_section_number_;
  double line_offset_, line_scale_, sample_offset_, sample_scale_,
    x_offset_, x_scale_, y_offset_, y_scale_, z_offset_, z_scale_;
  int nline_fit_, nsample_fit_, nheight_fit_, nsecond_pass_fit_;
  bool ignore_igc_error_in_fit_;
  boost::optional<double> line_fit_error_, sample_fit_error_;
  RsmPolynomial line_num_, line_den_, sample_num_, sample_den_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmRationalPolynomial);
GEOCAL_CLASS_VERSION(RsmRationalPolynomial, 1);
#endif


