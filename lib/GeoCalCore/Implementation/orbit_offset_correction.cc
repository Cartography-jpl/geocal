#include "orbit_offset_correction.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitOffsetCorrection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(orb_uncorr)
    & GEOCAL_NVP(att_corr)
    & GEOCAL_NVP(pos_corr)
    & GEOCAL_NVP_(outside_is_error)
    & GEOCAL_NVP_(fit_position)
    & GEOCAL_NVP_(fit_yaw)
    & GEOCAL_NVP_(fit_pitch)
    & GEOCAL_NVP_(fit_roll);
}

GEOCAL_IMPLEMENT(OrbitOffsetCorrection);
#endif

//-----------------------------------------------------------------------
/// Constructor. This has no time points for doing corrections, but
/// you can add those using insert_time_point.
//-----------------------------------------------------------------------

OrbitOffsetCorrection::OrbitOffsetCorrection(
const boost::shared_ptr<Orbit> Orb_uncorr,
bool Outside_is_error,
bool Fit_position,
bool Fit_yaw,
bool Fit_pitch,
bool Fit_roll)
: Orbit(Orb_uncorr->min_time(), Orb_uncorr->max_time()),
  orb_uncorr(Orb_uncorr),
  outside_is_error_(Outside_is_error),
  fit_position_(Fit_position),
  fit_yaw_(Fit_yaw),
  fit_pitch_(Fit_pitch),
  fit_roll_(Fit_roll)
{
  pos_corr[0] = 0;
  pos_corr[1] = 0;
  pos_corr[2] = 0;
}

//-----------------------------------------------------------------------
/// Return list of time points that we have corrections for.
//-----------------------------------------------------------------------

std::vector<Time> OrbitOffsetCorrection::time_point() const
{
  std::pair<Time, boost::math::quaternion<AutoDerivative<double> > > e;
  std::vector<Time> res;
  BOOST_FOREACH(e, att_corr)
    res.push_back(e.first);
  return res;
}

// See base class for description
boost::shared_ptr<OrbitData> OrbitOffsetCorrection::orbit_data(Time T) const
{
}

// See base class for description
boost::shared_ptr<OrbitData> 
OrbitOffsetCorrection::orbit_data(const TimeWithDerivative& T) const
{
}

// See base class for description
ArrayAd<double, 1> OrbitOffsetCorrection::parameter_with_derivative() const
{
}

// See base class for description
void OrbitOffsetCorrection::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
}

// See base class for description
std::vector<std::string> OrbitOffsetCorrection::parameter_name() const
{
}

// See base class for description
blitz::Array<bool, 1> OrbitOffsetCorrection::parameter_mask() const
{
}

// See base class for description
void OrbitOffsetCorrection::print(std::ostream& Os) const
{
}

