#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmId::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmId);
  ar & GEOCAL_NVP_(image_identifier)
    & GEOCAL_NVP_(rsm_suport_data_edition);
}

GEOCAL_IMPLEMENT(RsmId);
#endif

static boost::format f("%|1$-80s|%|2$-40s|");

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

std::string RsmId::tre_string() const
{
  return str_check_size(f % image_identifier_ % rsm_suport_data_edition_,
			120);
}

//-----------------------------------------------------------------------
/// Read a TRE string. 
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmId>
RsmId::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmId> res(new RsmId);
  res->image_identifier_ = read_size<std::string>(in, 80);
  res->rsm_suport_data_edition_ = read_size<std::string>(in, 40);
  boost::trim(res->image_identifier_);
  boost::trim(res->rsm_suport_data_edition_);
  check_end_of_stream(in);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmId::print(std::ostream& Os) const
{
  Os << "RsmId:\n"
     << "  Image Identifier:   " << image_identifier_ << "\n"
     << "  RSM Edition:        " << rsm_suport_data_edition_ << "\n";
}
