#include "rsm_multi_section.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"

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
 const RsmBase& Rsm_prototype, int Border,
 const std::string& Image_identifier,
 const std::string& Rsm_support_data_edition
)
  : RsmBase(Image_identifier, Rsm_support_data_edition),
    border_(Border), sec(Nrow_section, Ncol_section)
{
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j) {
      sec(i,j) = Rsm_prototype.clone();
      sec(i,j)->image_identifier(Image_identifier);
      sec(i,j)->rsm_suport_data_edition(Rsm_support_data_edition);
      sec(i,j)->row_section_number(i + 1);
      sec(i,j)->col_section_number(j + 1);
    }
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
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmMultiSection::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample)
{
  lp.fit(Igc, Cconv, Min_height, Max_height, Min_line, Max_line, Min_sample,
	 Max_sample);
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      sec(i,j)->fit(Igc, Cconv, Min_height, Max_height,
		    std::max(int(nline_sec * i) - border_, Min_line),
		    std::min(int(nline_sec * (i + 1)) + border_, Max_line),
		    std::max(int(nsamp_sec * j) - border_, Min_sample),
		    std::min(int(nsamp_sec * (j + 1)) + border_, Max_sample));
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

static boost::format secformat("%|1$03d|%|2$03d|%|3$03d|%|4$+21.14E|%|5$+21.14E|");

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note also that the TRE has a fixed precision which is less than
/// the machine precision. Writing a RsmMultiSection and then
/// reading it from a TRE does *not* in general give the exact same
/// RsmRationalPolynomial, rather just one that is close.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
///
/// We do *not* write out the actually RsmBase that make up the
/// section, this writing is handled separately.
//-----------------------------------------------------------------------

std::string RsmMultiSection::tre_string() const
{
  std::string res = base_tre_string();
  res += lp.tre_string();
  res += str_check_size(secformat % sec.rows() % sec.cols() % sec.size()
			% nline_sec % nsamp_sec, 3 * 3 + 2 * 21);
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string. Note that the TRE does not contain all the
/// fields we have in a RsmMultiSection. However the fields that
/// aren't contained are ones used for fitting the RSM, so in practice
/// this doesn't matter. We just set the various fields to the default
/// values found in the constructor.
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
///
/// We do *not* fill in the actual RsmBase stuff in sec, that is
/// handled separately. We do resize sec, but fill it with null pointers.
//-----------------------------------------------------------------------

boost::shared_ptr<RsmMultiSection>
RsmMultiSection::read_tre_string(const std::string& Tre_in)
{
  boost::shared_ptr<RsmMultiSection> res(new RsmMultiSection);
  res->border_ = 5;
  std::stringstream in(Tre_in);
  res->base_read_tre_string(in);
  res->lp.read_tre_string(in);
  int nrow = read_size<int>(in, 3);
  int ncol= read_size<int>(in, 3);
  int sz = read_size<int>(in, 3);
  if(sz != nrow * ncol) {
    Exception e;
    e << "Bad total size in TRE. We expect this to be nrow * nsize:\n"
      << "Nrow: " << nrow << "\n"
      << "Ncol: " << ncol << "\n"
      << "Sz:   " << sz << "\n";
    throw e;
  }
  res->sec.resize(nrow, ncol);
  for(int i = 0; i < nrow; ++i)
    for(int j = 0; j < ncol; ++j)
      res->sec(i,j).reset();
  res->nline_sec = read_size<double>(in, 21);
  res->nsamp_sec = read_size<double>(in, 21);
  res->lp.min_line(0);
  res->lp.max_line((int) floor(res->nline_sec * nrow + 0.5));
  res->lp.min_sample(0);
  res->lp.max_sample((int) floor(res->nsamp_sec * ncol + 0.5));
  check_end_of_stream(in);
  return res;
}
