#include "geocal_config.h"
#include "ground_coordinate.h"
#include "geocal_matrix.h"
#include "geocal_autoderivative_quaternion.h"
#include "geocal_serialize_support.h"
#include <cmath>
#ifdef HAVE_SPICE
#include "spice_helper.h"
#endif
#ifdef HAVE_SDP
#include "sdp_helper.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GroundCoordinate::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
}

template<class Archive>
void CartesianFixed::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(CartesianFixed, GroundCoordinate);
  ar & GEOCAL_NVP(position);
}

template<class Archive>
void CartesianInertial::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CartesianInertial);
  ar & GEOCAL_NVP(position);
}

GEOCAL_IMPLEMENT(GroundCoordinate);
GEOCAL_IMPLEMENT(CartesianFixed);
GEOCAL_IMPLEMENT(CartesianInertial);
#endif

inline double sqr(double x) { return x * x; }

/****************************************************************//**
  Each toolkit supplies methods to convert to and from CartesianInertial.
*******************************************************************/
class DummyToolkitCoordinateInterface : public ToolkitCoordinateInterface {
public:
  virtual ~DummyToolkitCoordinateInterface() {}
  virtual void to_inertial(int UNUSED(Body_id), const Time& UNUSED(T), 
   const CartesianFixed& UNUSED(From), CartesianInertial& UNUSED(To))
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianFixed to CartesianInertial.");
  }
  virtual void to_inertial(int UNUSED(Body_id), const Time& UNUSED(T), 
			   const CartesianFixed& UNUSED(From), const boost::array<double, 3>& UNUSED(Vel_c),
			   CartesianInertial& UNUSED(To), boost::array<double, 3>& UNUSED(Vel_ci))
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianFixed to CartesianInertial.");
  }
  virtual void to_fixed(int UNUSED(Body_id), const Time& UNUSED(T),
     const CartesianInertial& UNUSED(From), CartesianFixed& UNUSED(To)) 
  {
    throw Exception("Need to have either SPICE or SDP toolkit available to convert from CartesianInertial to CartesianFixed.");
  }

  virtual void to_fixed(int UNUSED(Body_id), const Time& UNUSED(T),
			double UNUSED(Ci_to_cf[3][3]))
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
  if(G1.naif_code() != G2.naif_code()) {
    Exception e;
    e << "NAIF codes in distance function don't match\n"
      << "  G1 NAIF code: " << G1.naif_code() << "\n"
      << "  G2 NAIF code: " << G2.naif_code() << "\n";
    throw e;
  }
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
 boost::array<AutoDerivative<double>, 3>& Ci_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_ci,
 boost::math::quaternion<AutoDerivative<double> >& Cf_to_ci_q
 )
{
  Ci = Cf.convert_to_ci(T.value());
  double m[6][6];
  Cf.cf_to_ci_with_vel(T.value(), m);

  const double eps = 1e-3;
  double meps[6][6];
  Cf.cf_to_ci_with_vel(T.value() + eps, meps);
  blitz::Array<double,1> tmg(T.gradient());
  AutoDerivative<double> mder[6][6];
  for(int i = 0; i < 6; ++i)
    for(int j = 0; j < 6; ++j) {
      blitz::Array<double, 1> g((meps[i][j] - m[i][j]) / eps * tmg);
      mder[i][j] = AutoDerivative<double>(m[i][j], g);
    }
  boost::array<AutoDerivative<double>, 6> from, to;
  from[0] = Cf_with_der[0];
  from[1] = Cf_with_der[1];
  from[2] = Cf_with_der[2];
  from[3] = Vel_cf[0];
  from[4] = Vel_cf[1];
  from[5] = Vel_cf[2];
  mul(mder, from, to);
  Ci_with_der[0] = to[0];
  Ci_with_der[1] = to[1];
  Ci_with_der[2] = to[2];
  Vel_ci[0] = to[3];
  Vel_ci[1] = to[4];
  Vel_ci[2] = to[5];

  AutoDerivative<double> m2[3][3];
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      m2[i][j] = m[i][j];
  Cf_to_ci_q = matrix_to_quaternion(m2);
  normalize(Cf_to_ci_q);
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
  normalize(Ci_to_cf_q);
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
 boost::array<AutoDerivative<double>, 3>& Cf_with_der,
 boost::array<AutoDerivative<double>, 3>& Vel_cf,
 boost::math::quaternion<AutoDerivative<double> >& Ci_to_cf_q
 )
{
  Cf = Ci.convert_to_cf(T.value());
  double m[6][6];
  Ci.ci_to_cf_with_vel(T.value(), m);

  const double eps = 1e-3;
  double meps[6][6];
  Ci.ci_to_cf_with_vel(T.value() + eps, meps);
  blitz::Array<double,1> tmg(T.gradient());
  AutoDerivative<double> mder[6][6];
  for(int i = 0; i < 6; ++i)
    for(int j = 0; j < 6; ++j) {
      blitz::Array<double, 1> g((meps[i][j] - m[i][j]) / eps * tmg);
      mder[i][j] = AutoDerivative<double>(m[i][j], g);
    }
  boost::array<AutoDerivative<double>, 6> from, to;
  from[0] = Ci_with_der[0];
  from[1] = Ci_with_der[1];
  from[2] = Ci_with_der[2];
  from[3] = Vel_ci[0];
  from[4] = Vel_ci[1];
  from[5] = Vel_ci[2];
  mul(mder, from, to);
  Cf_with_der[0] = to[0];
  Cf_with_der[1] = to[1];
  Cf_with_der[2] = to[2];
  Vel_cf[0] = to[3];
  Vel_cf[1] = to[4];
  Vel_cf[2] = to[5];

  AutoDerivative<double> m2[3][3];
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      m2[i][j] = m[i][j];
  Ci_to_cf_q = matrix_to_quaternion(m2);
  normalize(Ci_to_cf_q);
}
