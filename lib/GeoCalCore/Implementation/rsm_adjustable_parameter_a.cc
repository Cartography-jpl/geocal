#include "rsm_adjustable_parameter_a.h"
#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "planet_coordinate.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmAdjustableParameterA::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmAdjustableParameter)
    & GEOCAL_NVP(cconv) & GEOCAL_NVP(parm_index)
    & GEOCAL_NVP_(full_parameter);
}

GEOCAL_IMPLEMENT(RsmAdjustableParameterA);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

static boost::format numint("%|1$02d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");

std::string RsmAdjustableParameterA::tre_string() const
{
  std::string res = base_tre_string();
  res += str_check_size(numint % num_parameter(), 2);
  const LocalRcConverter* gconv = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(!gconv)
    throw Exception("Writing a RSMAPA TRE only supports LocalRcConverter. This is by definition of how the RSM adjustable parameters work");
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num % gconv->parameter()->cf_offset[i], 21);
  // This is column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      res += str_check_size(num % gconv->parameter()->cf_to_rc[i][j], 21);
  for(int i = 0; i < parm_index.rows(); ++i) {
    if(parm_index(i) > 0)
      res += str_check_size(numint % parm_index(i), 2);
    else
      res += str_check_size(numint_missing % "", 2);
  }
  blitz::Array<double, 1> parm = parameter();
  for(int i = 0; i < parm.rows(); ++i)
    res += str_check_size(num % parm(i), 21);
  return res;
}

//-----------------------------------------------------------------------
/// Create a RsmAdjustableParameterA. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id, and
/// can optionally activate a set of the parameters for the correction.
//-----------------------------------------------------------------------

RsmAdjustableParameterA::RsmAdjustableParameterA
(const ImageGroundConnection& Igc,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id,
 bool Activate_image_correction,
 bool Activate_ground_rotation,
 bool Activate_ground_correction
 )
  : RsmAdjustableParameter(Rsm_id.image_identifier(),
			   Rsm_id.rsm_support_data_edition(),
			   Triangulation_id),
    parm_index(36),
    full_parameter_(36, 0)
{
  for(int i = 0; i < full_parameter_.rows(); ++i) {
    full_parameter_(i) = 0.0;
    parm_index(i) = -1;
  }
  cconv = boost::make_shared<LocalRcConverter>(boost::make_shared<LocalRcParameter>(Igc));
  if(Activate_image_correction)
    activate_image_correction();
  if(Activate_ground_rotation)
    activate_ground_rotation();
  if(Activate_ground_correction)
    activate_ground_correction();
}


//-----------------------------------------------------------------------
/// Read a TRE string. Note that the TRE does not contain all the
/// fields we have in a RsmRationalPolynomial. However the fields that
/// aren't contained are ones used for fitting the RSM, so in practice
/// this doesn't matter. We just set the various fields to the default
/// values found in the constructor.
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmAdjustableParameterA>
RsmAdjustableParameterA::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmAdjustableParameterA> res(new RsmAdjustableParameterA);
  res->base_read_tre_string(in);
  int nparm = read_size<int>(in, 2);
  blitz::Array<double, 1> parm(nparm);
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>();
  lp->cf_prototype = boost::make_shared<Ecr>(0,0,0);
  for(int i = 0; i < 3; ++i)
    lp->cf_offset[i] = read_size<double>(in, 21);
  // Column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      lp->cf_to_rc[i][j] = read_size<double>(in, 21);
  res->cconv = boost::make_shared<LocalRcConverter>(lp);
  for(int i = 0; i < res->parm_index.rows(); ++i) {
    boost::optional<int> ind = read_size<boost::optional<int> >(in, 2);
    if(ind)
      res->parm_index(i) = *ind;
    else
      res->parm_index(i) = -1;
  }
  for(int i = 0; i < parm.rows(); ++i)
    parm(i) = read_size<double>(in, 21);
  res->parameter(parm);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmAdjustableParameterA::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmAdjustableParameterA:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_support_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n"
     << "  Local Coordinate System:\n";
  opad << *cconv << "\n";
  opad.strict_sync();
  Os << "  Parameter:\n";
  blitz::Array<double, 1> p = parameter_subset();
  std::vector<std::string> pname = parameter_name_subset();
  for(int i = 0; i < p.rows(); ++i)
    Os << "    " << pname[i] << ": " << p(i) << "\n";
}

//-----------------------------------------------------------------------
/// Name of the full parameters
//-----------------------------------------------------------------------

const std::vector<std::string>&
RsmAdjustableParameterA::full_parameter_name()
{
  static bool filled_in = false;
  static std::vector<std::string> res;
  if(!filled_in) {
    res.push_back("Image row constant");
    res.push_back("Image row X");
    res.push_back("Image row Y");
    res.push_back("Image row Z");
    res.push_back("Image row X*X");
    res.push_back("Image row X*Y");
    res.push_back("Image row X*Z");
    res.push_back("Image row Y*Y");
    res.push_back("Image row Y*Z");
    res.push_back("Image row Z*Z");
    res.push_back("Image col constant");
    res.push_back("Image col X");
    res.push_back("Image col Y");
    res.push_back("Image col Z");
    res.push_back("Image col X*X");
    res.push_back("Image col X*Y");
    res.push_back("Image col X*Z");
    res.push_back("Image col Y*Y");
    res.push_back("Image col Y*Z");
    res.push_back("Image col Z*Z");
    res.push_back("Ground X constant");
    res.push_back("Ground Y constant");
    res.push_back("Ground Z constant");
    res.push_back("Ground rotation X");
    res.push_back("Ground rotation Y");
    res.push_back("Ground rotation Z");
    res.push_back("Ground scale");
    res.push_back("Ground X proportional to X index");
    res.push_back("Ground X proportional to Y index");
    res.push_back("Ground X proportional to Z index");
    res.push_back("Ground Y proportional to X index");
    res.push_back("Ground Y proportional to Y index");
    res.push_back("Ground Y proportional to Z index");
    res.push_back("Ground Z proportional to X index");
    res.push_back("Ground Z proportional to Y index");
    res.push_back("Ground Z proportional to Z index");
    filled_in = true;
  }
  return res;
}

//-----------------------------------------------------------------------
/// Any set of parameters can be activated, in any desired order. But
/// we tend to activate them in groups. This function turns on all of
/// image correction parameters.
//-----------------------------------------------------------------------
 
void RsmAdjustableParameterA::activate_image_correction()
{
  int ind = (int) num_parameter();
  for(int i = 0; i < 20; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
}

//-----------------------------------------------------------------------
/// Any set of parameters can be activated, in any desired order. But
/// we tend to activate them in groups. This function turns on all of
/// ground rotation parameters.
//-----------------------------------------------------------------------
 
void RsmAdjustableParameterA::activate_ground_rotation()
{
  int ind = (int) num_parameter();
  for(int i = 20; i < 27; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
}

// See base class for description.
bool RsmAdjustableParameterA::has_ground_coordinate_parameter() const
{
  for(int i = 20; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      return true;
  return false;
}

//-----------------------------------------------------------------------
/// Any set of parameters can be activated, in any desired order. But
/// we tend to activate them in groups. This function turns on all of
/// ground correction parameters.
//-----------------------------------------------------------------------
 
void RsmAdjustableParameterA::activate_ground_correction()
{
  int ind = (int) num_parameter();
  for(int i = 20; i < 23; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
  for(int i = 28; i < 36; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
}
 
// See base class for description
ArrayAd<double, 1> RsmAdjustableParameterA::parameter_with_derivative() const
{
  ArrayAd<double, 1> res(num_parameter(), full_parameter_.number_variable());
  for(int i = 0; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      res(parm_index(i) - 1) = full_parameter_(i);
  return res;
}

// See base class for description
void RsmAdjustableParameterA::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != num_parameter()) {
    Exception e;
    e << "Wrong size parameter. Size " << Parm.rows()
      << " expected " << num_parameter();
    throw e;
  }
  full_parameter_.resize_number_variable(Parm.number_variable());
  for(int i = 0; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      full_parameter_(i) = Parm(parm_index(i) - 1);
    else
      full_parameter_(i) = 0;
}

int RsmAdjustableParameterA::num_parameter() const
{
  int nparm = 0;
  for(int i = 0; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      ++nparm;
  return nparm;
}
  
std::vector<std::string> RsmAdjustableParameterA::parameter_name() const
{
  const std::vector<std::string>& fp = full_parameter_name();
  std::vector<std::string> res;
  int nparm = num_parameter();
  for(int i = 0; i < nparm; ++i)
    res.push_back("");
  for(int i = 0; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      res[parm_index(i) - 1] = fp[i];
  return res;
}

//-----------------------------------------------------------------------
/// Calculate the delta for the ground x, y, z. This comes from the
/// RSM documentation (RSM TRE Specificatino for NITF 2.1, page 96).
//-----------------------------------------------------------------------

void RsmAdjustableParameterA::delta_x
(double X, double Y, double Z, double& xdelta, double& ydelta,
 double& zdelta) const
{
  const blitz::Array<double, 1>& p = full_parameter_.value();
  double dx = p(20);
  double dy = p(21);
  double dz = p(22);
  double alpha = p(23);
  double beta = p(24);
  double kappa = p(25);
  double s = p(26);
  xdelta = dx + s * X     + kappa * Y - beta * Z; 
  ydelta = dy - kappa * X + s * Y     + alpha * Z;
  zdelta = dz + beta * X  - alpha * Y + s * Z;
  xdelta += p(27) * X + p(28) * Y + p(29) * Z;
  ydelta += p(30) * X + p(31) * Y + p(32) * Z;
  zdelta += p(33) * X + p(34) * Y + p(35) * Z;
}

void RsmAdjustableParameterA::delta_x
(double X, double Y, double Z, AutoDerivative<double>& xdelta,
 AutoDerivative<double>& ydelta,
 AutoDerivative<double>& zdelta) const
{
  const ArrayAd<double, 1>& p = full_parameter_;
  AutoDerivative<double> dx = p(20);
  AutoDerivative<double> dy = p(21);
  AutoDerivative<double> dz = p(22);
  AutoDerivative<double> alpha = p(23);
  AutoDerivative<double> beta = p(24);
  AutoDerivative<double> kappa = p(25);
  AutoDerivative<double> s = p(26);
  xdelta = dx + s * X     + kappa * Y - beta * Z; 
  ydelta = dy - kappa * X + s * Y     + alpha * Z;
  zdelta = dz + beta * X  - alpha * Y + s * Z;
  xdelta += p(27) * X + p(28) * Y + p(29) * Z;
  ydelta += p(30) * X + p(31) * Y + p(32) * Z;
  zdelta += p(33) * X + p(34) * Y + p(35) * Z;
}

//-----------------------------------------------------------------------
/// Calculate the delta for the image coordinate. This comes from the
/// RSM documentation (RSM TRE Specificatino for NITF 2.1, page 96).
//-----------------------------------------------------------------------

void RsmAdjustableParameterA::delta_ls
(double x, double y, double z, double& lndelta, double& smpdelta) const
{
  const blitz::Array<double, 1>& p = full_parameter_.value();
  lndelta = p(0) + p(1) * x + p(2) * y + p(3) * z
    + p(4) * x * x + p(5) * x * y + p(6) * x * z
    + p(7) * y * y + p(8) * y * z + p(9) * z * z;
  smpdelta = p(10) + p(11) * x + p(12) * y + p(13) * z
    + p(14) * x * x + p(15) * x * y + p(16) * x * z
    + p(17) * y * y + p(18) * y * z + p(19) * z * z;
}

void RsmAdjustableParameterA::delta_ls
(double x, double y, double z, AutoDerivative<double>& lndelta,
 AutoDerivative<double>& smpdelta) const
{
  const ArrayAd<double, 1>& p = full_parameter_;
  lndelta = p(0) + p(1) * x + p(2) * y + p(3) * z
    + p(4) * x * x + p(5) * x * y + p(6) * x * z
    + p(7) * y * y + p(8) * y * z + p(9) * z * z;
  smpdelta = p(10) + p(11) * x + p(12) * y + p(13) * z
    + p(14) * x * x + p(15) * x * y + p(16) * x * z
    + p(17) * y * y + p(18) * y * z + p(19) * z * z;
}


// See base class for description.
void RsmAdjustableParameterA::adjustment
(const GroundCoordinate& Gc, boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
 double& Lndelta, double& Smpdelta) const
{
  double x, y, z;
  coordinate_converter()->convert_to_coordinate(Gc, x, y, z);
  double xdelta, ydelta, zdelta;
  delta_x(x, y, z, xdelta, ydelta, zdelta);
  delta_ls(x, y, z, Lndelta, Smpdelta);
  Gc_adjusted = coordinate_converter()->convert_from_coordinate(x + xdelta,
		      y + ydelta, z + zdelta);
}

// See base class for description.

void RsmAdjustableParameterA::adjustment_with_derivative
(const GroundCoordinate& Gc, ArrayAd<double, 1>& Cf_adjusted,
 AutoDerivative<double>& Lndelta, AutoDerivative<double>& Smpdelta) const
{
  double x, y, z;
  coordinate_converter()->convert_to_coordinate(Gc, x, y, z);
  AutoDerivative<double> xdelta, ydelta, zdelta;
  delta_x(x, y, z, xdelta, ydelta, zdelta);
  delta_ls(x, y, z, Lndelta, Smpdelta);
  Cf_adjusted.reference(coordinate_converter()->convert_to_cf(x + xdelta, y + ydelta, z + zdelta));
}

// See base class for description.
void RsmAdjustableParameterA::naif_code(int Naif_code)
{
  boost::shared_ptr<LocalRcParameter> p = cconv->parameter();
  if(Naif_code == Ecr::EARTH_NAIF_CODE)
    p->cf_prototype = boost::make_shared<Ecr>();
  else
    p->cf_prototype = boost::make_shared<PlanetFixed>(Naif_code);
}
