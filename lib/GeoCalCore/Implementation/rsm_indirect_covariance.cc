#include "rsm_indirect_covariance.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmIndirectCovariance::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmIndirectCovariance);
  ar & GEOCAL_NVP_(image_identifier)
    & GEOCAL_NVP_(rsm_support_data_edition)
    & GEOCAL_NVP_(triangulation_id);
}

GEOCAL_IMPLEMENT(RsmIndirectCovariance);
#endif

static boost::format f("%|1$-80s|%|2$-40s|%|3$-40s|");
static boost::format numint("%|1$02d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");

//-----------------------------------------------------------------------
/// Write the part of the TRE string for the image identification,
/// RSM support data edition, and triangulation_id.
//-----------------------------------------------------------------------

std::string RsmIndirectCovariance::base_tre_string() const
{
 return str_check_size(f % image_identifier_
		       % rsm_support_data_edition_
		       % triangulation_id_, 80+40+40);
}

void RsmIndirectCovariance::base_read_tre_string(std::istream& In)
{
  image_identifier_ = read_size<std::string>(In, 80);
  rsm_support_data_edition_ = read_size<std::string>(In, 40);
  triangulation_id_ = read_size<std::string>(In, 40);
  boost::trim(image_identifier_);
  boost::trim(rsm_support_data_edition_);
  boost::trim(triangulation_id_);
}
