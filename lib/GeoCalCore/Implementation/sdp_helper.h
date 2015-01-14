#ifndef SDP_HELPER_H
#define SDP_HELPER_H
#include "toolkit_time_interface.h"
#include "toolkit_coordinate_interface.h"
#include "geocal_time.h"

namespace GeoCal {
class CartesianFixed;
class CartesianInertial;
/****************************************************************//**
  This class centralizes all of the SDP library calls. This is the
  toolkit used by the EOS missions. This class provides one place to
  modify if we ever need to replace the dependency on this library.
  The library is available from
  http://newsroom.gsfc.nasa.gov/sdptoolkit/toolkit.html.

  To use the SDP toolkit, you need to supply a Process Control File
  (PCF) and set the environment variable PGS_PC_INFO_FILE. See the
  toolkit documentation for details, available at
  http://edhs1.gsfc.nasa.gov/waisdata/toc/cd33360503toc.html.
******************************************************************/
class SdpHelper {
public:
  static Time parse_time(const std::string& Time_string);
  static std::string to_string(const Time& T);
  static void convert_to_eci(const Time& T, const CartesianFixed& From,
			     CartesianInertial& To);
  static void convert_to_ecr(const Time& T, double Ci_to_cf[3][3]);
  static void convert_to_ecr(const Time& T, const CartesianInertial& From,
			     CartesianFixed& To);
  static bool sdp_available();
};

class SdpToolkitTimeInterface : public ToolkitTimeInterface {
public:
  virtual ~SdpToolkitTimeInterface() {}
  virtual Time parse_time(const std::string Time_string) const;
  virtual std::string to_string(const Time& T) const;
};

class SdpToolkitCoordinateInterface : public ToolkitCoordinateInterface {
public:
  virtual ~SdpToolkitCoordinateInterface() {}
  virtual void to_inertial(int Body_id, const Time& T,
    const CartesianFixed& From, CartesianInertial& To);
  virtual void to_inertial(int Body_id, const Time& T, 
   const CartesianFixed& From, const boost::array<double, 3>& Vel_cf,
   CartesianInertial& To, boost::array<double, 3>& Vel_ci);
  virtual void to_fixed(int Body_id, const Time& T,
    double Ci_to_cf[3][3]);
  virtual void to_fixed_with_vel(int Body_id, const Time& T,
    double Ci_to_cf[6][6]);
  virtual void to_fixed(int Body_id, const Time& T,
    const CartesianInertial& From, CartesianFixed& To);
  virtual void to_fixed(int Body_id, const Time& T, 
   const CartesianInertial& From, const boost::array<double, 3>& Vel_ci,
   CartesianFixed& To, boost::array<double, 3>& Vel_cf);
  virtual void 
  sub_solar_point(int Body_id, const Time& T, CartesianFixed& P);
  virtual double solar_distance(int Body_id, const Time& T);
};

}
#endif
