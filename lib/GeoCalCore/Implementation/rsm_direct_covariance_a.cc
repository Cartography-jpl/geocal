#include "rsm_direct_covariance_a.h"
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
void RsmDirectCovarianceA::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmDirectCovariance)
    & GEOCAL_NVP(cconv) & GEOCAL_NVP(parm_index)
    & GEOCAL_NVP(iidi) & GEOCAL_NVP(npari)
    & GEOCAL_NVP(cov);
}

GEOCAL_IMPLEMENT(RsmDirectCovarianceA);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

static boost::format numint("%|1$02d|");
static boost::format numint3("%|1$03d|");
static boost::format numint5("%|1$05d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");
static boost::format str("%|1$-80s|");

std::string RsmDirectCovarianceA::tre_string() const
{
  std::string res = base_tre_string();
  res += str_check_size(numint % num_parameter(), 2);
  res += str_check_size(numint3 % number_image(), 3);
  res += str_check_size(numint5 % total_num_parameter(), 5);
  for(int i = 0; i < number_image(); ++i) {
    res += str_check_size(str % image_identifier_i(i), 80);
    res += str_check_size(numint % number_parameter_i(i), 2);
  }
  const LocalRcConverter* gconv = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(!gconv)
    throw Exception("Writing a RSMDCA TRE only supports LocalRcConverter. This is by definition of how the RSM adjustable parameters work");
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
  for(int i = 0; i < cov.rows(); ++i)
    for(int j = i; j < cov.cols(); ++j)
      res += str_check_size(num % cov(i, j), 21);
  return res;
}

//-----------------------------------------------------------------------
/// Create a RsmDirectCovarianceA. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id, and
/// can optionally activate a set of the parameters for the correction.
//-----------------------------------------------------------------------

RsmDirectCovarianceA::RsmDirectCovarianceA
(const ImageGroundConnection& Igc,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id,
 bool Activate_image_correction,
 bool Activate_ground_rotation,
 bool Activate_ground_correction
 )
  : RsmDirectCovariance(Rsm_id.image_identifier(),
			   Rsm_id.rsm_suport_data_edition(),
			   Triangulation_id),
    parm_index(36)
{
  for(int i = 0; i < parm_index.rows(); ++i) {
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

boost::shared_ptr<RsmDirectCovarianceA>
RsmDirectCovarianceA::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmDirectCovarianceA> res(new RsmDirectCovarianceA);
  res->base_read_tre_string(in);
  int nparm = read_size<int>(in, 2);
  int nimge = read_size<int>(in, 3);
  int npart = read_size<int>(in, 5);
  for(int i = 0; i < nimge; ++i) {
    std::string iidi = read_size<std::string>(in, 80);
    int nparmi = read_size<int>(in, 2);
    boost::trim(iidi);
    res->add_image(iidi, nparmi);
  }
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
  res->cov.resize(npart, npart);
  // Data is row order, upper triangular. We fill in the lower
  // triangle at the same time.
  for(int i = 0; i < res->cov.rows(); ++i)
    for(int j = i; j < res->cov.cols(); ++j) {
      res->cov(i, j) = read_size<double>(in, 21);
      res->cov(j, i) = res->cov(i, j);
    }
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmDirectCovarianceA::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmDirectCovarianceA:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_suport_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n"
     << "  Local Coordinate System:\n";
  opad << *cconv << "\n";
  opad.strict_sync();
  Os << "  Number Image: " << number_image() << "\n";
  Os << "  Image:\n";
  for(int i = 0; i < number_image(); ++i)
    Os << "    Image Identifier " << i << ": " << image_identifier_i(i) << "\n"
       << "    Number parameter " << i << ": " << number_parameter_i(i) << "\n";
  Os << "  Parameter this image:\n";
  for(auto p : parameter_name())
    Os << "    " << p << "\n";
  Os << "  Covariance:\n";
  opad << cov << "\n";
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// Name of the full parameters
//-----------------------------------------------------------------------

const std::vector<std::string>&
RsmDirectCovarianceA::full_parameter_name()
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
 
void RsmDirectCovarianceA::activate_image_correction()
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
 
void RsmDirectCovarianceA::activate_ground_rotation()
{
  int ind = (int) num_parameter();
  for(int i = 20; i < 27; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
}

//-----------------------------------------------------------------------
/// Any set of parameters can be activated, in any desired order. But
/// we tend to activate them in groups. This function turns on all of
/// ground correction parameters.
//-----------------------------------------------------------------------
 
void RsmDirectCovarianceA::activate_ground_correction()
{
  int ind = (int) num_parameter();
  for(int i = 20; i < 23; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
  for(int i = 28; i < 36; ++i)
    if(parm_index(i) < 1)
      parm_index(i) = ++ind;
}
 
int RsmDirectCovarianceA::num_parameter() const
{
  int nparm = 0;
  for(int i = 0; i < parm_index.rows(); ++i)
    if(parm_index(i) > 0)
      ++nparm;
  return nparm;
}

int RsmDirectCovarianceA::total_num_parameter() const
{
  int nparm = 0;
  for(auto np : npari)
    nparm += np;
  return nparm;
}

std::vector<std::string> RsmDirectCovarianceA::parameter_name() const
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

// See base class for description.
void RsmDirectCovarianceA::naif_code(int Naif_code)
{
  boost::shared_ptr<LocalRcParameter> p = cconv->parameter();
  if(Naif_code == Ecr::EARTH_NAIF_CODE)
    p->cf_prototype = boost::make_shared<Ecr>();
  else
    p->cf_prototype = boost::make_shared<PlanetFixed>(Naif_code);
}
