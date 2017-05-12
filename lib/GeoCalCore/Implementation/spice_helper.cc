#include "geocal_internal_config.h"
#include "spice_helper.h"
#include "geocal_time.h"
#include "geocal_exception.h"
#include "geocal_matrix.h"
#include "geocal_quaternion.h"
#include "ground_coordinate.h"
#include "dir_change.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <boost/filesystem.hpp>
#ifdef HAVE_SPICE
extern "C" {
#include "SpiceUsr.h"
}
#endif

using namespace GeoCal;

double SpiceHelper::m[3][3];
double SpiceHelper::m2[6][6];

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
/// Add an additional kernel, after the one we automatically get
/// (i.e., $SPICEDATA/geocal.ker).
///
/// We change to the given Kernel_dir, so you can use relative paths
/// if desired for the kernel names.
//-----------------------------------------------------------------------

void SpiceHelper::add_kernel(const std::string& Kernel_dir, 
			     const std::string& Kernel)
{
#ifdef HAVE_SPICE
  spice_setup();
  DirChange dc(Kernel_dir);
  furnsh_c(Kernel.c_str());
  spice_error_check();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Check if a given kernel file has already been loaded.
///
/// Note that this is a bit limited, this checks against the exact
/// name that was loaded. So if you load "dir/bar/foo.ker" and then
/// check against "dir/./bar/foo.ker" will return false even though
/// this is the same file. This is just a limitation of the spice
/// function call.
//-----------------------------------------------------------------------

bool SpiceHelper::kernel_loaded(const std::string& Kernel)
{
#ifdef HAVE_SPICE
  spice_setup();
  char filetype[1000], source[1000];
  int handle;
  SpiceBoolean found;
  kinfo_c(Kernel.c_str(), 1000,1000, filetype, source, &handle, &found);
  spice_error_check();
  return found;
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Add an additional kernel, after the one we automatically get
/// (i.e., $SPICEDATA/geocal.ker).
//-----------------------------------------------------------------------

void SpiceHelper::add_kernel(const std::string& Kernel)
{
#ifdef HAVE_SPICE
  spice_setup();
  boost::filesystem::path p(Kernel);
  std::string dir = p.parent_path().string();
  std::string fname = p.filename().string();
  if(dir == "")
    dir = ".";
  DirChange d(dir);
  furnsh_c(fname.c_str());
  spice_error_check();
#else 
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Set SPICE errors to just return, rather than aborting.
//-----------------------------------------------------------------------

void SpiceHelper::spice_setup(const std::string& Kernel)
{
  static int already_done = 0;
#ifdef HAVE_SPICE
  if(!already_done) {
    errprt_c(const_cast<char*>("SET"), 10, const_cast<char*>("NONE"));
    erract_c(const_cast<char*>("SET"), 10, const_cast<char*>("RETURN"));
    if(!getenv("SPICEDATA"))
      throw Exception("Must have environment variable SPICEDATA set to use SPICE library");
    DirChange dc(getenv("SPICEDATA"));
    furnsh_c(Kernel.c_str());
    spice_error_check();
    already_done = 1;
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
/// Convert from the given coordinate to the given coordinate.
//-----------------------------------------------------------------------

void SpiceHelper::conversion(const std::string& From,
			     const std::string& To, const Time& T,
			     const boost::array<double, 6>& pin, 
			     boost::array<double, 6>& pout)
{
  conversion_matrix2(From, To, T);
  mul(m2, pin, pout);
}

//-----------------------------------------------------------------------
/// Calculate the sub solar point, and also the vector from the Sun to
/// the sub solar point.
//-----------------------------------------------------------------------

void SpiceHelper::sub_solar_point_calc(const std::string& Body,
				       const std::string& Ref_frame,
				       const Time& T,
				       boost::array<double, 3>& pout,
				       boost::array<double, 3>& pout2)
{
#ifdef HAVE_SPICE
  spice_setup();
  double trgepc;
  // Note that we really do *not* want to correct for light travel
  // time and aberration. This is for the *observer*, or in this case
  // the Sun. We don't want to correct the time we looking at the
  // earth by when the Sun happens to see it.
  //
  // It is possible that if we were working with something in mid or
  // high orbit we might care, but in that case we would need the
  // satellite available through a spice kernel, and we would need to 
  // make a different call. We just ignore the light travel time
  // (which is small), and report the subsolar point at the instance
  // in time at the surface.
  //
  // Note that we use the Sun here as the observer because we are also
  // often interested in the sun distance. We can get that through pout
  // (vector from center of earth to subsolar point) and pout2 (vector 
  // from observer to subsolar point, or in this case sun to subsolar
  // point).
  subslr_c(const_cast<char*>("Intercept: ellipsoid"),
	   const_cast<char*>(Body.c_str()), T.et(),
	   const_cast<char*>(Ref_frame.c_str()),
	   //const_cast<char*>("lt+s"),
	   const_cast<char*>("NONE"),
	   const_cast<char*>("SUN"), &pout[0], &trgepc, &pout2[0]);
  spice_error_check();
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Get the state vector (position and velocity, in meters), in the
/// fixed coordinates for the given Body_id, and the given Time. The
/// Target name can be anything spice recognizes.
///
/// Note we don't handle light travel time yet, or aberration. It
/// isn't clear if we want to or not.
//-----------------------------------------------------------------------

void SpiceHelper::state_vector
(int Body_id, const std::string& Target_name,
 const Time& T, boost::array<double, 3>& Pos,
 boost::array<double, 3>& Vel)
{
#ifdef HAVE_SPICE
  spice_setup();
  double state[6], lt;
  spkezr_c(Target_name.c_str(), T.et(), 
	   fixed_frame_name(Body_id).c_str(), "NONE", 
	   body_name(Body_id).c_str(), state, &lt);
  spice_error_check();
  for(int i = 0; i < 3; ++i) {
    Pos[i] = state[i] * 1000.0;
    Vel[i] = state[i + 3] * 1000.0;
  }
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return quaternion that converts between the two named coordinate 
/// systems.
//-----------------------------------------------------------------------

boost::math::quaternion<double> 
SpiceHelper::conversion_quaternion(const std::string& From,
				   const std::string& To, const Time& T)
{
  conversion_matrix(From, To, T);
  return matrix_to_quaternion(m);
}

//-----------------------------------------------------------------------
/// Return matrix that converts between the two named coordinate
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
/// Return matrix that converts between the two named coordinate
/// system, including velocify. To avoid making a second copy, we
/// return the data in SpiceHelper::m2. 
//-----------------------------------------------------------------------

void SpiceHelper::conversion_matrix2(const std::string& From,
				     const std::string& To, const Time& T)
{
#ifdef HAVE_SPICE
  spice_setup();
  sxform_c(const_cast<char*>(From.c_str()), const_cast<char*>(To.c_str()), 
	   T.et(), m2);
  spice_error_check();
#else
  throw SpiceNotAvailableException();
#endif
}


//-----------------------------------------------------------------------
/// Return the body name for the given id.
//-----------------------------------------------------------------------

std::string SpiceHelper::body_name(int Body_id)
{
  // Handle some special cases for speed.
  switch(Body_id) {
  case 399:
    return "EARTH";
    break;
  case 499:			// Mars
    return "MARS";
    break;
  case 502:			// Europa
    return "EUROPA";
    break;
  case 301:			// Moon
    return "MOON";
    break;
  }
#ifdef HAVE_SPICE
  spice_setup();
  const int maxlen = 100;
  char name[maxlen];
  SpiceBoolean found;
  bodc2n_c(Body_id, maxlen, name, &found);
  spice_error_check();
  if(!found) {
    Exception e;
    e << "Could not find name for the Body id " << Body_id;
    throw e;
  }
  return name;
#else
  throw SpiceNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return the fixed frame name for the given body. We pull this out
/// so we don't have lots of switch statements elsewhere.
//-----------------------------------------------------------------------

std::string SpiceHelper::fixed_frame_name(int Body_id)
{
  // Handle some special cases for speed.
  switch(Body_id) {
  case 399:
    return "ITRF93";
    break;
  case 499:			// Mars
    return "IAU_MARS";
    break;
  case 502:			// Europa
    return "IAU_EUROPA";
    break;
  case 301:			// Moon
    return "IAU_MOON";
    break;
  }
  // Default to IAU_ + the name of the body 
  return "IAU_" + body_name(Body_id);
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
  conversion_matrix("J2000", fixed_frame_name(Body_id), T);
}

//-----------------------------------------------------------------------
/// Return matrix that converts from CartesianInertial to
/// CartesianFixed including velocity. To avoid making a second copy,
/// we return the data 
/// in SpiceHelper::m2. Since this is only called by
/// SpiceToolkitCoordinateInterface, this slightly awkward interface
/// is worth the small performance advantage.
//-----------------------------------------------------------------------

void SpiceHelper::cartesian_inertial_to_cartesian_fixed2(int Body_id, 
							const Time& T)
{
  conversion_matrix2("J2000", fixed_frame_name(Body_id), T);
}

void SpiceHelper::cartesian_fixed_to_cartesian_inertial2(int Body_id, 
							const Time& T)
{
  conversion_matrix2(fixed_frame_name(Body_id), "J2000", T);
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
/// This converts from CartesianFixed to CartesianInertial for the
/// given body, including velocity. We use the NAIF coding for the
/// bodies (see the SPICE documentation for details). We use this
/// because it is a unique  coding, the underlying toolkit doesn't
/// need to be SPICE. 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_inertial
(int Body_id, const Time& T, 
 const CartesianFixed& From, const boost::array<double, 3>& Vel_cf,
 CartesianInertial& To, boost::array<double, 3>& Vel_ci)
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed(Body_id, T);
  boost::array<double, 6> from, to;
  from[0] = From.position[0];
  from[1] = From.position[1];
  from[2] = From.position[2];
  from[3] = Vel_cf[0];
  from[4] = Vel_cf[1];
  from[5] = Vel_cf[2];
  mul_t(SpiceHelper::m2, from, to);
  To.position[0] = to[0];
  To.position[1] = to[1];
  To.position[2] = to[2];
  Vel_ci[0] = to[3];
  Vel_ci[1] = to[4];
  Vel_ci[2] = to[5];
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
/// This calculates a matrix from converting from CartesianInertial to
/// CartesianFixed with velocity for the  given body. We use the NAIF
/// coding for the bodies (see the SPICE documentation for details). 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_fixed_with_vel(int Body_id, 
  const Time& T, double Ci_to_cf[6][6])
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed2(Body_id, T);
  mat_copy(SpiceHelper::m2, Ci_to_cf);
}

//-----------------------------------------------------------------------
/// This calculates a matrix from converting from CartesianFixed to
/// CartesianInertial with velocity for the  given body. We use the NAIF
/// coding for the bodies (see the SPICE documentation for details). 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_inertial_with_vel(int Body_id, 
  const Time& T, double Cf_to_ci[6][6])
{
  SpiceHelper::cartesian_fixed_to_cartesian_inertial2(Body_id, T);
  mat_copy(SpiceHelper::m2, Cf_to_ci);
}

void
SpiceToolkitCoordinateInterface::sub_solar_point(int Body_id, const Time& T, 
						 CartesianFixed& P)
{
  boost::array<double, 3> ign;
  SpiceHelper::sub_solar_point_calc(SpiceHelper::body_name(Body_id).c_str(), 
			    SpiceHelper::fixed_frame_name(Body_id).c_str(), 
			    T, P.position, ign);
  // sub_solar_point_calc returns km, *not* meter. Since
  // CartesianFixed expected meter convert this.
  P.position[0] *= 1000.0;
  P.position[1] *= 1000.0;
  P.position[2] *= 1000.0;
}

double
SpiceToolkitCoordinateInterface::solar_distance(int Body_id, const Time& T)
{
  boost::array<double, 3> pout1, pout2;
  SpiceHelper::sub_solar_point_calc(SpiceHelper::body_name(Body_id).c_str(), 
			    SpiceHelper::fixed_frame_name(Body_id).c_str(), 
			    T, pout1, pout2);
  // pout1 takes us from the center of the body to the subsolar
  // point, and then pout2 takes us to the sun. So together they are
  // the distance from the center of body to the sun. This is in km,
  // so we convert to meters.
  return (norm(pout1) + norm(pout2)) * 1000.0;
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
/// This converts from CartesianInertial to CartesianFixed for the
/// given body, including velocity. We use the NAIF coding for the
/// bodies (see the SPICE documentation for details). We use this
/// because it is a unique  coding, the underlying toolkit doesn't
/// need to be SPICE. 
//-----------------------------------------------------------------------

void SpiceToolkitCoordinateInterface::to_fixed
(int Body_id, const Time& T, 
 const CartesianInertial& From, const boost::array<double, 3>& Vel_ci,
 CartesianFixed& To, boost::array<double, 3>& Vel_cf)
{
  SpiceHelper::cartesian_inertial_to_cartesian_fixed(Body_id, T);
  boost::array<double, 6> from, to;
  from[0] = From.position[0];
  from[1] = From.position[1];
  from[2] = From.position[2];
  from[3] = Vel_ci[0];
  from[4] = Vel_ci[1];
  from[5] = Vel_ci[2];
  mul(SpiceHelper::m2, from, to);
  To.position[0] = to[0];
  To.position[1] = to[1];
  To.position[2] = to[2];
  Vel_cf[0] = to[3];
  Vel_cf[1] = to[4];
  Vel_cf[2] = to[5];
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

//-----------------------------------------------------------------------
/// Calculate planet data.
//-----------------------------------------------------------------------

void SpicePlanetConstant::calc_data() const
{
  SpiceHelper::spice_setup();
#ifdef HAVE_SPICE
  char name_buf[100];
  SpiceBoolean found;
  bodc2n_c(naif_code, 100, name_buf, &found);
  if(found)
    name = std::string(name_buf);
  else
    name = "Unknown";
  std::string bname = boost::lexical_cast<std::string>(naif_code);
  int dim;
  double values[3];
  bodvrd_c(bname.c_str(), "RADII", 3, &dim, values);
  SpiceHelper::spice_error_check();
  if(dim != 3)
    throw Exception("Call to bodvrd_c didn't returned expected values");
  // 1000.0 is to convert from km returned by bodvrd_c and meter we
  // use everywhere else.
  a = (values[0] + values[1]) / 2.0 * 1000.0;
  b = values[2] * 1000.0;
  esq = (a * a - b * b) / (a * a);
				// Eccentricity squared. From CRC.
#endif
}
