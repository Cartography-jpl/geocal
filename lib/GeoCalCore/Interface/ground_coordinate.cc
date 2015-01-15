#include "geocal_internal_config.h"
#include "ground_coordinate.h"
#include "geocal_matrix.h"
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

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

void GeoCal::convert_position_and_velocity
(const Time& T,
 const CartesianFixed& Cf,
 const boost::array<double, 3>& Vel_cf,
 boost::shared_ptr<CartesianInertial>& Ci,
 boost::array<double, 3>& Vel_ci
 )
{
  Ci = Cf.convert_to_ci(T);
  double m[6][6];
  Cf.cf_to_ci_with_vel(T, m);
  boost::array<double, 6> from, to;
  from[0] = Cf.position[0];
  from[1] = Cf.position[1];
  from[2] = Cf.position[2];
  from[3] = Vel_cf[0];
  from[4] = Vel_cf[1];
  from[5] = Vel_cf[2];
  mul(m, from, to);
  Vel_ci[0] = to[3];
  Vel_ci[1] = to[4];
  Vel_ci[2] = to[5];
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

void GeoCal::convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianFixed& Cf,
 const boost::array<AutoDerivative<double>, 3>& Cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_cf,
 boost::shared_ptr<CartesianInertial>& Ci,
 const boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_ci
 )
{
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

void GeoCal::convert_position_and_velocity
(const Time& T,
 const CartesianInertial& Ci,
 const boost::array<double, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 boost::array<double, 3>& Vel_cf,
 boost::math::quaternion<double>& Ci_to_cf_q
 )
{
  Cf = Ci.convert_to_cf(T);
  double m[6][6];
  Ci.ci_to_cf_with_vel(T, m);
  boost::array<double, 6> from, to;
  from[0] = Ci.position[0];
  from[1] = Ci.position[1];
  from[2] = Ci.position[2];
  from[3] = Vel_ci[0];
  from[4] = Vel_ci[1];
  from[5] = Vel_ci[2];
  mul(m, from, to);
  Vel_cf[0] = to[3];
  Vel_cf[1] = to[4];
  Vel_cf[2] = to[5];
  double m2[3][3];
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      m2[i][j] = m[i][j];
  Ci_to_cf_q = matrix_to_quaternion(m2);
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Convert including velocity.
//-----------------------------------------------------------------------

void GeoCal::convert_position_and_velocity
(const TimeWithDerivative& T,
 const CartesianInertial& Ci,
 const boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_ci,
 boost::shared_ptr<CartesianFixed>& Cf,
 const boost::array<AutoDerivative<double>, 3>& Cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& Vel_cf
 )
{
}
