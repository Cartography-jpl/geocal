#include "orbit_des.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "eci.h"
#include "geocal_serialize_support.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PosCsephb::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(PosCsephb);
  ar & GEOCAL_NVP_(min_time) & GEOCAL_NVP_(tstep)
    & GEOCAL_NVP_(is_cf) & GEOCAL_NVP(pos);
}

template<class Archive>
void OrbitDes::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(pos);
}

GEOCAL_IMPLEMENT(PosCsephb);
GEOCAL_IMPLEMENT(OrbitDes);
#endif

//-----------------------------------------------------------------------
/// Constructor. We sample the position of the given Orbit at fixed
/// spaces times. This version goes from the min_time() of the Orbit,
/// up to max_time() (or more accurately, the largest time min_time()
/// + i * Tstep that is <= max_time()).
//-----------------------------------------------------------------------

PosCsephb::PosCsephb(const Orbit& Orb, double Tstep)
  : min_time_(Orb.min_time()), tstep_(Tstep)
{
  is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  blitz::Array<double, 1> p(3);
  for(Time t = min_time_; t <= Orb.max_time(); t += tstep_) {
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      p = cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      p = ci->position[0], ci->position[1], ci->position[2];
    }
    pos.push_back(p.copy());
  }
}

//-----------------------------------------------------------------------
/// Constructor. We sample the position of the given Orbit at fixed
/// spaces times. This version goes from the Min_time,
/// up to Max_time (or more accurately, the largest time Min_time
/// + i * Tstep that is <= Max_time).w
//-----------------------------------------------------------------------

PosCsephb::PosCsephb
(const Orbit& Orb, const Time& Min_time, const Time& Max_time, double Tstep)
  : min_time_(Min_time), tstep_(Tstep)
{
  is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  blitz::Array<double, 1> p(3);
  for(Time t = min_time_; t <= Max_time; t += tstep_) {
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      p = cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      p = ci->position[0], ci->position[1], ci->position[2];
    }
    pos.push_back(p.copy());
  }
}

void PosCsephb::print(std::ostream& Os) const
{
  Os << "PosCsephb\n"
     << "  Min time: " << min_time_ << "\n"
     << "  Max time: " << max_time() << "\n"
     << "  Tstep:    " << tstep_ << "\n"
     << "  Is_cf:    " << is_cf_ << "\n";
}

//-----------------------------------------------------------------------
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<double, 1> PosCsephb::pos_vel(const Time& T) const
{
  range_check_inclusive(T, min_time_, max_time());
  // Just linear interpolation for now. We'll do more complicated
  // interpolation shortly.
  int i = (int) floor((T - min_time_) / tstep_);
  if(i >= (int) pos.size())
    --i;
  double f = (T - min_time_) / tstep_ - i;
  blitz::Array<double, 1> res(6);
  res(blitz::Range(0,2)) = f * pos[i+1] + (1 - f) * pos[i];
  res(blitz::Range(3,5)) = (pos[i+1] - pos[i]) / tstep_;
  return res;
}

//-----------------------------------------------------------------------
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<AutoDerivative<double>, 1> PosCsephb::pos_vel
(const TimeWithDerivative& T) const
{
  range_check_inclusive(T.value(), min_time_, max_time());
  // Just linear interpolation for now. We'll do more complicated
  // interpolation shortly.
  int i = (int) floor((T.value() - min_time_) / tstep_);
  if(i >= (int) pos.size())
    --i;
  AutoDerivative<double> f = (T - min_time_) / tstep_ - i;
  blitz::Array<AutoDerivative<double>, 1> res(6);
  res(blitz::Range(0,2)) = f * pos[i+1] + (1 - f) * pos[i];
  res(blitz::Range(3,5)) = (pos[i+1] - pos[i]) / tstep_;
  return res;
}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

OrbitDes::OrbitDes(const boost::shared_ptr<PosCsephb>& Pos)
  : Orbit(Pos->min_time(), Pos->max_time()),
    pos_(Pos)
{
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data(Time T) const
{
  // Temporary
  boost::math::quaternion<double> q(0,0,0,1);
  blitz::Array<double, 1> posvel = pos_->pos_vel(T);
  boost::array<double, 3> vel;
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(pos_->is_cf())
    return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Ecr>(posvel(0), posvel(1), posvel(2)), vel, q);
  return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Eci>(posvel(0), posvel(1), posvel(2)), vel, q);
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data
(const TimeWithDerivative& T) const
{
  // Temporary
  boost::math::quaternion<AutoDerivative<double> > q(0,0,0,1);
  blitz::Array<AutoDerivative<double>, 1> posvel = pos_->pos_vel(T);
  boost::array<AutoDerivative<double>, 3> pos;
  boost::array<AutoDerivative<double>, 3> vel;
  pos[0] = posvel(0);
  pos[1] = posvel(1);
  pos[2] = posvel(2);
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(pos_->is_cf())
    return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Ecr>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value()),
	   pos, vel, q);
  return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Eci>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value()),
	   pos, vel, q);
}

void OrbitDes::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitDes:\n"
     << "  pos_csephb:\n";
  opad << *pos_csephb() << "\n";
  opad.strict_sync();
}
