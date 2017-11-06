#include "rsm_direct_covariance.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmDirectCovariance::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmDirectCovariance);
}

GEOCAL_IMPLEMENT(RsmDirectCovariance);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

std::string RsmDirectCovariance::tre_string() const
{
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

boost::shared_ptr<RsmDirectCovariance>
RsmDirectCovariance::read_tre_string(const std::string& Tre_in)
{
}