#include "rsm_indirect_covariance_b.h"
#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmIndirectCovarianceB::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmIndirectCovariance)
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBParameterDesc);
}

GEOCAL_IMPLEMENT(RsmIndirectCovarianceB);
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
static boost::format numpow("%|1$1d|%|2$1d|%|3$1d|");

std::string RsmIndirectCovarianceB::tre_string() const
{
  std::string res = base_tre_string();
  res += rsm_b_parm_tre_string();
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

boost::shared_ptr<RsmIndirectCovarianceB>
RsmIndirectCovarianceB::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmIndirectCovarianceB> res(new RsmIndirectCovarianceB);
  res->base_read_tre_string(in);
  res->rsm_b_parm_read_tre_string(in);
  return res;
}

//-----------------------------------------------------------------------
/// Create a RsmIndirectCovarianceB. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id.
//-----------------------------------------------------------------------

RsmIndirectCovarianceB::RsmIndirectCovarianceB
(const ImageGroundConnection& Igc,
 double H_min, double H_max,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id)
  : RsmBParameterDesc(Igc, H_min, H_max),
    RsmIndirectCovariance(Rsm_id.image_identifier(),
			  Rsm_id.rsm_suport_data_edition(),
			  Triangulation_id)
{
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmIndirectCovarianceB::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmIndirectCovarianceB:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_suport_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n";
  RsmBParameterDesc::print(opad);
  opad.strict_sync();
}

