#include "rsm_multi_section.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "local_rectangular_coordinate.h"

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
      sec(i,j)->rsm_support_data_edition(Rsm_support_data_edition);
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
  fit_start(Igc, Cconv, Min_height, Max_height);
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      fit_section(i, j, Igc, Cconv, Min_height, Max_height);
}

void RsmMultiSection::fit_data(const blitz::Array<double, 2>& Data,
	       int Min_line, int Max_line, int Min_sample, int Max_sample)
{
  fit_start(Data);
  lp.min_line(0);
  lp.max_line(Max_line);
  lp.min_sample(0);
  lp.max_sample(Max_sample);
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      fit_section(i, j, Data);
}

//-----------------------------------------------------------------------
/// Fit just the low order polynomial. This is intended for doing
/// parallel fitting of the sections in python.
//-----------------------------------------------------------------------

void RsmMultiSection::fit_start(const ImageGroundConnection& Igc,
				const CoordinateConverter& Cconv,
				double Min_height, double Max_height)
{
  lp.fit(Igc, Cconv, Min_height, Max_height, 0, Igc.number_line() - 1,
	 0, Igc.number_sample() - 1);
  // Based on examples, the MSP library seems to prefer that all
  // sections have the same z start and delta. This isn't actually
  // required from the RSM standard, but seems to be one of the those
  // "unstated" requirements. This gives access  for RsmMultiSection to
  // pass the z values to use, rather than computing this in fit.
  bool first = true;
  double min_x=0, max_x=0, min_y=0, max_y=0, min_z=0, max_z=0;
  for(double i = 0; i < 2; ++i)
    for(double j = 0; j < 2; ++j)
      for(double k = 0; k < 2; ++k) {
	double ln = (i == 0 ? 0 : Igc.number_line() - 1);
	double smp = (j == 0 ? 0 : Igc.number_sample() - 1);
	double h = (k == 0 ? Min_height : Max_height);
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
      }
  double pad = 0.05;
  min_z -= (Max_height - Min_height) * pad;
  max_z += (Max_height - Min_height) * pad;
  // Right now, only works with LocalRcConverter. Should perhaps
  // add handling for other cases, but for now require this.
  if(dynamic_cast<const LocalRcConverter*>(&Cconv)) {
    for(int i = 0; i < sec.rows(); ++i)
      for(int j = 0; j < sec.cols(); ++j)
	sec(i,j)->set_z_range(min_z, max_z);
  }
}

void RsmMultiSection::fit_start(const blitz::Array<double, 2>& Data)
{
  blitz::Range ra = blitz::Range::all();
  std::vector<double> ln, smp, x, y, z;
  for(int i = 0; i < Data.rows(); ++i) {
    ln.push_back(Data(i,0));
    smp.push_back(Data(i,1));
    x.push_back(Data(i,2));
    y.push_back(Data(i,3));
    z.push_back(Data(i,4));
  }
  lp.fit_data(ln, smp, x, y, z);
  // Based on examples, the MSP library seems to prefer that all
  // sections have the same z start and delta. This isn't actually
  // required from the RSM standard, but seems to be one of the those
  // "unstated" requirements. This gives access  for RsmMultiSection to
  // pass the z values to use, rather than computing this in fit.
  // Note this is only used by RsmGrid, RsmRationalPolynomial just
  // ignores this.
  double min_z = blitz::min(Data(ra,4));
  double max_z = blitz::max(Data(ra,4));
  double pad = 0.05;
  double p = pad * (max_z - min_z);
  min_z -= p;
  max_z += p;
  for(int i = 0; i < sec.rows(); ++i)
    for(int j = 0; j < sec.cols(); ++j)
      sec(i,j)->set_z_range(min_z, max_z);
}


//-----------------------------------------------------------------------
/// Fit a single segment. This is intended for doing parallel fitting
/// in python.
//-----------------------------------------------------------------------

const boost::shared_ptr<RsmBase>&
RsmMultiSection::fit_section(int i, int j, const ImageGroundConnection& Igc,
	    const CoordinateConverter& Cconv,
	    double Min_height, double Max_height)
{
  range_check(i, 0, sec.rows());
  range_check(j, 0, sec.cols());
  sec(i,j)->fit(Igc, Cconv, Min_height, Max_height,
		std::max(int(nline_sec * i) - border_, 0),
		std::min(int(nline_sec * (i + 1)) + border_,
			 Igc.number_line()-1),
		std::max(int(nsamp_sec * j) - border_, 0),
		std::min(int(nsamp_sec * (j + 1)) + border_,
			 Igc.number_sample()-1));
  return sec(i,j);
}

const boost::shared_ptr<RsmBase>&
RsmMultiSection::fit_section(int i, int j, const blitz::Array<double, 2>& Data)
{
  range_check(i, 0, sec.rows());
  range_check(j, 0, sec.cols());
  sec(i,j)->fit_data(Data,
		std::max(int(nline_sec * i) - border_, 0),
		std::min(int(nline_sec * (i + 1)) + border_,
			 lp.max_line()),
		std::max(int(nsamp_sec * j) - border_, 0),
		std::min(int(nsamp_sec * (j + 1)) + border_,
			 lp.max_sample()));
  return sec(i,j);
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

// int RsmMultiSection::min_line() const
// {
//   int res = 0;
//   for(int i = 0; i < sec.rows(); ++i)
//     for(int j = 0; j < sec.cols(); ++j)
//       if(i == 0 && j == 0)
// 	res = sec(i,j)->min_line();
//       else
// 	res = std::min(res, sec(i,j)->min_line());
//   return res;
// }

// int RsmMultiSection::max_line() const
// {
//   int res = 0;
//   for(int i = 0; i < sec.rows(); ++i)
//     for(int j = 0; j < sec.cols(); ++j)
//       if(i == 0 && j == 0)
// 	res = sec(i,j)->max_line();
//       else
// 	res = std::max(res, sec(i,j)->max_line());
//   return res;
// }

// int RsmMultiSection::min_sample() const
// {
//   int res = 0;
//   for(int i = 0; i < sec.rows(); ++i)
//     for(int j = 0; j < sec.cols(); ++j)
//       if(i == 0 && j == 0)
// 	res = sec(i,j)->min_sample();
//       else
// 	res = std::min(res, sec(i,j)->min_sample());
//   return res;
// }

// int RsmMultiSection::max_sample() const
// {
//   int res = 0;
//   for(int i = 0; i < sec.rows(); ++i)
//     for(int j = 0; j < sec.cols(); ++j)
//       if(i == 0 && j == 0)
// 	res = sec(i,j)->max_sample();
//       else
// 	res = std::max(res, sec(i,j)->max_sample());
//   return res;
// }

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

//-----------------------------------------------------------------------
/// Return the section that handles the given image coordinate line
/// and sample.
//-----------------------------------------------------------------------

const boost::shared_ptr<RsmBase>&
RsmMultiSection::section_ls(double Line, double Sample) const
{
  int i = floor(Line / nline_sec);
  int j = floor(Sample / nsamp_sec);
  i = std::min(std::max(i,0), sec.rows() - 1);
  j = std::min(std::max(j,0), sec.cols() - 1);
  return sec(i,j);
}

//-----------------------------------------------------------------------
/// Use low order polynomial to determine section for a particular
/// coordinate.
//-----------------------------------------------------------------------

const boost::shared_ptr<RsmBase>&
RsmMultiSection::section_xyz(double X, double Y, double Z) const
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
  res->lp.max_line((int) floor(res->nline_sec * nrow + 0.5) - 1);
  res->lp.min_sample(0);
  res->lp.max_sample((int) floor(res->nsamp_sec * ncol + 0.5) - 1);
  check_end_of_stream(in);
  return res;
}
