#include "config.h"
#include "spice_helper.h"
#include "geocal_time.h"
#include "geocal_exception.h"
#include "geocal_matrix.h"
#include "ground_coordinate.h"
#include "dir_change.h"
#include <boost/regex.hpp>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#ifdef HAVE_SPICE
extern "C" {
#include "SpiceUsr.h"
}
#endif

using namespace GeoCal;

double SpiceHelper::m[3][3];

//-----------------------------------------------------------------------
/// Spice exception. This automatically gets the SPICE error message.
/// As a side effect, we also clear the error status of the SPICE library.
//-----------------------------------------------------------------------

SpiceException::SpiceException()
{
#ifdef HAVE_SPICE
  static const int maxlen = 2000;
  char msg[maxlen];
  getmsg_c ( "LONG", maxlen, msg);
  long_message = std::string(msg);
  getmsg_c ( "SHORT", maxlen, msg);
  short_message = std::string(msg);
  getmsg_c ( "EXPLAIN", maxlen, msg);
  explain = std::string(msg);
  *this << "SPICE toolkit error:\n"
	<< "  " << short_message << " -- " << explain << "\n\n"
	<< "  " << long_message << "\n";
  reset_c();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Set SPICE errors to just return, rather than aborting.
//-----------------------------------------------------------------------

void SpiceHelper::spice_setup(const std::string& Kernel)
{
#ifdef HAVE_SPICE
  static bool already_done = false;
  if(!already_done) {
    errprt_c(const_cast<char*>("SET"), 10, const_cast<char*>("NONE"));
    erract_c(const_cast<char*>("SET"), 10, const_cast<char*>("RETURN"));
    if(!getenv("SPICEDATA"))
      throw Exception("Must have environment variable SPICEDATA set to use SPICE library");
    DirChange dc(getenv("SPICEDATA"));
    furnsh_c(Kernel.c_str());
    spice_error_check();
    already_done = true;
  }
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Check if an error occurred in a SPICE call, and if so throw an
/// exception.
//-----------------------------------------------------------------------

void SpiceHelper::spice_error_check()
{
#ifdef HAVE_SPICE
  if(failed_c())
    throw SpiceException();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This routine is no longer used, instead we read one master kernel
/// file (e.g., "geocal.ker") that points to all the others. However
/// it is possible that we will to change this at some point in the
/// future, so I'll leave this old function in place even though it
/// isn't currently used.
///
/// This searchs through the directory $(SPICEDATA)/D for all files
/// matching the regular expression F_reg. The version is extracted as
/// $1 of the regular expression, and the largest version number is
/// selected. If no match is found, the empty string "" is returned.
//-----------------------------------------------------------------------

std::string SpiceHelper::max_version_find(const std::string& D, 
					  const boost::regex& F_reg)
{
#ifdef HAVE_SPICE
  if(!getenv("SPICEDATA"))
    throw Exception("Must have environment variable SPICEDATA set to use SPICE library");

  std::string path = std::string(getenv("SPICEDATA")) + "/" + D;
  DIR* dir = opendir(path.c_str());
  if(!dir)
    return "";
  int maxi = -1;
  std::string res = "";
  while(struct dirent* dp = readdir(dir)) {
    boost::cmatch m;
    if(boost::regex_match(dp->d_name, m, F_reg)) {
      std::string t = m[1];
      int i = atoi(t.c_str());
      if(i > maxi) {
	maxi = i;
	res = path + "/" + dp->d_name;
      }
    }
  }
  closedir(dir);
  return res;
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a wrapper around the SPICE call str2et_c that parses a
/// string to produce a time. We convert the spice time to
/// GeoCal::Time. See the spice documentation for detailed information 
/// on the parse strings (see
/// http://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/str2et_c.html)
///
/// Note that str2et_c doesn't support the CCSDS ASCII time format
/// (e.g., "1996-07-03T04:13:57.987654Z"). Because this is a format
/// commonly used in EOS missions, we add support for this format.
/// This is little more than a slight reformatting before calling
/// str2et_c (e.g., "1996-07-03 04:13:57.987654 UTC" which the toolkit
/// does support).
//-----------------------------------------------------------------------

Time SpiceHelper::parse_time(const std::string& Time_string)
{
#ifdef HAVE_SPICE
  spice_setup();
  double et;

//-----------------------------------------------------------------------
// Change CCSDS ASCII format to one that str2et_c can parse. This
// changes something like "1996-07-03T04:13:57.987654Z" to
// "1996-07-03 04:13:57.987654 UTC"
//-----------------------------------------------------------------------

  std::string t = 
    boost::regex_replace(Time_string, 
     boost::regex("(\\d{4}-\\d{2}-\\d{2})T(\\d{2}:\\d{2}:\\d{2}(\\.\\d+)?)Z"), 
			 "\\1 \\2 UTC");
  str2et_c(t.c_str(), &et);
  spice_error_check();
  return Time::time_et(et);
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Convert from the given coordinate to the given coordinate.
//-----------------------------------------------------------------------

void SpiceHelper::conversion(const std::string& From,
			     const std::string& To, const Time& T,
			     const boost::array<double, 3>& pin, 
			     boost::array<double, 3>& pout)
{
  conversion_matrix(From, To, T);
  mul(m, pin, pout);
}

//-----------------------------------------------------------------------
/// Return matrix that converts between the two names coordinate
/// system. To avoid making a second copy, we return the data in
/// SpiceHelper::m. 
//-----------------------------------------------------------------------

void SpiceHelper::conversion_matrix(const std::string& From,
				    const std::string& To, const Time& T)
{
#ifdef HAVE_SPICE
  spice_setup();
  pxform_c(const_cast<char*>(From.c_str()), const_cast<char*>(To.c_str()), 
	   T.et(), m);
  spice_error_check();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return matrix that converts from CartesianInertial to
/// CartesianFixed. To avoid making a second copy, we return the data
/// in SpiceHelper::m. Since this is only called by
/// SpiceToolkitCoordinateInterface, this slightly awkward interface
/// is worth the small performance advantage.
//-----------------------------------------------------------------------

void SpiceHelper::cartesian_inertial_to_cartesian_fixed(int Body_id, 
							const Time& T)
{
#ifdef HAVE_SPICE
  switch(Body_id) {
  case 399:			// Earth
    conversion_matrix("J2000", "ITRF93", T);
    break;
  case 499:			// Mars
    conversion_matrix("J2000", "IAU_MARS", T);
    break;
  case 301:			// Moon
    conversion_matrix("J2000", "IAU_MOON", T);
    break;
  default:
    throw Exception("Not yet implemented");
  }
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Convert from GeoCal::Time to Spice ET time.
//-----------------------------------------------------------------------

double SpiceHelper::geocal_to_et(const Time& T)
{
#ifdef HAVE_SPICE
  spice_setup();
  // The number 220881573 is the number of seconds between the J2000
  // epoch and the pgs epoch (note that this is UTC J2000, not the
  // more commonly used TT J2000. unitim_c takes care of the
  // difference to TT used by et.

  return unitim_c(T.pgs() - 220881573.0, "TAI", "TDB"); 
  spice_error_check();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Convert from Spice ET time to GeoCal::Time.
//-----------------------------------------------------------------------

Time SpiceHelper::et_to_geocal(double Et) 
{
#ifdef HAVE_SPICE
  spice_setup();
  // The number 220881573 is the number of seconds between the J2000
  // epoch and the pgs epoch (note that this is UTC J2000, not the
  // more commonly used TT J2000. unitim_c takes care of the
  // difference to TT used by et.
  Time res = Time::time_pgs(unitim_c(Et, "TDB", "TAI") + 220881573.0);
  spice_error_check();
  return res;
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This converts Time to CCSDS ASCII time format (e.g.,
/// "1996-07-03T04:13:57.987654Z").
//-----------------------------------------------------------------------

std::string SpiceHelper::to_string(const Time& T)
{
#ifdef HAVE_SPICE
  spice_setup();
  char temp[28];
  timout_c(T.et(), "YYYY-MM-DDTHR:MN:SC.######Z", 28, temp);
  spice_error_check();
  return temp;
#else
  throw SpiceNotAvailableException();
#endif
}

Time SpiceToolkitTimeInterface::parse_time(const std::string Time_string) const
{ 
  return SpiceHelper::parse_time(Time_string);
}

std::string SpiceToolkitTimeInterface::to_string(const Time& T) const
{
  return SpiceHelper::to_string(T);
}


//-----------------------------------------------------------------------
/// This converts from CartesianFixed to CartesianInertial for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_inertial(int Body_id, 
  const Time& T, const CartesianFixed& From, CartesianInertial& To)
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed(Body_id, T);
  mul_t(SpiceHelper::m, From.position, To.position);
}

//-----------------------------------------------------------------------
/// This calculates a matrix from converting from CartesianInertial to
/// CartesianFixed for the  given body. We use the NAIF coding for
/// the bodies (see the SPICE documentation for details). 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_fixed(int Body_id, 
  const Time& T, double Ci_to_cf[3][3])
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed(Body_id, T);
  mat_copy(SpiceHelper::m, Ci_to_cf);
}

//-----------------------------------------------------------------------
/// This converts from CartesianInertial to CartesianFixed for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_fixed(int Body_id, 
  const Time& T, const CartesianInertial& From, CartesianFixed& To)
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed(Body_id, T);
  mul(SpiceHelper::m, From.position, To.position);
}

//-----------------------------------------------------------------------
/// Return true if we have SPICE functionality available, false otherwise.
//-----------------------------------------------------------------------

bool SpiceHelper::spice_available()
{
#ifdef HAVE_SPICE
  return true;
#else
  return false;
#endif
}
