#ifndef GEOCAL_TIME_H
#define GEOCAL_TIME_H
#include "toolkit_time_interface.h"
#include "auto_derivative.h"
#include "printable.h"
#include <boost/operators.hpp>
#include <math.h>
#include <ctime>

namespace GeoCal {
/****************************************************************//**
  There are a few reasonable choices for expressing time information. 
  We could use TAI, GPS, the PGS toolkit. Each of these time system
  can be related to the other by a constant, since the only difference
  is the Epoch that time is measure against.

  Note that for accurate work we do *not* want to use something like
  Unix time, because this does not account for leapseconds (POSIX unix
  time is the number of seconds since January 1, 1970 *not* including
  leapseconds).

  Most code doesn't care what the underlying time representation is,
  we just need to be able to do functions such as comparing two times
  to determine what is later, adding or subtracting a given number of
  seconds to a time, or give the duration between two times.

  This class abstracts out the representation we use for time. We
  supply conversions to the specific time systems for use in the cases
  that a specific system is needed (e.g., calling a PGS toolkit
  routine).

  We also supply methods for converting to and from a string
  representation of the time. The supported formats of the string
  parsing depends on the underlying toolkit used, but all of them
  support CCSDS format (e.g., "1996-07-03T04:13:57.987654Z").

  If either SPICE or SDP is available, then that toolkit is the one
  used for the conversion. If both are available, we default to SPICE.
  In each case, you can change the default by updating the variable 
  Time::toolkit_time_interface.

  If we don't have either SPICE or SDP, then we default to using unix
  time (through the boost date_time library). This isn't ideal, but it
  is better than not supporting time at all. For a number of purposes
  the unix time is fine (e.g., indexing data in a orbit file).

  As an implementation detail, we use PGS time, which has an epoch of
  1993-01-01.

  Note there is a subtle difference between time used for Terrestrial
  uses (such as UTC) and for planetary use. There are two ways used
  for measuring time - International atomic time (TAI) and Barycentric
  Dynamic Time (TDB). The first is the time measured by an atomic clock
  on the earth vs. the second which measures time at the barycenter of
  the solar system. Due to relativistic effects, the two clocks vary
  by something close to a periodic function with a size of about 1 ms
  over the coarse of a year. We measure time durations (e.g., the
  difference between 2 times) in TAI. For most purposes, you can
  ignore the difference between the two systems.

  When calling SPICE routines, we internally convert to and from TAI
  time as needed. This is a bit less efficient than simply keeping
  everything in TDB time, but it gives a cleaner interface. We can
  revisit this if the computer time it takes to do the conversions
  start becoming prohibitive. 
*******************************************************************/

template<class T> class TimeBase {
public:
//-----------------------------------------------------------------------
/// Add given number of seconds to Time.
//-----------------------------------------------------------------------

  TimeBase<T>& operator+=(const T& Toff) {time_pgs_ += Toff; return *this;}

//-----------------------------------------------------------------------
/// Subtract given number of seconds to Time.
//-----------------------------------------------------------------------

  TimeBase<T>& operator-=(const T& Toff) {time_pgs_ -= Toff; return *this;}

//-----------------------------------------------------------------------
/// Give time in PGS toolkit time (epoch 1993-01-01).
//-----------------------------------------------------------------------
  
  T pgs() const {return time_pgs_;}

//-----------------------------------------------------------------------
/// Give time in GPS.
//-----------------------------------------------------------------------
  
  T gps() const {return time_pgs_ + 409881608.0;}

//-----------------------------------------------------------------------
/// Give time in j2000.
//-----------------------------------------------------------------------
  
  T j2000() const {return time_pgs_ - (220881605.0 - 64.1839272778);}

protected:
  T time_pgs_;
};

class Time : public TimeBase<double>, public Printable<Time>,
	     private boost::addable<Time, double>,
             private boost::subtractable<Time, double>,
             private boost::less_than_comparable<Time> 
{
public:
//-----------------------------------------------------------------------
/// Return time from given PGS toolkit time (epoch of 1993-01-01).
//-----------------------------------------------------------------------
  
  static Time time_pgs(double pgs) 
  {Time res; res.time_pgs_ = pgs; return res;}

//-----------------------------------------------------------------------
/// Return time from given J2000 time (epoch of 2000-01-01 12:00:00
/// TT). Note that TT is different than UTC noon by about 64.184 seconds
//-----------------------------------------------------------------------
  
  static Time time_j2000(double j2000) 
  {Time res; 
    // Constant here is from SPICE kernel, and is accurate to more
    // digits than the 64.184
    res.time_pgs_ = j2000 + 220881605.0 - 64.1839272778; 
    return res;}

//-----------------------------------------------------------------------
/// Return time from given GPS time (epoch of 1980-01-06).
//-----------------------------------------------------------------------
  
  static Time time_gps(double gps) {return time_pgs(gps - 409881608.0);}

  static Time time_et(double et);
  double et() const;

  static Time time_acs(double acs_time);
  double acs() const;

//-----------------------------------------------------------------------
/// Return time from given Unix time (epoch of 1970-01-01).
//-----------------------------------------------------------------------
  
  static Time time_unix(std::time_t unix_time) 
  {Time res; 
    res.time_pgs_ = (double)(unix_time - 725846400.0); return res;}


//-----------------------------------------------------------------------
/// Return time from given Unix time (epoch of 1970-01-01).
//-----------------------------------------------------------------------
  
  static Time time_unix(double unix_time) 
  {Time res; res.time_pgs_ = (unix_time - 725846400.0); return res;}

//-----------------------------------------------------------------------
/// Give time in unix time. Note that this is only accurate to the
/// nearest second.
//-----------------------------------------------------------------------

  std::time_t unix_time() const 
  { return ((std::time_t) (int) round(time_pgs_)) + 725846400; }
                                                    
//-----------------------------------------------------------------------
/// Give time in unix time, as a double.
//-----------------------------------------------------------------------

  double unix_time_double() const 
  { return time_pgs_ + 725846400; }

//-----------------------------------------------------------------------
/// Minimum valid time. This is 1961-01-01T00:00:00Z.
//-----------------------------------------------------------------------
  
  static const Time min_valid_time;

//-----------------------------------------------------------------------
/// Maximum valid time. This is 9999-12-31T00:00:00Z, at least with
/// the current leapseconds file. We have a full day of pad here to
/// accommodate future leapseconds without needing to change this
/// constant.  
//-----------------------------------------------------------------------

  static const Time max_valid_time;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  void print(std::ostream& os) const { os << to_string(); }

//-----------------------------------------------------------------------
/// Interface to use for converting times.
//-----------------------------------------------------------------------

  static ToolkitTimeInterface* toolkit_time_interface;

//-----------------------------------------------------------------------
/// Stash a copy of a UnixToolkitTimeInterface, this is just used for
/// testing.
//-----------------------------------------------------------------------

  static ToolkitTimeInterface* _unix_toolkit_time_interface;

  static Time parse_time(const std::string& Time_string)
  { return toolkit_time_interface->parse_time(Time_string);}

  std::string to_string() const
  { return toolkit_time_interface->to_string(*this); }
};

class TimeWithDerivative : public TimeBase<AutoDerivative<double> >,
   public Printable<TimeWithDerivative>,
   private boost::addable<TimeWithDerivative, AutoDerivative<double> >,
   private boost::subtractable<TimeWithDerivative, AutoDerivative<double> >,
   private boost::less_than_comparable<TimeWithDerivative> 
{
public:
  TimeWithDerivative(const Time& T)
  {
    time_pgs_ = T.pgs();
  }
  TimeWithDerivative() {}

//-----------------------------------------------------------------------
/// Return time from given PGS toolkit time (epoch of 1993-01-01).
//-----------------------------------------------------------------------
  
  static TimeWithDerivative time_pgs(const AutoDerivative<double> pgs) 
  {TimeWithDerivative res; res.time_pgs_ = pgs; return res;}

//-----------------------------------------------------------------------
/// Return time from given J2000 time (epoch of 2000-01-01 12:00:00
/// TT). Note that TT is different than UTC noon by about 64.184 seconds
//-----------------------------------------------------------------------
  
  static TimeWithDerivative time_j2000(const AutoDerivative<double> j2000) 
  {TimeWithDerivative res; 
    // Constant here is from SPICE kernel, and is accurate to more
    // digits than the 64.184
    res.time_pgs_ = j2000 + 220881605.0 - 64.1839272778; 
    return res;}

//-----------------------------------------------------------------------
/// Return time from given GPS time (epoch of 1980-01-06).
//-----------------------------------------------------------------------
  
  static TimeWithDerivative 
  time_gps(const AutoDerivative<double>& gps) 
  {return time_pgs(gps - 409881608.0);}

//-----------------------------------------------------------------------
/// Strip off gradient to just give a time.
//-----------------------------------------------------------------------

  Time value() const { return Time::time_pgs(pgs().value()); }

//-----------------------------------------------------------------------
/// Return gradient.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> gradient() const { return pgs().gradient();}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  void print(std::ostream& os) const 
  { os << "TimeWithDerivative:\n"
       << "  " << Time::time_pgs(pgs().value()) << "\n"
       << "  Gradient: " << time_pgs_.gradient() << "\n"; }
};

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
///
/// Subtract two Times, giving the interval between them in seconds.
//-----------------------------------------------------------------------

template<class T> inline T operator-(const TimeBase<T>& T1, 
				     const TimeBase<T>& T2) 
{ return T1.pgs() - T2.pgs(); }

inline double operator-(const Time& T1, 
			const Time& T2) 
{ return T1.pgs() - T2.pgs(); }

inline AutoDerivative<double> operator-(const TimeWithDerivative& T1, 
					const Time& T2) 
{ return T1.pgs() - T2.pgs(); }

inline AutoDerivative<double> operator-(const Time& T1, 
					const TimeWithDerivative& T2) 
{ return T1.pgs() - T2.pgs(); }

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Compare Times
///
/// We define <=, >= and > in terms of this operator.
//-----------------------------------------------------------------------

inline bool operator<(const Time& T1, const Time& T2)
{ return T1 - T2 < 0; }

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Compare TimeWithDerivative
///
/// We define <=, >= and > in terms of this operator.
//-----------------------------------------------------------------------

inline bool operator<(const TimeWithDerivative& T1, const TimeWithDerivative& T2)
{ return (T1 - T2).value() < 0; }

/****************************************************************//**
  Small helper class to wrap creation of time in a common interface.
  This is useful for templates.
*******************************************************************/

struct TimeEtCreator {
  Time operator()(double t) { return Time::time_et(t); }
};

/****************************************************************//**
  Small helper class to wrap creation of time in a common interface.
  This is useful for templates.
*******************************************************************/

struct TimePgsCreator {
  Time operator()(double t) { return Time::time_pgs(t); }
};

/****************************************************************//**
  Small helper class to wrap creation of time in a common interface.
  This is useful for templates.
*******************************************************************/

struct TimeJ2000Creator {
  Time operator()(double t) { return Time::time_j2000(t); }
};

/****************************************************************//**
  Small helper class to wrap creation of time in a common interface.
  This is useful for templates.
*******************************************************************/

struct TimeAcsCreator {
  Time operator()(double t) { return Time::time_acs(t); }
};

/****************************************************************//**
  Small helper class to wrap creation of time in a common interface.
  This is useful for templates.
*******************************************************************/

struct TimeGpsCreator {
  Time operator()(double t) { return Time::time_gps(t); }
};

}
#endif
