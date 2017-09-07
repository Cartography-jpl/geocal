#include "rsm_grid.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
#include "auto_derivative.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmGrid::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBase)
    & GEOCAL_NVP_(line)
    & GEOCAL_NVP_(sample)
    & GEOCAL_NVP_(x_start)
    & GEOCAL_NVP_(y_start)
    & GEOCAL_NVP_(z_start)
    & GEOCAL_NVP_(x_delta)
    & GEOCAL_NVP_(y_delta)
    & GEOCAL_NVP_(z_delta)
    & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP_(max_line)
    & GEOCAL_NVP_(min_sample)
    & GEOCAL_NVP_(max_sample);
}

GEOCAL_IMPLEMENT(RsmGrid);
#endif

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value.
//-----------------------------------------------------------------------

ImageCoordinate RsmGrid::image_coordinate
(double X, double Y, double Z) const
{
  double iv = (X - x_start_) / x_delta_;
  double jv = (Y - y_start_) / y_delta_;
  double kv = (Z - z_start_) / z_delta_;
  int i = (int) floor(iv);
  double i_delta = iv - i;
  int j = (int) floor(jv);
  double j_delta = jv - j;
  int k = (int) floor(kv);
  double k_delta = kv - k;
  if(i < 0 || i + 1 >= number_x() ||
     j < 0 || j + 1 >= number_y() ||
     k < 0 || k + 1 >= number_z())
    return ImageCoordinate(std::numeric_limits<double>::quiet_NaN(),
			   std::numeric_limits<double>::quiet_NaN());
  // Bilinear interpolation. Should add 3x3 lagrange interpolation at
  // some point according to the RSM documentation
  double xinter_line[2][2],
    xinter_sample[2][2];
  for(int i1 = 0; i1 < 2; ++i1)
    for(int i2 = 0; i2 < 2; ++i2) {
      xinter_line[i1][i2] = line_(i,j+i1,k+i2) +
	(line_(i+1,j+i1,k+i2)-line_(i,j+i1,k+i2)) * i_delta;
      xinter_sample[i1][i2] = sample_(i,j+i1,k+i2) +
	(sample_(i+1,j+i1,k+i2)-sample_(i,j+i1,k+i2)) * i_delta;
    }
  double yinter_line[2], yinter_sample[2];
  for(int i1 = 0; i1 < 2; ++i1) {
    yinter_line[i1] = xinter_line[0][i1] +
      (xinter_line[1][i1]-xinter_line[0][i1])*j_delta;
    yinter_sample[i1] = xinter_sample[0][i1] +
      (xinter_sample[1][i1]-xinter_sample[0][i1])*j_delta;
  }
  double ln = yinter_line[0] + (yinter_line[1]-yinter_line[0])*k_delta;
  double smp = yinter_sample[0] +
    (yinter_sample[1]-yinter_sample[0])*k_delta;

  // Bunch of diagnostic messages. Leave stubbed out for now, we may
  // want this when we go to the lagrange interpolation to debug
  // problems.
  if(false) {
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	for(int i3 = 0; i3 < 2; ++i3)
	  std::cerr << "line(" << i1 << ", " << i2 << ", " << i3 << "): "
		    << line_(i,j+i1,k+i2) << "\n";
    for(int i1 = 0; i1 < 2; ++i1)
      for(int i2 = 0; i2 < 2; ++i2) 
	for(int i3 = 0; i3 < 2; ++i3)
	  std::cerr << "sample(" << i1 << ", " << i2 << ", " << i3 << "): "
		    << sample_(i,j+i1,k+i2) << "\n";
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
 const blitz::Array<double, 1>& Z) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 2> res(2, X.rows());
  for(int i = 0; i < X.rows(); ++i) {
    ImageCoordinate ic = image_coordinate(X(i), Y(i), Z(i));
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
 const blitz::Array<double, 2>& Z) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 3> res(2, X.rows(), X.cols());
  for(int i = 0; i < X.rows(); ++i)
    for(int j = 0; j < X.cols(); ++j) {
      ImageCoordinate ic = image_coordinate(X(i,j), Y(i,j), Z(i,j));
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
 const blitz::Array<double, 3>& Z) const
{
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols() ||
     X.depth() != Y.depth() || X.depth() != Z.depth())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 4> res(2, X.rows(), X.cols(), X.depth());
  for(int i = 0; i < X.rows(); ++i)
    for(int j = 0; j < X.cols(); ++j)
      for(int k = 0; k < X.depth(); ++k) {
	ImageCoordinate ic = image_coordinate(X(i,j,k), Y(i,j,k), Z(i,j,k));
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
/// This routine always ignores ImageGroundConnectionFailed
/// exceptions, and just skips to the next point. But if we are using
/// python code for the ImageGroundConnection we can't translate
/// errors to ImageGroundConnectionFailed (this is a limitation of
/// SWIG). So you can optionally specify Ignore_error as true, in
/// which case we ignore *all* exceptions and just skip to the next
/// point.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmGrid::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample,
 int Nline, int Nsample, int Nheight,
 bool Skip_masked_point,
 bool Ignore_error)
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
	double h = (k == 0 ? Min_height : Max_height);
	try {
	  boost::shared_ptr<GroundCoordinate> gc =
	    Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp), h);
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
	  if(!Ignore_error)
	    throw;
	}
      }
  x_start_ = min_x;
  y_start_ = min_y;
  z_start_ = min_z;
  x_delta_ = (max_x - min_x) / (number_x() - 1);
  y_delta_ = (max_y - min_y) / (number_y() - 1);
  z_delta_ = (max_z - min_z) / (number_z() - 1);
  for(int i = 0; i < number_x(); ++i)
    for(int j = 0; j < number_y(); ++j)
      for(int k = 0; k < number_z(); ++k) {
	double x = x_start_ + x_delta_ * i;
	double y = y_start_ + y_delta_ * j;
	double z = z_start_ + z_delta_ * k;
	boost::shared_ptr<GroundCoordinate> gc =
	  Cconv.convert_from_coordinate(x, y, z);
	ImageCoordinate ic;
	bool success;
	Igc.image_coordinate_with_status(*gc, ic, success);
	if(success) {
	  line_(i,j,k) = ic.line;
	  sample_(i,j,k) = ic.sample;
	} else {
	  line_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	  sample_(i,j,k) = std::numeric_limits<double>::quiet_NaN();
	}
      }
}

//-----------------------------------------------------------------------
/// This the jacobian of the line, sample with respect to X, Y, Z.
/// This is a 2x3 matrix.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmGrid::image_coordinate_jacobian
(double X, double Y, double Z) const
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
  AutoDerivative<double> i_delta = iv - i;
  int j = (int) floor(jv.value());
  AutoDerivative<double> j_delta = jv - j;
  int k = (int) floor(kv.value());
  AutoDerivative<double> k_delta = kv - k;
  if(i < 0 || i + 1 >= number_x() ||
     j < 0 || j + 1 >= number_y() ||
     k < 0 || k + 1 >= number_z()) {
    res = 0;
    return res;
  }
  // Bilinear interpolation. Should add 3x3 lagrange interpolation at
  // some point according to the RSM documentation
  AutoDerivative<double> xinter_line[2][2],
    xinter_sample[2][2];
  for(int i1 = 0; i1 < 2; ++i1)
    for(int i2 = 0; i2 < 2; ++i2) {
      xinter_line[i1][i2] = line_(i,j+i1,k+i2) +
	(line_(i+1,j+i1,k+i2)-line_(i,j+i1,k+i2)) * i_delta;
      xinter_sample[i1][i2] = sample_(i,j+i1,k+i2) +
	(sample_(i+1,j+i1,k+i2)-sample_(i,j+i1,k+i2)) * i_delta;
    }
  AutoDerivative<double> yinter_line[2], yinter_sample[2];
  for(int i1 = 0; i1 < 2; ++i1) {
    yinter_line[i1] = xinter_line[0][i1] +
      (xinter_line[1][i1]-xinter_line[0][i1])*j_delta;
    yinter_sample[i1] = xinter_sample[0][i1] +
      (xinter_sample[1][i1]-xinter_sample[0][i1])*j_delta;
  }
  AutoDerivative<double> ln =
    yinter_line[0] + (yinter_line[1]-yinter_line[0])*k_delta;
  AutoDerivative<double> smp = yinter_sample[0] +
    (yinter_sample[1]-yinter_sample[0])*k_delta;
  res(0,ra) = ln.gradient();
  res(1,ra) = smp.gradient();
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmGrid::print(std::ostream& Os) const
{
  Os << "RsmGrid:\n"
     << "  Number x: " << number_x() << "\n"
     << "  Number y: " << number_y() << "\n"
     << "  Number z: " << number_z() << "\n"
     << "  Start:    (" << x_start() << ", " << y_start() << ", " << z_start()
     << ")\n"
     << "  Delta:    (" << x_delta() << ", " << y_delta() << ", " << z_delta()
     << ")\n";
}
