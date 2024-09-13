#ifndef SPICE_HELPER_H
#define SPICE_HELPER_H
#include "toolkit_time_interface.h"
#include "toolkit_coordinate_interface.h"
#include "ground_coordinate.h"
#include <string>
#include <boost/regex.hpp>
#include <boost/array.hpp>
#include <boost/math/quaternion.hpp>
#include <sys/types.h>
#include <boost/filesystem.hpp>

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
  static double m2[6][6];
  static bool have_spice();
  static int name_to_body(const std::string& Name);
  static std::string body_name(int Body_id);
  static int kernel_data_int(const std::string& Dname);
  static double kernel_data_double(const std::string& Dname);
  static blitz::Array<double,1>
  kernel_data_array_double(const std::string& Dname);
  static boost::shared_ptr<GroundCoordinate>
  latsrf(int Body_id, const Time& Tm, double Lat_deg, double Lon_deg);
  static std::string fixed_frame_name(int Body_id);
  static void cartesian_inertial_to_cartesian_fixed(int Body_id, const Time& T);
  static void cartesian_inertial_to_cartesian_fixed2(int Body_id, const Time& T);
  static void cartesian_fixed_to_cartesian_inertial2(int Body_id, const Time& T);
  static Time parse_time(const std::string& Time_string);
  static double geocal_to_et(const Time& T);
  static Time et_to_geocal(double Et);
  static std::string to_string(const Time& T);
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker",
			  bool Force_kernel_pool_reset = false);
  static void add_kernel(const std::string& Kernel, bool Skip_save = false);
  static bool kernel_loaded(const std::string& Kernel);
  static void spice_error_check();
  static boost::math::quaternion<double> 
  conversion_quaternion(const std::string& From,
			const std::string& To, const Time& T);
  static void conversion_matrix(const std::string& From,
				const std::string& To, const Time& T);
  static void conversion_matrix2(const std::string& From,
				 const std::string& To, const Time& T);
  static void conversion(const std::string& From,
			 const std::string& To, const Time& T,
			 const boost::array<double, 3>& pin, 
			 boost::array<double, 3>& pout);
  static void conversion(const std::string& From,
			 const std::string& To, const Time& T,
			 const boost::array<double, 6>& pin, 
			 boost::array<double, 6>& pout);
  static void sub_solar_point_calc(const std::string& Body,
				   const std::string& Ref_frame,
				   const Time& T,
				   boost::array<double, 3>& pout,
				   boost::array<double, 3>& pout2);
  static void sub_body_point_calc(const std::string& Body,
				  const std::string& Body2,
				  const std::string& Ref_frame,
				  const Time& T,
				  boost::array<double, 3>& pout,
				  boost::array<double, 3>& pout2);
  static void state_vector(int Body_id, const std::string& Target_name,
			   const Time& T, boost::array<double, 3>& Pos,
			   boost::array<double, 3>& Vel,
			   const std::string& Abcorr = "NONE");
  static std::vector<boost::shared_ptr<GroundCoordinate> >
  boresight_and_footprint(const Time& T, int Body_id,
			  const std::string& Satellite_name,
			  const std::string& Camera_name,
			  const std::string& Corr_type = "CN+S");
  static void 
  surface_intersect(const Time& T, int Body_id,
		    const std::string& Satellite_name,
		    const std::string& Camera_name,
		    const ScLookVector& Slv,
		    boost::shared_ptr<GroundCoordinate>& Gc,
		    double &Light_time,
		    const std::string& Corr_type = "CN+S",
		    const std::string& Method = "Ellipsoid");
private:
  static std::string max_version_find(const std::string& D, 
				      const boost::regex& F_reg);
  static pid_t pid;
  static std::vector<boost::filesystem::path> kernel_list;
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
  virtual void to_inertial(int Body_id, const Time& T, 
   const CartesianFixed& From, const boost::array<double, 3>& Vel_cf,
   CartesianInertial& To, boost::array<double, 3>& Vel_ci);
  virtual void to_fixed(int Body_id, const Time& T,
    double Ci_to_cf[3][3]);
  virtual void to_fixed_with_vel(int Body_id, const Time& T,
    double Ci_to_cf[6][6]);
  virtual void to_inertial_with_vel(int Body_id, const Time& T,
    double Cf_to_ci[6][6]);
  virtual void to_fixed(int Body_id, const Time& T,
    const CartesianInertial& From, CartesianFixed& To);
  virtual void to_fixed(int Body_id, const Time& T, 
   const CartesianInertial& From, const boost::array<double, 3>& Vel_ci,
   CartesianFixed& To, boost::array<double, 3>& Vel_cf);
  virtual void
  sub_solar_point(int Body_id, const Time& T, CartesianFixed& P);
  virtual double solar_distance(int Body_id, const Time& T);
  virtual void
  sub_body_point(int Body_id, int Body2_id, const Time& T, CartesianFixed& P);
  virtual double body_distance(int Body_id, int Body2_id, const Time& T);
};

/****************************************************************//**
  Constants for the planet. Note that if the planet is actually
  described as triaxial we average the 2 equatorial radius. We don't
  curently support triaxial models. We could extend the code for this if 
  needed.
  
*******************************************************************/

class SpicePlanetConstant {
public:
  SpicePlanetConstant(int Naif_code=-1) 
    : naif_code(Naif_code), filled_in(false), a(0.0), b(0.0), esq(0.0) {}

//-----------------------------------------------------------------------
/// Planet equatorial radius, in meters
//-----------------------------------------------------------------------

  double planet_a() const 
  { fill_in_data(); return a; }

//-----------------------------------------------------------------------
/// Planet polar radius, in meters
//-----------------------------------------------------------------------

  double planet_b() const 
  { fill_in_data(); return b; }

//-----------------------------------------------------------------------
/// Planet eccentricity squared
//-----------------------------------------------------------------------

  double planet_esq() const
  { fill_in_data(); return esq; }

//-----------------------------------------------------------------------
/// Planet name.
//-----------------------------------------------------------------------
  std::string planet_name() const
  { fill_in_data(); return name;}
private:
  int naif_code;
  mutable bool filled_in;
  mutable double a, b, esq;
  mutable std::string name;
  void fill_in_data() const 
  {
    if(!filled_in) {
      calc_data();
      filled_in = true;
    }
  }
  void calc_data() const;
};
}
#endif
