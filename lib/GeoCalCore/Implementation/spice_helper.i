// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "spice_helper.h"
#include "geocal_time.h"
%}
%import "geocal_quaternion.i"
%import "geocal_time.i"
namespace GeoCal {
class SpiceHelper  {
public:
  static std::string body_name(int Body_id);
  static std::string fixed_frame_name(int Body_id);
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker");
  static void add_kernel(const std::string& Kernel_dir, 
			 const std::string& Kernel);
  static boost::math::quaternion<double> 
  conversion_quaternion(const std::string& From,
			const std::string& To, const Time& T);
};
}
