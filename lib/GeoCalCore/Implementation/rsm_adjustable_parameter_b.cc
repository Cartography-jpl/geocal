#include "rsm_adjustable_parameter_b.h"
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
void RsmAdjustableParameterB::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmAdjustableParameter);
}

GEOCAL_IMPLEMENT(RsmAdjustableParameterB);
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

std::string RsmAdjustableParameterB::tre_string() const
{
  std::string res = base_tre_string();
  const LocalRcConverter* gconv = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(!gconv)
    throw Exception("Writing a RSMAPB TRE only supports LocalRcConverter. This is by definition of how the RSM adjustable parameters work");
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num % gconv->parameter()->cf_offset[i], 21);
  // This is column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      res += str_check_size(num % gconv->parameter()->cf_to_rc[i][j], 21);
  return res;
}

//-----------------------------------------------------------------------
/// Create a RsmAdjustableParameterB. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id, and
/// can optionally activate a set of the parameters for the correction.
//-----------------------------------------------------------------------

RsmAdjustableParameterB::RsmAdjustableParameterB
(const ImageGroundConnection& Igc,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id
 )
  : RsmAdjustableParameter(Rsm_id.image_identifier(),
			   Rsm_id.rsm_suport_data_edition(),
			   Triangulation_id)
{
  cconv = boost::make_shared<LocalRcConverter>(boost::make_shared<LocalRcParameter>(Igc));
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

boost::shared_ptr<RsmAdjustableParameterB>
RsmAdjustableParameterB::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmAdjustableParameterB> res(new RsmAdjustableParameterB);
  res->base_read_tre_string(in);
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>();
  lp->cf_prototype = boost::make_shared<Ecr>(0,0,0);
  for(int i = 0; i < 3; ++i)
    lp->cf_offset[i] = read_size<double>(in, 21);
  // Column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      lp->cf_to_rc[i][j] = read_size<double>(in, 21);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmAdjustableParameterB::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmAdjustableParameterB:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_suport_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n"
     << "  Local Coordinate System:\n";
  opad << *cconv << "\n";
  opad.strict_sync();
}

// See base class for description.
bool RsmAdjustableParameterB::has_ground_coordinate_parameter() const
{
  return false;
}

// See base class for description
ArrayAd<double, 1> RsmAdjustableParameterB::parameter_with_derivative() const
{
  ArrayAd<double, 1> res;
  return res;
}

// See base class for description
void RsmAdjustableParameterB::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
}

std::vector<std::string> RsmAdjustableParameterB::parameter_name() const
{
  std::vector<std::string> res;
  return res;
}

// See base class for description.
void RsmAdjustableParameterB::adjustment
(const GroundCoordinate& Gc, boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
 double& Lndelta, double& Smpdelta) const
{
}

// See base class for description.

void RsmAdjustableParameterB::adjustment_with_derivative
(const GroundCoordinate& Gc, ArrayAd<double, 1>& Cf_adjusted,
 AutoDerivative<double>& Lndelta, AutoDerivative<double>& Smpdelta) const
{
}

// See base class for description.
void RsmAdjustableParameterB::naif_code(int Naif_code)
{
  boost::shared_ptr<LocalRcParameter> p = cconv->parameter();
  if(Naif_code == Ecr::EARTH_NAIF_CODE)
    p->cf_prototype = boost::make_shared<Ecr>();
  else
    p->cf_prototype = boost::make_shared<PlanetFixed>(Naif_code);
}
