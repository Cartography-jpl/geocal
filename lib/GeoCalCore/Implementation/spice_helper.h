#ifndef SPICE_HELPER_H
#define SPICE_HELPER_H
#include "toolkit_time_interface.h"
#include "toolkit_coordinate_interface.h"
#include <string>
#include <boost/regex.hpp>
#include <boost/array.hpp>

namespace GeoCal {
  class Time;			// Forward declaration.

/****************************************************************//**
   Exception thrown if Spice call fails.
*******************************************************************/

class SpiceException : public Exception {
public:
  SpiceException();

//-----------------------------------------------------------------------
/// SPICE error short message.
//-----------------------------------------------------------------------

  std::string short_message;

//-----------------------------------------------------------------------
/// SPICE error long message.
//-----------------------------------------------------------------------

  std::string long_message;

//-----------------------------------------------------------------------
/// SPICE explanation of short message.
//-----------------------------------------------------------------------

  std::string explain;

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~SpiceException() throw() {}
};

/****************************************************************//**
  This class centralizes all of the CSPICE calls from the CSPICE
  library (available from NAIF at
  http://naif.jpl.nasa.gov/naif/pds.html). This class handles
  initializing CSPICE, and provides one place to modify if we ever
  need to replace the dependency on this library.

  SPICE depends on various kernels. We load the file found at 
  $SPICEDATA/geocal.ker. This file in turn loads a number of other
  kernels, you can edit this file to change which kernels are used.
*******************************************************************/
class SpiceHelper {
public:
  static double m[3][3];
  static void cartesian_inertial_to_cartesian_fixed(int Body_id, const Time& T);
  static Time parse_time(const std::string& Time_string);
  static double geocal_to_et(const Time& T);
  static Time et_to_geocal(double Et);
  static std::string to_string(const Time& T);
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker");
  static void spice_error_check();
  static void conversion_matrix(const std::string& From,
				const std::string& To, const Time& T);
  static void conversion(const std::string& From,
			 const std::string& To, const Time& T,
			 const boost::array<double, 3>& pin, 
			 boost::array<double, 3>& pout);
  static void sub_solar_point_calc(const std::string& Body,
				   const std::string& Ref_frame,
				   const Time& T,
				   boost::array<double, 3>& pout);
private:
  static std::string max_version_find(const std::string& D, 
				      const boost::regex& F_reg);
};

class SpiceToolkitTimeInterface : public ToolkitTimeInterface {
public:
  virtual ~SpiceToolkitTimeInterface() {}
  virtual Time parse_time(const std::string Time_string) const;
  virtual std::string to_string(const Time& T) const;
};

class SpiceToolkitCoordinateInterface : public ToolkitCoordinateInterface {
public:
  virtual ~SpiceToolkitCoordinateInterface() {}
  virtual void to_inertial(int Body_id, const Time& T,
    const CartesianFixed& From, CartesianInertial& To);
  virtual void to_fixed(int Body_id, const Time& T,
    double Ci_to_cf[3][3]);
  virtual void to_fixed(int Body_id, const Time& T,
    const CartesianInertial& From, CartesianFixed& To);
  virtual void
  sub_solar_point(int Body_id, const Time& T, CartesianFixed& P);
};

}
#endif
