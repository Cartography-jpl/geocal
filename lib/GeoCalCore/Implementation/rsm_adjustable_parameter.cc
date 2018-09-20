#include "rsm_adjustable_parameter.h"
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
void RsmAdjustableParameter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(RsmAdjustableParameter, WithParameter);
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

static boost::format f("%|1$-80s|%|2$-40s|%|3$-40s|");
static boost::format numint("%|1$02d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");

//-----------------------------------------------------------------------
/// Write the part of the TRE string for the image identification,
/// RSM support data edition, and triangulation_id.
//-----------------------------------------------------------------------

std::string RsmAdjustableParameter::base_tre_string() const
{
 return str_check_size(f % image_identifier_
		       % rsm_suport_data_edition_
		       % triangulation_id_, 80+40+40);
}

void RsmAdjustableParameter::base_read_tre_string(std::istream& In)
{
  image_identifier_ = read_size<std::string>(In, 80);
  rsm_suport_data_edition_ = read_size<std::string>(In, 40);
  triangulation_id_ = read_size<std::string>(In, 40);
  boost::trim(image_identifier_);
  boost::trim(rsm_suport_data_edition_);
  boost::trim(triangulation_id_);
}

