#include "rsm_multi_section.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
// Don't have serialization defined yet for
// blitz::Array<boost::shared_ptr<RsmBase>, 2>, so define that
typedef blitz::Array<boost::shared_ptr<RsmBase>, 2> blitz_rsm_base_array_2d;
BOOST_SERIALIZATION_SPLIT_FREE(blitz_rsm_base_array_2d);

template<class Archive, class T>
void boost::serialization::save(Archive& ar, const blitz::Array<T, 2>& A, 
			      const unsigned version) 
{
  using boost::serialization::make_array;
  if(A.size() > 0 && !A.isStorageContiguous())
    throw GeoCal::Exception("We can only save contiguous matrix data");
  int rows = A.rows();
  int cols = A.cols();
  ar << GEOCAL_NVP(rows) << GEOCAL_NVP(cols);
  if(A.size() > 0)
    ar << GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template<typename Archive, class T>
void boost::serialization::load(Archive& ar, blitz::Array<T, 2>& A, 
				const unsigned version) 
{
  using boost::serialization::make_array;
  int rows, cols;
  ar >> GEOCAL_NVP(rows) >> GEOCAL_NVP(cols);
  A.resize(rows, cols);
  if(A.size() > 0)
    ar >> GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template void boost::serialization::load(polymorphic_iarchive& ar, 
					 blitz::Array<boost::shared_ptr<RsmBase>, 2>& A, 
					 const unsigned version);
template void boost::serialization::save(polymorphic_oarchive& ar, 
					 const blitz::Array<boost::shared_ptr<RsmBase>, 2>& A, 
					 const unsigned version);

template<class Archive>
void RsmMultiSection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBase)
    & GEOCAL_NVP(nline_sec)
    & GEOCAL_NVP(nsamp_sec)
    & GEOCAL_NVP_(border)
    & GEOCAL_NVP(sec)
    & GEOCAL_NVP(lp);
}

GEOCAL_IMPLEMENT(RsmMultiSection);
#endif

//-----------------------------------------------------------------------
/// Constructor.
///
/// The RsmRationalPolynomial tends to extrapolate badly. Because the
/// low order polynomial is only approximately correct, we add a little
/// bit of a border to each underlying RsmRationalPolynomial so we can
/// avoid extrapolating.
//-----------------------------------------------------------------------

RsmMultiSection::RsmMultiSection
(int Nline, int Nsamp, int Nrow_section, int Ncol_section,
 const RsmBase& Rsm_prototype, int Border)
  : border_(Border), sec(Nrow_section, Ncol_section)
{
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      sec(i,j) = Rsm_prototype.clone();
  nline_sec = double(Nline) / sec.rows();
  nsamp_sec = double(Nsamp) / sec.cols();
}

//-----------------------------------------------------------------------
/// Apply the grid to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmMultiSection::image_coordinate
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

blitz::Array<double, 3> RsmMultiSection::image_coordinate
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

blitz::Array<double, 4> RsmMultiSection::image_coordinate
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

void RsmMultiSection::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample,
 int Nline, int Nsample, int Nheight,
 bool Skip_masked_point,
 bool Ignore_error)
{
  lp.fit(Igc, Cconv, Min_height, Max_height, Min_line, Max_line, Min_sample,
	 Max_sample);
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      sec(i,j)->fit(Igc, Cconv, Min_height, Max_height,
		    std::max(int(nline_sec * i) - border_, Min_line),
		    std::min(int(nline_sec * (i + 1)) + border_, Max_line),
		    std::max(int(nsamp_sec * j) - border_, Min_sample),
		    std::min(int(nsamp_sec * (j + 1)) + border_, Max_sample),
		    Nline, Nsample, Nheight,
		    Skip_masked_point, Ignore_error);
}


//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmMultiSection::print(std::ostream& Os) const
{
  Os << "RsmMultiSection:\n"
     << "  Number rows: " << sec.rows() << "\n"
     << "  Number cols: " << sec.cols() << "\n";
}

double RsmMultiSection::min_x() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->min_x();
      else
	res = std::min(res, sec(i,j)->min_x());
  return res;
}

double RsmMultiSection::max_x() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->max_x();
      else
	res = std::max(res, sec(i,j)->max_x());
  return res;
}

double RsmMultiSection::min_y() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->min_y();
      else
	res = std::min(res, sec(i,j)->min_y());
  return res;
}

double RsmMultiSection::max_y() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->max_y();
      else
	res = std::max(res, sec(i,j)->max_y());
  return res;
}

double RsmMultiSection::min_z() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->min_z();
      else
	res = std::min(res, sec(i,j)->min_z());
  return res;
}

double RsmMultiSection::max_z() const
{
  double res = 0;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      if(i == 0 && j == 0)
	res = sec(i,j)->max_z();
      else
	res = std::max(res, sec(i,j)->max_z());
  return res;
}

const RsmBase& RsmMultiSection::section_ls(double Line, double Sample) const
{
  int i = floor(Line / nline_sec);
  int j = floor(Sample / nsamp_sec);
  i = std::min(std::max(i,0), sec.rows() - 1);
  j = std::min(std::max(j,0), sec.cols() - 1);
  return *sec(i,j);
}

const RsmBase& RsmMultiSection::section_xyz(double X, double Y, double Z) const
{
  ImageCoordinate ic = lp.image_coordinate(X,Y,Z);
  return section_ls(ic.line,ic.sample);
}
