#include "orbit_des.h"
#include "ecr.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PosCsephb::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(PosCsephb);
  ar & GEOCAL_NVP_(min_time) & GEOCAL_NVP_(tstep)
    & GEOCAL_NVP_(is_cf) & GEOCAL_NVP(pos);
}

GEOCAL_IMPLEMENT(PosCsephb);
#endif

//-----------------------------------------------------------------------
/// Constructor. We sample the position of the given Orbit at fixed
/// spaces times. This version goes from the min_time() of the Orbit,
/// up to max_time() (or more accurately, the largest time min_time()
/// + i * Tstep that is <= max_time()).
//-----------------------------------------------------------------------

PosCsephb::PosCsephb(Orbit& Orb, double Tstep)
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
/// + i * Tstep that is <= Max_time).
//-----------------------------------------------------------------------

PosCsephb::PosCsephb
(Orbit& Orb, const Time& Min_time, const Time& Max_time, double Tstep)
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
     << "  Min_time:   " << min_time_ << "\n"
     << "  Tstep:      " << tstep_ << "\n"
     << "  Is_cf:      " << is_cf_ << "\n"
     << "  Number pos: " << pos.size() << "\n";
}

