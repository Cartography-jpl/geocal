#include "rsm_adjustable_parameter_a.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "local_rectangular_coordinate.h"
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
    & GEOCAL_NVP(parm_index)
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
     << "  RSM Edition:      " << rsm_suport_data_edition() << "\n"
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
