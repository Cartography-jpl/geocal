// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%{
  // Need this before blitz gets included.
#include "auto_derivative.h"
%}
%include "geocal_common.i"

%{
#include "spice_helper.h"
#include "geocal_time.h"
#include "geocal_autoderivative_quaternion.h"
%}
%import "geocal_quaternion.i"
%import "geocal_time.i"
%import "ground_coordinate.i"
%include "geocal_time_include.i"
namespace GeoCal {
class SpiceHelper  {
public:
  static int name_to_body(const std::string& Name);
  static std::string body_name(int Body_id);
  static boost::shared_ptr<GroundCoordinate>
  latsrf(int Body_id, const Time& Tm, double Lat_deg, double Lon_deg);
  static std::string fixed_frame_name(int Body_id);
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker",
			  bool Force_kernel_pool_reset = false);
  static void add_kernel(const std::string& Kernel);
  static bool kernel_loaded(const std::string& Kernel);
  static boost::math::quaternion<double> 
  conversion_quaternion(const std::string& From,
			const std::string& To, const Time& T);
  static void state_vector(int Body_id, const std::string& Target_name,
			   const Time& T, boost::array<double, 3>& OUT,
			   boost::array<double, 3>& OUT);
  static std::vector<boost::shared_ptr<GroundCoordinate> >
  boresight_and_footprint(const Time& T, int Body_id,
			  const std::string& Satellite_name,
			  const std::string& Camera_name,
			  const std::string& Corr_type = "CN+S");
};
}
