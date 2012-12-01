#include "unit_test_support.h"
#include "orbit.h"
#include "geodetic.h"
#include "config.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(kepler_orbit_data)
{
#ifdef HAVE_TIME_TOOLKIT
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  BOOST_CHECK_CLOSE(orb.min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[0], -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[1], -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[2], -16811.0, 1e-3);
  try {
    orb.position_ci(t - 10);
    BOOST_FAIL("Should have thrown exception");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
  try {
    orb.position_ci(t + 110);
    BOOST_FAIL("Should have thrown exception");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
#endif
}

class OrbitTest : public KeplerOrbit {
public:
  void interpolate_t(const boost::array<double, 3>& P1,
		     const boost::array<double, 3>& V1,
		     const boost::array<double, 3>& P2,
		     const boost::array<double, 3>& V2,
		     double toffset, double tspace,
		     boost::array<double, 3>& Pres,
		     boost::array<double, 3>& Vres) const
  { interpolate(P1, V1, P2, V2, toffset, tspace, Pres, Vres); }
  boost::math::quaternion<double> interpolate_t(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace) const
  { return interpolate(Q1, Q2, toffset, tspace); }
};

BOOST_AUTO_TEST_CASE(basic_test)
{
  OrbitTest ot;
  boost::array<double, 3> p1 = {{1, 2, 3}};
  boost::array<double, 3> v1 = {{4, 5, 6}};
  boost::array<double, 3> p2 = {{7, 8, 9}};
  boost::array<double, 3> v2 = {{10, 11, 12}};
  boost::array<double, 3> pres, vres;
  ot.interpolate_t(p1, v1, p2, v2, 0, 2, pres, vres);
  BOOST_CHECK_CLOSE(pres[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[1], 2.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[2], 3.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[0], 4.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[1], 5.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[2], 6.0, 1e-4);
  ot.interpolate_t(p1, v1, p2, v2, 2, 2, pres, vres);
  BOOST_CHECK_CLOSE(pres[0], 7.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[1], 8.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[2], 9.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[0], 10.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[1], 11.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[2], 12.0, 1e-4);
  boost::math::quaternion<double> q1(-0.3087939999536268,    
				     -0.8376664361461180,     
				     0.3189198092995629,    
				     -0.3182001926197278);
  boost::math::quaternion<double> q2(-0.3423342205519756,    
				     -0.7019587136127043,     
				     0.5494114237300394,    
				     -0.2969988776025926);
  boost::math::quaternion<double> qres = ot.interpolate_t(q1, q2, 0, 2);
  BOOST_CHECK(l1(qres - q1) < 1e-8);
  qres = ot.interpolate_t(q1, q2, 2, 2);
  BOOST_CHECK(l1(qres - q2) < 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
