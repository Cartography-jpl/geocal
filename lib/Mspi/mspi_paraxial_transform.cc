#include "mspi_paraxial_transform.h"
#include "mspi_config_file.h"
#include "auto_derivative.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;


#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void MspiParaxialTransform::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(MspiParaxialTransform);
  ar & GEOCAL_NVP(fname) 
    & GEOCAL_NVP(row_to_index)
    & GEOCAL_NVP_(a)
    & GEOCAL_NVP_(b)
    & GEOCAL_NVP_(c)
    & GEOCAL_NVP_(d);
}

GEOCAL_IMPLEMENT(MspiParaxialTransform);
#endif

const int number_a_parameter = 4;
const int number_c_parameter = 12;

//-----------------------------------------------------------------------
/// Constructor that reads the given file for the transform coefficients.
//-----------------------------------------------------------------------

MspiParaxialTransform::MspiParaxialTransform(const std::string& File_name)
  :fname(File_name)
{
  Range ra = Range::all();
  MspiConfigFile config(File_name);
  std::vector<int> row_numbers = 
    config.value<std::vector<int> >("paraxial_row_numbers");
  if((int) row_numbers.size() != config.value<int>("paraxial_number_row")) {
    Exception e;
    e << "The number of values in 'paraxial_row_numbers' doesn't match\n"
      << "the value of 'paraxial_number_row' in the file '"
      << File_name;
    throw e;
  }
  for(int i = 0; i < (int) row_numbers.size(); ++i)
    row_to_index[row_numbers[i]] = i;
  a_.resize((int) row_numbers.size(), number_a_parameter);
  b_.resize(a_.shape());
  c_.resize(a_.rows(), number_c_parameter);
  d_.resize(c_.shape());
  Array<double, 1> scratch;
  for(int i = 0; i < a_.rows(); ++i) {
    std::string lbl = boost::lexical_cast<std::string>(row_numbers[i]);
    scratch.reference(config.value<Array<double, 1> >("a" + lbl));
    if(scratch.rows() != a_.cols())
      throw Exception("Coefficient is the wrong size");
    a_(i, ra) = scratch;

    scratch.reference(config.value<Array<double, 1> >("b" + lbl));
    if(scratch.rows() != b_.cols())
      throw Exception("Coefficient is the wrong size");
    b_(i, ra) = scratch;

    scratch.reference(config.value<Array<double, 1> >("c" + lbl));
    if(scratch.rows() != c_.cols())
      throw Exception("Coefficient is the wrong size");
    c_(i, ra) = scratch;

    scratch.reference(config.value<Array<double, 1> >("d" + lbl));
    if(scratch.rows() != d_.cols())
      throw Exception("Coefficient is the wrong size");
    d_(i, ra) = scratch;
  }
}

//-----------------------------------------------------------------------
/// Convert from paraxial to real coordinates.
//-----------------------------------------------------------------------

void MspiParaxialTransform::paraxial_to_real
(int Row_number, double Paraxial_x, double Paraxial_y, double& Real_x, 
 double& Real_y) const
{
  Range ra = Range::all();
  if(!has_row(Row_number)) {
    Exception e;
    e << "Don't have row " << Row_number;
    throw e;
  }
  int index = row_to_index.find(Row_number)->second;
  Array<double, 1> cf = c_(index, ra);
  Array<double, 1> df = d_(index, ra);

  double xf = Paraxial_x;
  double yf = Paraxial_y;
  double xf2 = xf * xf;
  double xf3 = xf2 * xf;
  double xf4 = xf3 * xf;
  double xf5 = xf3 * xf2;
  double xf6 = xf5 * xf;
  double xf7 = xf5 * xf2;
  double yf2 = yf * yf;
  Real_x = (cf(0) * xf + 
	    cf(1) * xf * yf + 
	    cf(2) * xf * yf2 + 
	    cf(3) * xf3 + 
	    cf(4) * xf3 * yf + 
	    cf(5) * xf3 * yf2 + 
	    cf(6) * xf5 + 
	    cf(7) * xf5 * yf +
	    cf(8) * xf5 * yf2 + 
	    cf(9) * xf7 + 
	    cf(10) * xf7 * yf + 
	    cf(11) * xf7 * yf2);

  Real_y = (df(0) * 1 +
	    df(1) * yf +
	    df(2) * yf2 +
	    df(3) * xf2 +
	    df(4) * xf2 * yf +
	    df(5) * xf2 * yf2 +
	    df(6) * xf4 +
	    df(7) * xf4 * yf +
	    df(8) * xf4 * yf2 +
	    df(9) * xf6 +
	    df(10) * xf6 * yf +
	    df(11) * xf6 * yf2);
}

void MspiParaxialTransform::paraxial_to_real
(int Row_number, const AutoDerivative<double>& Paraxial_x,
 const AutoDerivative<double>& Paraxial_y, 
 AutoDerivative<double>& Real_x, 
 AutoDerivative<double>& Real_y) const
{
  Range ra = Range::all();
  if(!has_row(Row_number)) {
    Exception e;
    e << "Don't have row " << Row_number;
    throw e;
  }
  int index = row_to_index.find(Row_number)->second;
  Array<double, 1> cf = c_(index, ra);
  Array<double, 1> df = d_(index, ra);

  AutoDerivative<double> xf = Paraxial_x;
  AutoDerivative<double> yf = Paraxial_y;
  AutoDerivative<double> xf2 = xf * xf;
  AutoDerivative<double> xf3 = xf2 * xf;
  AutoDerivative<double> xf4 = xf3 * xf;
  AutoDerivative<double> xf5 = xf3 * xf2;
  AutoDerivative<double> xf6 = xf5 * xf;
  AutoDerivative<double> xf7 = xf5 * xf2;
  AutoDerivative<double> yf2 = yf * yf;
  Real_x = (cf(0) * xf + 
	    cf(1) * xf * yf + 
	    cf(2) * xf * yf2 + 
	    cf(3) * xf3 + 
	    cf(4) * xf3 * yf + 
	    cf(5) * xf3 * yf2 + 
	    cf(6) * xf5 + 
	    cf(7) * xf5 * yf +
	    cf(8) * xf5 * yf2 + 
	    cf(9) * xf7 + 
	    cf(10) * xf7 * yf + 
	    cf(11) * xf7 * yf2);

  Real_y = (df(0) * 1 +
	    df(1) * yf +
	    df(2) * yf2 +
	    df(3) * xf2 +
	    df(4) * xf2 * yf +
	    df(5) * xf2 * yf2 +
	    df(6) * xf4 +
	    df(7) * xf4 * yf +
	    df(8) * xf4 * yf2 +
	    df(9) * xf6 +
	    df(10) * xf6 * yf +
	    df(11) * xf6 * yf2);
}

//-----------------------------------------------------------------------
/// Convert from real coordinates to paraxial.
//-----------------------------------------------------------------------

void MspiParaxialTransform::real_to_paraxial
(int Row_number, double Real_x, double Real_y, double& Paraxial_x,
 double& Paraxial_y) const
{
  Range ra = Range::all();
  if(!has_row(Row_number)) {
    Exception e;
    e << "Don't have row " << Row_number;
    throw e;
  }
  int index = row_to_index.find(Row_number)->second;
  Array<double, 1> af = a_(index, ra);
  Array<double, 1> bf = b_(index, ra);

  double xfp = Real_x;
  double yfp = Real_y;

  double xfp2 = xfp * xfp;
  double xfp3 = xfp2 * xfp;
  double xfp4 = xfp3 * xfp;
  double xfp5 = xfp4 * xfp;
  double xfp6 = xfp5 * xfp;
  double xfp7 = xfp6 * xfp;

  Paraxial_x = (af(0) * xfp + 
		af(1) * xfp3 +
		af(2)  * xfp5 +
		af(3) * xfp7);

  Paraxial_y = (bf(0) +
		yfp +
		bf(1) * xfp2 +
		bf(2) * xfp4 +
		bf(3) * xfp6);
}

void MspiParaxialTransform::real_to_paraxial
(int Row_number, const AutoDerivative<double>& Real_x, 
 const AutoDerivative<double>& Real_y, AutoDerivative<double>& Paraxial_x,
 AutoDerivative<double>& Paraxial_y) const
{
  Range ra = Range::all();
  if(!has_row(Row_number)) {
    Exception e;
    e << "Don't have row " << Row_number;
    throw e;
  }
  int index = row_to_index.find(Row_number)->second;
  Array<double, 1> af = a_(index, ra);
  Array<double, 1> bf = b_(index, ra);

  AutoDerivative<double> xfp = Real_x;
  AutoDerivative<double> yfp = Real_y;

  AutoDerivative<double> xfp2 = xfp * xfp;
  AutoDerivative<double> xfp3 = xfp2 * xfp;
  AutoDerivative<double> xfp4 = xfp3 * xfp;
  AutoDerivative<double> xfp5 = xfp4 * xfp;
  AutoDerivative<double> xfp6 = xfp5 * xfp;
  AutoDerivative<double> xfp7 = xfp6 * xfp;

  Paraxial_x = (af(0) * xfp + 
		af(1) * xfp3 +
		af(2)  * xfp5 +
		af(3) * xfp7);

  Paraxial_y = (bf(0) +
		yfp +
		bf(1) * xfp2 +
		bf(2) * xfp4 +
		bf(3) * xfp6);
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------
void MspiParaxialTransform::print(std::ostream& Os) const
{
  Os << "MspiParaxialTransform:\n"
     << "  File name: " << file_name() << "\n";
}
