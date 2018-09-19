#include "rsm_adjustable_parameter.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmAdjustableParameter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmAdjustableParameter);
  ar & GEOCAL_NVP_(image_identifier)
    & GEOCAL_NVP_(rsm_suport_data_edition)
    & GEOCAL_NVP_(triangulation_id);
}

GEOCAL_IMPLEMENT(RsmAdjustableParameter);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

static boost::format f("%|1$-80s|%|2$-40s|%|3$-40s|%|4$02d|");

// Short term, fake out tre_string with whatever was passed to
  // read_tre_string.
static std::string temp;

std::string RsmAdjustableParameter::tre_string() const
{
  std::string s = str_check_size(f % image_identifier_
				 % rsm_suport_data_edition_
				 % triangulation_id_ % 0, 80+40+40+2);
  //return s;
  return temp;
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

boost::shared_ptr<RsmAdjustableParameter>
RsmAdjustableParameter::read_tre_string(const std::string& Tre_in)
{
  temp = Tre_in;
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmAdjustableParameter> res(new RsmAdjustableParameter);
  res->image_identifier_ = read_size<std::string>(in, 80);
  res->rsm_suport_data_edition_ = read_size<std::string>(in, 40);
  res->triangulation_id_ = read_size<std::string>(in, 40);
  boost::trim(res->image_identifier_);
  boost::trim(res->rsm_suport_data_edition_);
  boost::trim(res->triangulation_id_);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmAdjustableParameter::print(std::ostream& Os) const
{
  Os << "RsmAdjustableParameter:\n"
     << "  Image Identifier: " << image_identifier_ << "\n"
     << "  RSM Edition:      " << rsm_suport_data_edition_ << "\n"
     << "  Triangulation ID: " << triangulation_id_ << "\n";
}
