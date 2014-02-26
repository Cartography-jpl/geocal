#include "geocal_time.h"
#ifdef HAVE_SPICE
#include "spice_helper.h"
extern "C" {
#include "SpiceUsr.h"
}
#endif
#ifdef HAVE_SDP
#include "sdp_helper.h"
#endif
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/regex.hpp>
using namespace GeoCal;

const Time Time::min_valid_time = Time::time_pgs(-1009843218.0);
const Time Time::max_valid_time = Time::time_pgs(252676368006.0);

/****************************************************************//**
  Unix version
*******************************************************************/
class UnixToolkitTimeInterface : public ToolkitTimeInterface {
public:
  virtual ~UnixToolkitTimeInterface() {}
  virtual Time parse_time(const std::string Time_string) const
  {
    using namespace boost::posix_time;
    ptime t = time_from_string(Time_string);
    tm t_tm = to_tm(t);
    return Time::time_unix(mktime(&t_tm));
  }
  virtual std::string to_string(const Time& T) const
  {
    using namespace boost::posix_time;
    ptime t = from_time_t(T.unix_time());
    return to_iso_extended_string(t);
  }
};

//-----------------------------------------------------------------------
/// Return time from given SPICE ET time.
//-----------------------------------------------------------------------

Time Time::time_et(double et) 
{
#ifdef HAVE_SPICE
  return SpiceHelper::et_to_geocal(et);
#else
  throw Exception("Need to have SPICE toolkit to convert from SPICE ET time");
#endif
}

//-----------------------------------------------------------------------
/// Return time from ACS time.
///
/// ACS time is an odd time system. It is measured in UTC
/// seconds from a particular epoch. The choice of UTC seconds means
/// that this cannot correctly handle times that occur during a
/// leapsecond, by definition the UTC time before and after a
/// leapsecond is the same. The epoch is noon January, 1 2000 in UTC. 
/// Note that this is 64.184 seconds different from terrestrial time J2000.
//-----------------------------------------------------------------------

Time Time::time_acs(double acs_time)
{
#ifdef HAVE_SPICE
  SpiceHelper::spice_setup();
  double delta;
  deltet_c(acs_time, "UTC", &delta);
  return Time::time_et(acs_time + delta);
#else
  // Much slower implementation
  using namespace boost::posix_time;
  using namespace boost::gregorian;
  // We need to add acs_time to January 1, 2000 but *without*
  // including leapseconds. We take advantage of the fact that boost
  // ptime does this.
  
  int acs_time_int = (int) floor(acs_time);
  double acs_time_frac = acs_time - acs_time_int;
  ptime epoch(date(2000, Jan, 1), hours(12));
  return Time::parse_time(to_iso_extended_string(epoch + seconds(acs_time_int)))
			  + acs_time_frac;
#endif
}

//-----------------------------------------------------------------------
/// Give ACS time.
//-----------------------------------------------------------------------

double Time::acs() const
{
#ifdef HAVE_SPICE
  SpiceHelper::spice_setup();
  double t = et();
  double delta;
  deltet_c(t, "ET", &delta);
  return t - delta;
#else
  // Much slower implementation
  using namespace boost::posix_time;
  using namespace boost::gregorian;

  // Take advantage of the fact that difference in ptime does not
  // include leapseconds.
  std::string tstring = 
    boost::regex_replace(to_string(), 
     boost::regex("(\\d{4}-\\d{2}-\\d{2})T(\\d{2}:\\d{2}:\\d{2}(\\.\\d+)?)Z"), 
			 "\\1 \\2");
  ptime t = time_from_string(tstring);
  ptime epoch(date(2000, Jan, 1), hours(12));
  time_duration td = t - epoch;
  return td.total_nanoseconds() * 1e-9;
#endif
}

//-----------------------------------------------------------------------
/// Give time as SPICE ET time.
//-----------------------------------------------------------------------

double Time::et() const 
{
#ifdef HAVE_SPICE
  return SpiceHelper::geocal_to_et(*this);
#else
  throw Exception("Need to have SPICE toolkit to convert to SPICE ET time");
#endif
}

#ifdef HAVE_SPICE
ToolkitTimeInterface* Time::toolkit_time_interface = 
  new SpiceToolkitTimeInterface();
#else
  #ifdef HAVE_SDP
ToolkitTimeInterface* Time::toolkit_time_interface = 
  new SdpToolkitTimeInterface();
  #else
ToolkitTimeInterface* Time::toolkit_time_interface = 
  new UnixToolkitTimeInterface();
  #endif
#endif

