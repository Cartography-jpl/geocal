#include "orbit_offset_correction.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
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
  blitz::Array<AutoDerivative<double>, 1> res(3 + 3 * att_corr.size());
  res(0) = pos_corr[0];
  res(1) = pos_corr[1];
  res(2) = pos_corr[2];
  std::pair<Time, boost::math::quaternion<AutoDerivative<double> > > e;
  int i = 3;
  BOOST_FOREACH(e, att_corr) {
    quat_to_ypr(e.second, res(i + 0), res(i + 1), res(i + 2));
    i += 3;
  }
  return res;
}

// See base class for description
void OrbitOffsetCorrection::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  pos_corr[0] = Parm(0);
  pos_corr[1] = Parm(1);
  pos_corr[2] = Parm(2);
  int i = 3;
  std::map<Time, boost::math::quaternion<AutoDerivative<double> > >::iterator e;
  for(e = att_corr.begin(); e != att_corr.end(); ++e, i += 3)
    e->second = quat_rot("xyz", Parm(i + 1), Parm(i + 2), Parm(i + 0));
}

// See base class for description
std::vector<std::string> OrbitOffsetCorrection::parameter_name() const
{
  std::vector<std::string> res;
  res.push_back("Position X Offset");
  res.push_back("Position Y Offset");
  res.push_back("Position Z Offset");
  std::pair<Time, boost::math::quaternion<AutoDerivative<double> > > e;
  BOOST_FOREACH(e, att_corr) {
    res.push_back("Yaw correction time " + e.first.to_string());
    res.push_back("Pitch correction time " + e.first.to_string());
    res.push_back("Yaw correction time " + e.first.to_string());
  }
  return res;
}

// See base class for description
blitz::Array<bool, 1> OrbitOffsetCorrection::parameter_mask() const
{
  blitz::Array<bool, 1> res(3 + 3 * att_corr.size());
  res(0) = fit_position();
  res(1) = fit_position();
  res(2) = fit_position();
  for(int i = 0; i < (int) att_corr.size(); ++i) {
    res(i * 3 + 0) = fit_yaw();
    res(i * 3 + 1) = fit_pitch();
    res(i * 3 + 2) = fit_roll();
  }
  return res;
}

// See base class for description
void OrbitOffsetCorrection::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitOffsetCorrection\n"
     << "  Underlying orbit:\n";
  opad << *orbit_uncorrected() << "\n";
  opad.strict_sync();
  Os << "  Fit position: " << (fit_position() ? "True" : "False") << "\n"
     << "  Fit yaw:      " << (fit_yaw() ? "True" : "False") << "\n"
     << "  Fit pitch:    " << (fit_pitch() ? "True" : "False") << "\n"
     << "  Fit roll:     " << (fit_roll() ? "True" : "False") << "\n";
  Os << "  Parameter:\n";
  std::vector<std::string> pname = parameter_name();
  blitz::Array<double, 1> parm = parameter();
  const static int sv_num_width = 17;
  for(int i = 0; i < parm.rows(); ++i)
    Os << "     " << std::setprecision(sv_num_width - 7) 
       << std::setw(sv_num_width) << parm(i) << "  "
       << pname[i] << "\n";
}

