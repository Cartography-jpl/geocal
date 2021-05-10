#include "rsm_grid.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
#include "auto_derivative.h"
#include "tre_support.h"
#include "local_rectangular_coordinate.h"
#include "simple_dem.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmGrid::load(Archive & ar, const unsigned int version)
{
  // Unfortunately boost serialization doesn't support NaN in XML
  // format (see
  // https://stackoverflow.com/questions/28696663/boost-serialization-1-5-5-crash-when-meets-nan-and-inf)
  // So we map line and sample using a "special" value to indicate
  // NaN.
  blitz::Array<double, 3> line_no_nan;
  blitz::Array<double, 3> sample_no_nan;
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBase)
    & GEOCAL_NVP2("line", line_no_nan)
    & GEOCAL_NVP2("sample", sample_no_nan)
    & GEOCAL_NVP_(x_start)
    & GEOCAL_NVP_(y_start)
    & GEOCAL_NVP_(z_start)
    & GEOCAL_NVP_(x_delta)
    & GEOCAL_NVP_(y_delta)
    & GEOCAL_NVP_(z_delta)
    & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP_(max_line)
    & GEOCAL_NVP_(min_sample)
    & GEOCAL_NVP_(max_sample)
    & GEOCAL_NVP_(ignore_igc_error_in_fit);
  line_.resize(line_no_nan.shape());
  sample_.resize(sample_no_nan.shape());
  line_ = where(line_no_nan < -9e19,
		std::numeric_limits<double>::quiet_NaN(),
		line_no_nan);
  sample_ = where(sample_no_nan < -9e19,
		  std::numeric_limits<double>::quiet_NaN(),
		  sample_no_nan);
  // Older version didn't have number of digits
  if(version >= 1) {
    ar & GEOCAL_NVP_(total_number_row_digit)
      & GEOCAL_NVP_(total_number_col_digit)
      & GEOCAL_NVP_(number_fractional_row_digit)
      & GEOCAL_NVP_(number_fractional_col_digit)
      & GEOCAL_NVP_(row_section_number)
      & GEOCAL_NVP_(col_section_number);
  } else {
    total_number_row_digit_ = 11;
    total_number_col_digit_ = 11;
    number_fractional_row_digit_ = 11;
    number_fractional_col_digit_ = 11;
    row_section_number_ = 1;
    col_section_number_ = 1;
  }  
  if(version >= 2) {
    ar & GEOCAL_NVP_(have_z_range);
  } else {
    have_z_range_ = false;
  }
}

template<class Archive>
void RsmGrid::save(Archive & ar, const unsigned int version) const
{
  // Unfortunately boost serialization doesn't support NaN in XML
  // format (see
  // https://stackoverflow.com/questions/28696663/boost-serialization-1-5-5-crash-when-meets-nan-and-inf)
  // So we map line and sample using a "special" value to indicate
  // NaN.
  blitz::Array<double, 3> line_no_nan(line_.shape());
  blitz::Array<double, 3> sample_no_nan(sample_.shape());
  line_no_nan = where(blitz_isnan(line_), -1e20, line_);
  sample_no_nan = where(blitz_isnan(sample_), -1e20, sample_);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBase)
    & GEOCAL_NVP2("line", line_no_nan)
    & GEOCAL_NVP2("sample", sample_no_nan)
    & GEOCAL_NVP_(x_start)
    & GEOCAL_NVP_(y_start)
    & GEOCAL_NVP_(z_start)
    & GEOCAL_NVP_(x_delta)
    & GEOCAL_NVP_(y_delta)
    & GEOCAL_NVP_(z_delta)
    & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP_(max_line)
    & GEOCAL_NVP_(min_sample)
    & GEOCAL_NVP_(max_sample)
    & GEOCAL_NVP_(ignore_igc_error_in_fit);
    
  // Older version didn't have number of digits
  if(version >= 1) {
    ar & GEOCAL_NVP_(total_number_row_digit)
      & GEOCAL_NVP_(total_number_col_digit)
      & GEOCAL_NVP_(number_fractional_row_digit)
      & GEOCAL_NVP_(number_fractional_col_digit)
      & GEOCAL_NVP_(row_section_number)
      & GEOCAL_NVP_(col_section_number);
  }
  if(version >= 2) {
    ar & GEOCAL_NVP_(have_z_range);
  }
}

GEOCAL_SPLIT_IMPLEMENT(RsmGrid);
#endif

// Have handling for NaN in interpolation
inline double interp_nan(double x1, double x2, double delta)
{
  if(std::isnan(x1))
    return x2;
  if(std::isnan(x2))
    return x1;
  return x1 + (x2 - x1) * delta;
}

inline AutoDerivative<double> interp_nan(double x1, double x2,
			 const AutoDerivative<double>& delta)
{
  if(std::isnan(x1))
    return x2;
  if(std::isnan(x2))
    return x1;
  return x1 + (x2 - x1) * delta;
}

inline AutoDerivative<double> interp_nan
(const AutoDerivative<double>& x1, const AutoDerivative<double>& x2,
 const AutoDerivative<double>& delta)
{
  if(std::isnan(x1.value()))
    return x2;
  if(std::isnan(x2.value()))
    return x1;
  return x1 + (x2 - x1) * delta;
}

inline double lagrange_cubic_interp_nan(double x1, double x2,
					double x3, double delta)
{
  // For nans, fall back onto the linear interpolation)
  if(std::isnan(x1))
    return interp_nan(x2, x3, delta);
  if(std::isnan(x3))
    return interp_nan(x1, x2, 1+delta);
  if(std::isnan(x2))
    return std::numeric_limits<double>::quiet_NaN();
  return x1 * (-0.5 * delta + 0.5 * delta * delta) + x2 * (1 - delta * delta) +
    x3 * (0.5 * delta + 0.5 * delta * delta);
}

inline AutoDerivative<double> lagrange_cubic_interp_nan
(AutoDerivative<double>& x1, AutoDerivative<double>& x2,
 AutoDerivative<double>& x3, const AutoDerivative<double>& delta)
{
  if(std::isnan(x1.value()))
    return interp_nan(x2, x3, delta);
  if(std::isnan(x3.value()))
    return interp_nan(x1, x2, 1+delta);
  if(std::isnan(x2.value()))
    return std::numeric_limits<double>::quiet_NaN();
  return x1 * (-0.5 * delta + 0.5 * delta * delta) + x2 * (1 - delta * delta) +
    x3 * (0.5 * delta + 0.5 * delta * delta);
}

inline AutoDerivative<double> lagrange_cubic_interp_nan
(double x1, double x2, double x3, const AutoDerivative<double>& delta)
{
  if(std::isnan(x1))
    return interp_nan(x2, x3, delta);
  if(std::isnan(x3))
    return interp_nan(x1, x2, 1+delta);
  if(std::isnan(x2))
    return std::numeric_limits<double>::quiet_NaN();
  return x1 * (-0.5 * delta + 0.5 * delta * delta) + x2 * (1 - delta * delta) +
    x3 * (0.5 * delta + 0.5 * delta * delta);
}

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value.
//-----------------------------------------------------------------------

ImageCoordinate RsmGrid::image_coordinate
(double X, double Y, double Z, bool Extrapolate_ok) const
{
  double iv = (X - x_start_) / x_delta_;
  double jv = (Y - y_start_) / y_delta_;
  double kv = (Z - z_start_) / z_delta_;
  int i = (int) floor(iv);
  int j = (int) floor(jv);
  int k = (int) floor(kv);
  double i_delta = iv - i;
  double j_delta = jv - j;
  double k_delta = kv - k;
  if(Extrapolate_ok) {
    if(i < 0) {
      i = 0;
      i_delta = 0;
    }
    if(j < 0) {
      j = 0;
      j_delta = 0;
    }
    if(k < 0) {
      k = 0;
      k_delta = 0;
    }
    if(i > line_.rows() - 2) {
      i = line_.rows() - 2;
      i_delta = 1;
    }
    if(j > line_.cols() - 2) {
      j = line_.cols() - 2;
      j_delta = 1;
    }
    if(k > line_.depth() - 2) {
      k = line_.depth() - 2;
      k_delta = 1;
    }
  }
  if(i < 0 || i >= line_.rows() ||
     j < 0 || j >= line_.cols() ||
     k < 0 || k >= line_.depth())
    return ImageCoordinate(std::numeric_limits<double>::quiet_NaN(),
			   std::numeric_limits<double>::quiet_NaN());
  // Lagrange cubit interpolation. 
  double xinter_line[3][3],
    xinter_sample[3][3];
  for(int i1 = 0; i1 < 3; ++i1)
    for(int i2 = 0; i2 < 3; ++i2) {
      xinter_line[i1][i2] =
	lagrange_cubic_interp_nan(line_value_or_nan(i-1,j-1+i1,k-1+i2),
				  line_value_or_nan(i,j-1+i1,k-1+i2),
				  line_value_or_nan(i+1,j-1+i1,k-1+i2),
				  i_delta);
      xinter_sample[i1][i2] =
	lagrange_cubic_interp_nan(sample_value_or_nan(i-1,j-1+i1,k-1+i2),
				  sample_value_or_nan(i,j-1+i1,k-1+i2),
				  sample_value_or_nan(i+1,j-1+i1,k-1+i2),
				  i_delta);
    }
  double yinter_line[3], yinter_sample[3];
  for(int i1 = 0; i1 < 3; ++i1) {
    yinter_line[i1] =
      lagrange_cubic_interp_nan(xinter_line[0][i1],
				xinter_line[1][i1],
				xinter_line[2][i1],
				j_delta);
    yinter_sample[i1] = 
      lagrange_cubic_interp_nan(xinter_sample[0][i1],
				xinter_sample[1][i1],
				xinter_sample[2][i1],
				j_delta);
  }
  double ln = lagrange_cubic_interp_nan(yinter_line[0], yinter_line[1],
					yinter_line[2], k_delta);
  double smp = lagrange_cubic_interp_nan(yinter_sample[0], yinter_sample[1],
					yinter_sample[2], k_delta);

  // Bunch of diagnostic messages. Leave stubbed out for now, we may
  // want this when we go to the lagrange interpolation to debug
  // problems.
  if(false) {
    std::cerr << "Row section number: " << row_section_number() << "\n"
	      << "Col section number: " << col_section_number() << "\n";
    std::cerr << "X: " << X << "\n"
	      << "Y: " << Y << "\n"
	      << "Z: " << Z << "\n";
    std::cerr << "i: " << i << "\n"
	      << "j: " << j << "\n"
	      << "k: " << k << "\n";
    std::cerr << "line shape:   " << line_.shape() << "\n"
	      << "sample shape: " << sample_.shape() << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	for(int i3 = 0; i3 < 2; ++i3)
	  std::cerr << "line(" << i1 << ", " << i2 << ", " << i3 << "): "
		    << line_(i+i1,j+i2,k+i3) << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	for(int i3 = 0; i3 < 2; ++i3)
	  std::cerr << "sample(" << i1 << ", " << i2 << ", " << i3 << "): "
		    << sample_(i+i1,j+i2,k+i3) << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	std::cerr << "xinter_line(" << i1 << ", " << i2 << "): "
		  << xinter_line[i1][i2] << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	std::cerr << "xinter_sample(" << i1 << ", " << i2 << "): "
		  << xinter_sample[i1][i2] << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      std::cerr << "yinter_line(" << i1 << "): "
		<< yinter_line[i1] << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      std::cerr << "yinter_sample(" << i1 << "): "
		<< yinter_sample[i1] << "\n";
    std::cerr << "ln: " << ln << "\n"
	      << "smp: " << smp << "\n";
  }

  return ImageCoordinate(ln,smp);
}

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmGrid::image_coordinate
(const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
 const blitz::Array<double, 1>& Z, bool Extrapolate_ok) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 2> res(2, X.rows());
  for(int i = 0; i < X.rows(); ++i) {
    ImageCoordinate ic = image_coordinate(X(i), Y(i), Z(i), Extrapolate_ok);
    res(0, i) = ic.line;
    res(1, i) = ic.sample;
  }
  return res;
}

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 3> RsmGrid::image_coordinate
(const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
 const blitz::Array<double, 2>& Z, bool Extrapolate_ok) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 3> res(2, X.rows(), X.cols());
  for(int i = 0; i < X.rows(); ++i)
    for(int j = 0; j < X.cols(); ++j) {
      ImageCoordinate ic = image_coordinate(X(i,j), Y(i,j), Z(i,j),
					    Extrapolate_ok);
      res(0, i, j) = ic.line;
      res(1, i, j) = ic.sample;
    }
  return res;
}

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 4> RsmGrid::image_coordinate
(const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
 const blitz::Array<double, 3>& Z, bool Extrapolate_ok) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols() ||
     X.depth() != Y.depth() || X.depth() != Z.depth())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 4> res(2, X.rows(), X.cols(), X.depth());
  for(int i = 0; i < X.rows(); ++i)
    for(int j = 0; j < X.cols(); ++j)
      for(int k = 0; k < X.depth(); ++k) {
	ImageCoordinate ic = image_coordinate(X(i,j,k), Y(i,j,k), Z(i,j,k),
					      Extrapolate_ok);
	res(0, i, j, k) = ic.line;
	res(1, i, j, k) = ic.sample;
      }
  return res;
}

//-----------------------------------------------------------------------
/// Generate a RsmGrid that approximates the calculation
/// done by a ImageGroundConnection.  We determine that X, Y, and Z
/// range to use automatically to cover the range given by the
/// ImageGroundConnection.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmGrid::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample)
{
  bool first = true;
  min_line_ = Min_line;
  max_line_ = Max_line;
  min_sample_ = Min_sample;
  max_sample_ = Max_sample;
  double min_x=0, max_x=0, min_y=0, max_y=0, min_z=0, max_z=0;
  for(double i = 0; i < 2; ++i)
    for(double j = 0; j < 2; ++j)
      for(double k = 0; k < 2; ++k) {
	double ln = (i == 0 ? Min_line : Max_line);
	double smp = (j == 0 ? Min_sample : Max_sample);
	boost::shared_ptr<Dem> dem;
	if(have_z_range_) {
	  double z = (k == 0 ? z_start_ : z_start_ + number_z() * z_delta_);
	  auto lc_cconv = boost::make_shared<LocalRcConverter>
	    (dynamic_cast<const LocalRcConverter&>(Cconv).parameter());
	  dem = boost::make_shared<LocalZDem>(lc_cconv, z);
	} else {
	  double h = (k == 0 ? Min_height : Max_height);
	  dem = boost::make_shared<SimpleDem>(h);
	}
	try {
	  boost::shared_ptr<GroundCoordinate> gc =
	    Igc.ground_coordinate_dem(ImageCoordinate(ln, smp), *dem);
	  double xv, yv, zv;
	  Cconv.convert_to_coordinate(*gc, xv, yv, zv);
	  if(first) {
	    min_x = max_x = xv;
	    min_y = max_y = yv;
	    min_z = max_z = zv;
	    first = false;
	  } else {
	    min_x = std::min(min_x, xv);
	    max_x = std::max(max_x, xv);
	    min_y = std::min(min_y, yv);
	    max_y = std::max(max_y, yv);
	    min_z = std::min(min_z, zv);
	    max_z = std::max(max_z, zv);
	  }
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	} catch(...) {
	  if(!ignore_igc_error_in_fit_)
	    throw;
	}
      }
  // The interpolation works better if we go a little past the edges
  // in x and y.
  const int pad = 2;
  x_delta_ = (max_x - min_x) / (number_x(0) - 1);
  y_delta_ = (max_y - min_y) / (number_y(0) - 1);
  z_delta_ = (max_z - min_z) / (number_z() - 1);
  min_x -= pad * x_delta_;
  max_x += pad * x_delta_;
  min_y -= pad * y_delta_;
  max_y += pad * y_delta_;
  
  x_start_ = min_x;
  y_start_ = min_y;
  x_delta_ = (max_x - min_x) / (number_x(0) - 1);
  y_delta_ = (max_y - min_y) / (number_y(0) - 1);
  if(!have_z_range_) {
    z_start_ = min_z;
    z_delta_ = (max_z - min_z) / (number_z() - 1);
  }
  for(int i = 0; i < line_.rows(); ++i)
    for(int j = 0; j < line_.cols(); ++j)
      for(int k = 0; k < line_.depth(); ++k) {
	double x = x_start_ + x_delta_ * i;
	double y = y_start_ + y_delta_ * j;
	double z = z_start_ + z_delta_ * k;
	boost::shared_ptr<GroundCoordinate> gc =
	  Cconv.convert_from_coordinate(x, y, z);
	ImageCoordinate ic;
	bool success;
	try {
	  Igc.image_coordinate_extended(*gc, ic, success);
	  if(success) {
	    line_(i,j,k) = ic.line;
	    sample_(i,j,k) = ic.sample;
	  } else {
	    line_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	    sample_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	  }
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	  line_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	  sample_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	} catch(...) {
	  if(!ignore_igc_error_in_fit_)
	    throw;
	  line_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	  sample_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	}
      }
}

//-----------------------------------------------------------------------
/// Generate a RsmGrid that gives a correction to another RsmBase.
//-----------------------------------------------------------------------

void RsmGrid::fit_corr
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 const RsmBase& Rb)
{
  min_line_ = Rb.min_line();
  max_line_ = Rb.max_line();
  min_sample_ = Rb.min_sample();
  max_sample_ = Rb.max_sample();
  x_start_ = Rb.min_x();
  y_start_ = Rb.min_y();
  z_start_ = Rb.min_z();
  x_delta_ = (Rb.max_x() - Rb.min_x()) / (number_x(0) - 1);
  y_delta_ = (Rb.max_y() - Rb.min_y()) / (number_y(0) - 1);
  z_delta_ = (Rb.max_z() - Rb.min_z()) / (number_z() - 1);
  for(int i = 0; i < line_.rows(); ++i)
    for(int j = 0; j < line_.cols(); ++j)
      for(int k = 0; k < line_.depth(); ++k) {
	double x = x_start_ + x_delta_ * i;
	double y = y_start_ + y_delta_ * j;
	double z = z_start_ + z_delta_ * k;
	boost::shared_ptr<GroundCoordinate> gc =
	  Cconv.convert_from_coordinate(x, y, z);
	ImageCoordinate ic;
	bool success;
	try {
	  Igc.image_coordinate_extended(*gc, ic, success);
	  ImageCoordinate ic_calc;
	  ic_calc = Rb.image_coordinate(x,y,z);
	  if(success) {
	    line_(i,j,k) = ic.line - ic_calc.line;
	    sample_(i,j,k) = ic.sample - ic_calc.sample;
	  } else {
	    line_(i,j,k) = 0;
	    sample_(i,j,k) = 0;
	  }
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	  line_(i,j,k) = 0;
	  sample_(i,j,k) = 0;
	} catch(...) {
	  if(!ignore_igc_error_in_fit_)
	    throw;
	  line_(i,j,k) = 0;
	  sample_(i,j,k) = 0;
	}
      }
}

//-----------------------------------------------------------------------
/// This the jacobian of the line, sample with respect to X, Y, Z.
/// This is a 2x3 matrix.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmGrid::image_coordinate_jacobian
(double X, double Y, double Z, bool Extrapolate_ok) const
{
  Range ra = Range::all();
  Array<double, 2> res(2, 3);
  AutoDerivative<double> xd(X, 0, 3);
  AutoDerivative<double> yd(Y, 1, 3);
  AutoDerivative<double> zd(Z, 2, 3);
  AutoDerivative<double> iv = (xd - x_start_) / x_delta_;
  AutoDerivative<double> jv = (yd - y_start_) / y_delta_;
  AutoDerivative<double> kv = (zd - z_start_) / z_delta_;
  int i = (int) floor(iv.value());
  int j = (int) floor(jv.value());
  int k = (int) floor(kv.value());
  AutoDerivative<double> i_delta = iv - i;
  AutoDerivative<double> j_delta = jv - j;
  AutoDerivative<double> k_delta = kv - k;
  if(Extrapolate_ok) {
    if(i < 0) {
      i = 0;
      i_delta = 0;
    }
    if(j < 0) {
      j = 0;
      j_delta = 0;
    }
    if(k < 0) {
      k = 0;
      k_delta = 0;
    }
    if(i > line_.rows() - 2) {
      i = line_.rows() - 2;
      i_delta = 1;
    }
    if(j > line_.cols() - 2) {
      j = line_.cols() - 2;
      j_delta = 1;
    }
    if(k > line_.depth() - 2) {
      k = line_.depth() - 2;
      k_delta = 1;
    }
  }
  if(i < 0 || i >= line_.rows() ||
     j < 0 || j >= line_.cols() ||
     k < 0 || k >= line_.depth()) {
    res = 0;
    return res;
  }
  // Lagrange cubit interpolation. 
  AutoDerivative<double> xinter_line[3][3],
    xinter_sample[3][3];
  for(int i1 = 0; i1 < 3; ++i1)
    for(int i2 = 0; i2 < 3; ++i2) {
      xinter_line[i1][i2] =
	lagrange_cubic_interp_nan(line_value_or_nan(i-1,j-1+i1,k-1+i2),
				  line_value_or_nan(i,j-1+i1,k-1+i2),
				  line_value_or_nan(i+1,j-1+i1,k-1+i2),
				  i_delta);
      xinter_sample[i1][i2] =
	lagrange_cubic_interp_nan(sample_value_or_nan(i-1,j-1+i1,k-1+i2),
				  sample_value_or_nan(i,j-1+i1,k-1+i2),
				  sample_value_or_nan(i+1,j-1+i1,k-1+i2),
				  i_delta);
    }
  AutoDerivative<double> yinter_line[3], yinter_sample[3];
  for(int i1 = 0; i1 < 3; ++i1) {
    yinter_line[i1] =
      lagrange_cubic_interp_nan(xinter_line[0][i1],
				xinter_line[1][i1],
				xinter_line[2][i1],
				j_delta);
    yinter_sample[i1] = 
      lagrange_cubic_interp_nan(xinter_sample[0][i1],
				xinter_sample[1][i1],
				xinter_sample[2][i1],
				j_delta);
  }
  AutoDerivative<double> ln = lagrange_cubic_interp_nan(yinter_line[0],
							yinter_line[1],
							yinter_line[2],
							k_delta);
  AutoDerivative<double> smp = lagrange_cubic_interp_nan(yinter_sample[0],
							 yinter_sample[1],
							 yinter_sample[2],
							 k_delta);
  if(ln.is_constant())
    res(0, ra) = 0;
  else
    res(0,ra) = ln.gradient();
  
  if(smp.is_constant())
    res(1,ra) = 0;
  else
    res(1,ra) = smp.gradient();
  return res;
}

//-----------------------------------------------------------------------
/// Extrapolate data in the x/sample direction
//-----------------------------------------------------------------------

void RsmGrid::extrapolate_x_direction()
{
  blitz::Range ra = blitz::Range::all();
  for(int k = 0; k < line_.depth(); ++k)
    for(int j = 0; j < line_.cols(); ++j) {
      blitz::Array<double, 1> line_col(line_(ra, j, k));
      blitz::Array<double, 1> sample_col(sample_(ra, j, k));
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
      line_col.reverseSelf(0);
      sample_col.reverseSelf(0);
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
    }
}

//-----------------------------------------------------------------------
/// Extrapolate data in the y/line direction
//-----------------------------------------------------------------------

void RsmGrid::extrapolate_y_direction()
{
  blitz::Range ra = blitz::Range::all();
  for(int k = 0; k < line_.depth(); ++k)
    for(int i = 0; i < line_.rows(); ++i) {
      blitz::Array<double, 1> line_col(line_(i, ra, k));
      blitz::Array<double, 1> sample_col(sample_(i, ra, k));
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
      line_col.reverseSelf(0);
      sample_col.reverseSelf(0);
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
    }
}

//-----------------------------------------------------------------------
/// Extrapolate data in the z/height direction
//-----------------------------------------------------------------------

void RsmGrid::extrapolate_z_direction()
{
  blitz::Range ra = blitz::Range::all();
  for(int j = 0; j < line_.cols(); ++j)
    for(int i = 0; i < line_.rows(); ++i) {
      blitz::Array<double, 1> line_col(line_(i, j, ra));
      blitz::Array<double, 1> sample_col(sample_(i, j, ra));
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
      line_col.reverseSelf(0);
      sample_col.reverseSelf(0);
      extrapolate_helper(line_col);
      extrapolate_helper(sample_col);
    }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmGrid::print(std::ostream& Os) const
{
  Os << "RsmGrid:\n"
     << "  Number x: " << number_x(0) << "\n"
     << "  Number y: " << number_y(0) << "\n"
     << "  Number z: " << number_z() << "\n"
     << "  Start:    (" << x_start() << ", " << y_start() << ", " << z_start()
     << ")\n"
     << "  Delta:    (" << x_delta() << ", " << y_delta() << ", " << z_delta()
     << ")\n";
}

static boost::format secformat("%|1$03d|%|2$03d|%|3$21s|%|4$21s|%|5$1s|");
static boost::format gplaneformat("%|1$03d|%|2$+21.14E|%|3$+21.14E|%|4$+21.14E|%|5$+21.14E|%|6$+21.14E|%|7$+21.14E|%|8$+09d|%|9$+09d|%|10$02d|%|11$02d|%|12$1d|%|13$1d|");
static boost::format igoffformat("%|1$+04d|%|2$+04d|");
static boost::format szformat("%|1$03d|%|2$03d|");

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note also that the TRE has a fixed precision which is less than
/// the machine precision. Writing a RsmGrid and then
/// reading it from a TRE does *not* in general give the exact same
/// RsmGrid, rather just one that is close.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

std::string RsmGrid::tre_string() const
{
  // Before starting, check that all the data will fit
  if(tre_size() > 99999) {
    Exception e;
    e << "Maximimum size of TRE is 99999\n"
      << "  TRE size: " << tre_size() << "\n";
    throw e;
  }
  std::string res = base_tre_string();
  // Don't fill in the row and column fit error
  std::string row_fit_error="";
  std::string col_fit_error="";
  // Linear interpolation
  //std::string int_ord = "1";
  std::string int_ord = "2";
  res += str_check_size(secformat % row_section_number_ % col_section_number_
			% row_fit_error % col_fit_error % int_ord,
			3 + 3 + 21 + 21 + 1);
  // This is the value that the line/sample are recorded relative
  // to. The TRE *only* has positive values, so this number needs to
  // be < the minimum value in the array so we don't need negative
  // numbers.
  int refrow = min_value_handle_nan(line_);
  int refcol = min_value_handle_nan(sample_);
  res += str_check_size(gplaneformat % number_z() % z_delta_ % x_delta_
			% y_delta_ % z_start_ % x_start_ % y_start_
			% refrow % refcol % total_number_row_digit_
			% total_number_col_digit_
			% number_fractional_row_digit_
			% number_fractional_col_digit_,
			3 + 6 * 21 + 2 * 9 + 2 * 2 + 2 * 1);
  for(int i = 1; i < number_z(); ++i) {
    // Offset held to 0 for now
    int xoff = 0;
    int yoff = 0;
    res += str_check_size(igoffformat % xoff % yoff, 2 * 4);
  }
  // The actual grid value size depends on the total_number_rol_digit_
  // and total_number_col_digit_, so we need to create it here rather
  // than having a static variable defined.
  boost::format lnform("%|1$0" +
       boost::lexical_cast<std::string>(total_number_row_digit_) + "d|");
  boost::format lnnanform("%|1$" +
       boost::lexical_cast<std::string>(total_number_row_digit_) + "s|");
  boost::format smpform("%|1$0" +
       boost::lexical_cast<std::string>(total_number_col_digit_) + "d|");
  boost::format smpnanform("%|1$" +
       boost::lexical_cast<std::string>(total_number_col_digit_) + "s|");
  double lnscale = pow(10, number_fractional_row_digit_);
  double smpscale = pow(10, number_fractional_row_digit_);
  for(int i = 0; i < number_z(); ++i) {
    // Give a better error message for too large size
    if(number_x(i) > 999 || number_y(i) > 999) {
      Exception e;
      e << "The NITF TRE supports a maximum number_x and number_y of 999\n"
	<< "  number_x(" << i << "): " << number_x(i) << "\n"
	<< "  number_y(" << i << "): " << number_y(i) << "\n";
      throw e;
    }
    res += str_check_size(szformat % number_x(i) % number_y(i), 2 * 3);
    for(int i1 = 0; i1 < number_x(i); ++i1)
      for(int i2 = 0; i2 < number_y(i); ++i2) {
	if(std::isnan(line_(i1,i2, i)))
	  res += str_check_size(lnnanform % "", total_number_row_digit_);
	else
	  res += str_check_size(lnform %
		(int) floor((line_(i1,i2, i) - refrow) * lnscale + 0.5),
		total_number_row_digit_);
	if(std::isnan(sample_(i1,i2, i)))
	  res += str_check_size(smpnanform % "", total_number_col_digit_);
	else
	  res += str_check_size(smpform % 
		(int) floor((sample_(i1,i2, i) - refcol) * smpscale + 0.5),
		total_number_col_digit_);
      }
  }
  // Don't fill in the row and column fit error
  // std::string row_fit_error="";
  // std::string col_fit_error="";
  // res += str_check_size(secformat % row_section_number_ % col_section_number_
  // 			% row_fit_error % col_fit_error,
  // 			3 + 3 + 21 + 21);
  // res += str_check_size(scaleformat % line_offset_ % sample_offset_ %
  // 			x_offset_ % y_offset_ % z_offset_, 5 * 21);
  // res += str_check_size(scaleformat % line_scale_ % sample_scale_ %
  // 			x_scale_ % y_scale_ % z_scale_, 5 * 21);
  // res += line_num_.tre_string();
  // res += line_den_.tre_string();
  // res += sample_num_.tre_string();
  // res += sample_den_.tre_string();
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string. 
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmGrid>
RsmGrid::read_tre_string(const std::string& Tre_in)
{
  // Have check in place for xoffset or number of pixels being
  // different from one grid level to the next. We don't currently
  // support this. Only matters for reading RsmGrid that we didn't
  // generate ourselves. We can add support for this is needed, just
  // need to work through saving the offset/number and making sure
  // line_ and sample_ are big enough. Note we use the convention that
  // line_ doesn't account for xoffset, instead we have an array big
  // enough for all the data. We pad it with NaN on the outside, and
  // only trim when we read and write the TRE (we could revisit this
  // if needed, but seems to make a difference in speed of using the
  // grid).
  
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmGrid> res(new RsmGrid);
  res->base_read_tre_string(in);
  res->row_section_number_ = read_size<int>(in, 3);
  res->col_section_number_ = read_size<int>(in, 3);
  std::string trash = read_size<std::string>(in, 21);
  trash = read_size<std::string>(in, 21);
  trash = read_size<std::string>(in, 1);
  int num_z = read_size<int>(in, 3);
  res->z_delta_ = read_size<double>(in, 21);
  res->x_delta_ = read_size<double>(in, 21);
  res->y_delta_ = read_size<double>(in, 21);
  res->z_start_ = read_size<double>(in, 21);
  res->x_start_ = read_size<double>(in, 21);
  res->y_start_ = read_size<double>(in, 21);
  int refrow = read_size<int>(in, 9);
  int refcol = read_size<int>(in, 9);
  res->total_number_row_digit_ = read_size<int>(in, 2);
  res->total_number_col_digit_ = read_size<int>(in, 2);
  res->number_fractional_row_digit_ = read_size<int>(in, 1);
  res->number_fractional_col_digit_ = read_size<int>(in, 1);
  // Values are set by tre, so just put in dummy values here
  res->min_line_ = 0;
  res->max_line_ = 0;
  res->min_sample_ = 0;
  res->max_sample_ = 0;
  res->ignore_igc_error_in_fit_ = true;
  for(int i = 1; i < num_z; ++i) {
    int xoff = read_size<int>(in, 4);
    int yoff = read_size<int>(in, 4);
    // Offset held to 0 for now
    if(xoff != 0 || yoff != 0) {
      Exception e;
      e << "We don't currently support a initial x or y offset other than 0\n"
	<< "Z index:  " << i << "\n"
	<< "X offset: " << xoff << "\n"
	<< "Y offset: " << yoff << "\n";
      throw e;
    }
  }
  int num_x = -1;
  int num_y = -1;
  double lnscale = pow(10, res->number_fractional_row_digit_);
  double smpscale = pow(10, res->number_fractional_row_digit_);
  for(int i = 0; i < num_z; ++i) {
    int num_x_new = read_size<int>(in, 3);
    int num_y_new = read_size<int>(in, 3);
    if(i == 0) {
      num_x = num_x_new;
      num_y = num_y_new;
      res->line_.resize(num_x, num_y, num_z);
      res->sample_.resize(num_x, num_y, num_z);
    }
    if(num_x_new != num_x || num_y_new != num_y) {
      Exception e;
      e << "We don't currently support variable x/y size\n"
	<< "Z index:  " << i << "\n"
	<< "Number x: " << num_x_new << "\n"
	<< "Number y: " << num_y_new << "\n"
	<< "Number x previous layer: " << num_x << "\n"
	<< "Number y previous layer: " << num_y << "\n";
      throw e;
    }
    for(int i1 = 0; i1 < num_x; ++i1)
      for(int i2 = 0; i2 < num_y; ++i2) {
	res->line_(i1, i2, i) =
	  (read_size_nan(in, res->total_number_row_digit_) / lnscale) + refrow;
	res->sample_(i1, i2, i) =
	  (read_size_nan(in, res->total_number_col_digit_) / smpscale) + refcol;
      }
  }
  // res->nline_fit_ = 20;
  // res->nsample_fit_ = 20;
  // res->nheight_fit_ = 20;
  // res->nsecond_pass_fit_ = 20;
  // res->ignore_igc_error_in_fit_ = false;
  // res->row_section_number_ = read_size<int>(in, 3);
  // res->col_section_number_ = read_size<int>(in, 3);
  // std::string trash = read_size<std::string>(in, 21);
  // trash = read_size<std::string>(in, 21);
  // res->line_offset_ = read_size<double>(in, 21);
  // res->sample_offset_ = read_size<double>(in, 21);
  // res->x_offset_ = read_size<double>(in, 21);
  // res->y_offset_ = read_size<double>(in, 21);
  // res->z_offset_ = read_size<double>(in, 21);
  // res->line_scale_ = read_size<double>(in, 21);
  // res->sample_scale_ = read_size<double>(in, 21);
  // res->x_scale_ = read_size<double>(in, 21);
  // res->y_scale_ = read_size<double>(in, 21);
  // res->z_scale_ = read_size<double>(in, 21);
  // res->line_num_.read_tre_string(in);
  // res->line_den_.read_tre_string(in);
  // res->sample_num_.read_tre_string(in);
  // res->sample_den_.read_tre_string(in);
  check_end_of_stream(in);
  return res;
}
