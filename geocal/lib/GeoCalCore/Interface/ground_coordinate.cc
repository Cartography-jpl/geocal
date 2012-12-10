#include "ground_coordinate.h"
#include <cmath>
#ifdef HAVE_SPICE
#include "spice_helper.h"
#endif
#ifdef HAVE_SDP
#include "sdp_helper.h"
#endif

using namespace GeoCal;

inline double sqr(double x) { return x * x; }

/****************************************************************//**
  Each toolkit supplies methods to convert to and from CartesianInertial.
*******************************************************************/
class DummyToolkitCoordinateInterface : public ToolkitCoordinateInterface {
public:
  virtual ~DummyToolkitCoordinateInterface() {}
  virtual void to_inertial(int Body_id, const Time& T,
    const CartesianFixed& From, CartesianInertial& To)
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianFixed to CartesianInertial.");
  }
  virtual void to_inertial(int Body_id, const Time& T, 
			   double Cf_to_ci[3][3])
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianFixed to CartesianInertial.");
  }
  virtual void to_fixed(int Body_id, const Time& T,
    const CartesianInertial& From, CartesianFixed& To) 
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianInertial to CartesianFixed.");
  }

  virtual void to_fixed(int Body_id, const Time& T,
			double Ci_to_cf[3][3])
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianInertial to CartesianFixed.");
  }

};

#ifdef HAVE_SPICE
ToolkitCoordinateInterface* CartesianFixed::toolkit_coordinate_interface =
  new SpiceToolkitCoordinateInterface();
#else
  #ifdef HAVE_SDP
ToolkitCoordinateInterface* CartesianFixed::toolkit_coordinate_interface =
  new SdpToolkitCoordinateInterface();
  #else
ToolkitCoordinateInterface* CartesianFixed::toolkit_coordinate_interface =
  new DummyToolkitCoordinateInterface();
  #endif
#endif

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Distance in meters between ground coordinates. This is the
/// distance of the points in 3 space, i.e. it is not distance along a
/// geodesic or anything like that.
//-----------------------------------------------------------------------

double GeoCal::distance(const GroundCoordinate& G1, const GroundCoordinate& G2)
{
  boost::array<double, 3> t1 = G1.convert_to_cf()->position;
  boost::array<double, 3> t2 = G2.convert_to_cf()->position;
  return sqrt(sqr(t1[0] - t2[0]) + sqr(t1[1] - t2[1]) + sqr(t1[2] - t2[2]));
}
