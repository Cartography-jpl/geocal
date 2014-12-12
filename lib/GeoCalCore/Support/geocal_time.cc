#include "geocal_internal_config.h"
#include "geocal_time.h"
#include "geocal_serialize_support.h"
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
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Time::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Time);
  ar & GEOCAL_NVP_(time_pgs);
}

GEOCAL_IMPLEMENT(Time);
#endif

const Time Time::min_valid_time = Time::time_pgs(-1009843218.0);
const Time Time::max_valid_time = Time::time_pgs(252676368006.0);

/****************************************************************//**
  Unix version
*******************************************************************/
class UnixToolkitTimeInterface : public ToolkitTimeInterface {
public:
  UnixToolkitTimeInterface()
  {
    // This is a hardcoded leapseconds file, used in converting to and
    // from CCSDS strings. This is only going forward from 1993, I
    // suppose we could extend this backwards if we ever need that
    // (but at that point we should really just be using SDP or SPICE)
    leapsecond_table[0.0] = 0;
    double one_yr = 365 * 24 * 60 * 60;
    // June 30, 1993
    leapsecond_table[0.5 * one_yr] = 0.0;
    // June 30, 1994
    leapsecond_table[1.5 * one_yr] = 1.0;
    // Dec 31, 1995
    leapsecond_table[3.0 * one_yr] = 2.0;
    // June 30, 1997
    leapsecond_table[4.5 * one_yr] = 3.0;
    // Dec 31, 1998
    leapsecond_table[6.0 * one_yr] = 4.0;
    // Dec 31, 2005
    leapsecond_table[13.0 * one_yr] = 5.0;
    // Dec 31, 2008
    leapsecond_table[16.0 * one_yr] = 6.0;
    // June 30, 2012
    leapsecond_table[19.5 * one_yr] = 7.0;
    // Far in the future, as an easy endpoint
    leapsecond_table[1e6 * one_yr] = 8.0;
    typedef std::pair<double, int> ptype;
    BOOST_FOREACH(ptype i, leapsecond_table) {
      unleapsecond_table[i.first + i.second] = -i.second;
    }

    // Unix epoch in std::time_t.
    tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 1;
    t.tm_year = 1970;
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_isdst = 0;
    unix_ep = mktime(&t);
  }
  virtual ~UnixToolkitTimeInterface() {}
  virtual Time parse_time(const std::string Time_string) const
  {
    using namespace boost::posix_time;
    // Only handle CCSDS time format.
    boost::smatch m;
    if(!boost::regex_match(Time_string, m,
     boost::regex("(\\d{4})-(\\d{2})-(\\d{2})T(\\d{2}):(\\d{2}):(\\d{2}(\\.\\d+)?)Z"))) {
      Exception e;
      e << "The UnixToolkitTimeInterface is limited to only parsing CCSDS format time strings. The string \"" << Time_string << "\" does not match this format. If you need better parsing, use one of the other time toolkits (e.g, SPICE or SDP toolkit";
      throw e;
    }
    double sec_d = boost::lexical_cast<double>(m[6]);
    int sec_i = (int) floor(sec_d);
    double frac_sec = sec_d - sec_i;
    tm t;
    t.tm_sec = sec_i;
    t.tm_min = boost::lexical_cast<double>(m[5]);
    t.tm_hour = boost::lexical_cast<double>(m[4]);
    t.tm_mday = boost::lexical_cast<double>(m[3]);
    t.tm_mon = boost::lexical_cast<double>(m[2]);
    t.tm_year = boost::lexical_cast<double>(m[1]);
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_isdst = 0;
    std::time_t r = mktime(&t);
    Time res = Time::time_unix(r - unix_ep + frac_sec);
    if(res.pgs() < 0)
      throw Exception("We can't handle leapseconds before 1993. If you need earlier dates, install one of the other time toolkits");
    res += leapsecond_table.lower_bound(res.pgs())->second;
    return res;
  }
  virtual std::string to_string(const Time& T) const
  {
    if(T.pgs() < 0)
      throw Exception("We can't handle leapseconds before 1993. If you need earlier dates, install one of the other time toolkits");
    using namespace boost::posix_time;
    Time tadjust = T;
    tadjust += unleapsecond_table.lower_bound(tadjust.pgs())->second;
    // Doesn't handle fractional seconds, so we add that in.
    double frac_sec = tadjust.unix_time_double() - 
      floor(tadjust.unix_time_double());
    ptime t = from_time_t(tadjust.unix_time() - frac_sec);
    std::string res = to_iso_extended_string(t);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6) << frac_sec;
    boost::smatch m;
    if(boost::regex_search(ss.str(), m, boost::regex("(\\.\\d+)")))
      res += m[1];
    else
      res += ".000000";
    return  res + "Z";
  }
private:
  std::map<double, int> leapsecond_table;
  std::map<double, int> unleapsecond_table;
  // Unix epoch. This accounts for any handling of time zone information.
  std::time_t unix_ep;
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
// This is just used by the unit test, so we can always 
// test the default unix case.
ToolkitTimeInterface* Time::_unix_toolkit_time_interface =
  new UnixToolkitTimeInterface();
