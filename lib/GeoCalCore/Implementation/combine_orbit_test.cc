#include "unit_test_support.h"
#include "combine_orbit.h"
#include "orbit.h"

using namespace GeoCal;
using namespace blitz;

class CombineOrbitFixture : public GlobalFixture {
public:
  CombineOrbitFixture() {
    // Create simple KeplerOrbit instances with different time ranges
    t0 = Time::time_pgs(100.0);
    t1 = Time::time_pgs(200.0);
    t2 = Time::time_pgs(300.0);
    t3 = Time::time_pgs(400.0);
    t4 = Time::time_pgs(500.0);
    t5 = Time::time_pgs(600.0);

    // Non-overlapping orbits
    orb1.reset(new KeplerOrbit(t0, t1));  // [100, 200)
    orb2.reset(new KeplerOrbit(t2, t3));  // [300, 400)
    orb3.reset(new KeplerOrbit(t4, t5));  // [500, 600)

    // Overlapping orbits
    orb_overlap1.reset(new KeplerOrbit(t0, t1));  // [100, 200)
    orb_overlap2.reset(new KeplerOrbit(Time::time_pgs(150.0), Time::time_pgs(250.0)));  // [150, 250)
    orb_overlap3.reset(new KeplerOrbit(t1, t2));  // [200, 300)

    // For constraint violation tests
    orb_bad1.reset(new KeplerOrbit(Time::time_pgs(160.0), Time::time_pgs(280.0)));  // Would cause 2 overlaps
  }

  Time t0, t1, t2, t3, t4, t5;
  boost::shared_ptr<KeplerOrbit> orb1, orb2, orb3;
  boost::shared_ptr<KeplerOrbit> orb_overlap1, orb_overlap2, orb_overlap3;
  boost::shared_ptr<KeplerOrbit> orb_bad1;
};

BOOST_FIXTURE_TEST_SUITE(combine_orbit, CombineOrbitFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb1);

  BOOST_CHECK_CLOSE(corb->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(corb->max_time().pgs(), 200.0, 1e-6);

  // Query orbit data in range
  Time t_query = Time::time_pgs(150.0);
  boost::shared_ptr<OrbitData> od = corb->orbit_data(t_query);
  BOOST_CHECK(od);

  // Verify position matches underlying orbit
  boost::shared_ptr<CartesianInertial> pos_combined = od->position_ci();
  boost::shared_ptr<CartesianInertial> pos_original = orb1->position_ci(t_query);
  BOOST_CHECK_CLOSE(pos_combined->position[0], pos_original->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_combined->position[1], pos_original->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_combined->position[2], pos_original->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(disjoint_orbits)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb1);  // [100, 200)
  corb->add_orbit(orb2);  // [300, 400)
  corb->add_orbit(orb3);  // [500, 600)

  BOOST_CHECK_CLOSE(corb->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(corb->max_time().pgs(), 600.0, 1e-6);

  // Query in first orbit
  boost::shared_ptr<OrbitData> od1 = corb->orbit_data(Time::time_pgs(150.0));
  BOOST_CHECK(od1);

  // Query in second orbit
  boost::shared_ptr<OrbitData> od2 = corb->orbit_data(Time::time_pgs(350.0));
  BOOST_CHECK(od2);

  // Query in third orbit
  boost::shared_ptr<OrbitData> od3 = corb->orbit_data(Time::time_pgs(550.0));
  BOOST_CHECK(od3);

  // Query in gap - should throw exception
  try {
    corb->orbit_data(Time::time_pgs(250.0));
    BOOST_FAIL("Should have thrown exception for time in gap");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }

  // Query before min_time - should throw exception
  try {
    corb->orbit_data(Time::time_pgs(50.0));
    BOOST_FAIL("Should have thrown exception for time before min");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }

  // Query after max_time - should throw exception
  try {
    corb->orbit_data(Time::time_pgs(650.0));
    BOOST_FAIL("Should have thrown exception for time after max");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(overlapping_orbits)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb_overlap1);  // [100, 200)
  corb->add_orbit(orb_overlap2);  // [150, 250)
  corb->add_orbit(orb_overlap3);  // [200, 300)

  BOOST_CHECK_CLOSE(corb->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(corb->max_time().pgs(), 300.0, 1e-6);

  // Query in overlap region [150, 200) - should prefer orbit with earliest min_time (orb_overlap1)
  Time t_overlap = Time::time_pgs(175.0);
  boost::shared_ptr<OrbitData> od_overlap = corb->orbit_data(t_overlap);
  boost::shared_ptr<CartesianInertial> pos_overlap = od_overlap->position_ci();
  boost::shared_ptr<CartesianInertial> pos_orb1 = orb_overlap1->position_ci(t_overlap);
  BOOST_CHECK_CLOSE(pos_overlap->position[0], pos_orb1->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_overlap->position[1], pos_orb1->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_overlap->position[2], pos_orb1->position[2], 1e-6);

  // Query at exact boundary (200) - should use orb_overlap3
  Time t_boundary = Time::time_pgs(200.0);
  boost::shared_ptr<OrbitData> od_boundary = corb->orbit_data(t_boundary);
  boost::shared_ptr<CartesianInertial> pos_boundary = od_boundary->position_ci();
  boost::shared_ptr<CartesianInertial> pos_orb3 = orb_overlap3->position_ci(t_boundary);
  BOOST_CHECK_CLOSE(pos_boundary->position[0], pos_orb3->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_boundary->position[1], pos_orb3->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_boundary->position[2], pos_orb3->position[2], 1e-6);

  // Query in non-overlapping region of orb_overlap2 [200, 250)
  Time t_middle = Time::time_pgs(225.0);
  boost::shared_ptr<OrbitData> od_middle = corb->orbit_data(t_middle);
  boost::shared_ptr<CartesianInertial> pos_middle = od_middle->position_ci();
  boost::shared_ptr<CartesianInertial> pos_orb2 = orb_overlap2->position_ci(t_middle);
  BOOST_CHECK_CLOSE(pos_middle->position[0], pos_orb2->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_middle->position[1], pos_orb2->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_middle->position[2], pos_orb2->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(add_order_independence)
{
  // Add orbits in non-sorted order
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb2);  // [300, 400) - added first
  corb->add_orbit(orb1);  // [100, 200) - added second (earlier)
  corb->add_orbit(orb3);  // [500, 600) - added third

  // Should still work correctly due to lazy sorting
  BOOST_CHECK_CLOSE(corb->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(corb->max_time().pgs(), 600.0, 1e-6);

  // Query each orbit
  boost::shared_ptr<OrbitData> od1 = corb->orbit_data(Time::time_pgs(150.0));
  boost::shared_ptr<OrbitData> od2 = corb->orbit_data(Time::time_pgs(350.0));
  boost::shared_ptr<OrbitData> od3 = corb->orbit_data(Time::time_pgs(550.0));
  BOOST_CHECK(od1);
  BOOST_CHECK(od2);
  BOOST_CHECK(od3);
}

BOOST_AUTO_TEST_CASE(cache_effectiveness)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb1);
  corb->add_orbit(orb2);

  // Query same orbit multiple times
  Time t = Time::time_pgs(150.0);
  for(int i = 0; i < 10; ++i) {
    boost::shared_ptr<OrbitData> od = corb->orbit_data(t);
    BOOST_CHECK(od);
  }

  // Query consecutive times in same orbit
  for(double t_pgs = 110.0; t_pgs < 190.0; t_pgs += 10.0) {
    boost::shared_ptr<OrbitData> od = corb->orbit_data(Time::time_pgs(t_pgs));
    BOOST_CHECK(od);
  }
}

BOOST_AUTO_TEST_CASE(overlap_constraints)
{
  // Test case: orbit with 2 overlaps at its max_time (should fail)
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb_overlap1);  // [100, 200)
  corb->add_orbit(orb_overlap2);  // [150, 250)

  // Try to add orbit that would create 2 overlaps at orb_overlap2's max_time
  // orb_bad1 is [160, 280), which overlaps orb_overlap2 at [160, 250)
  // But orb_overlap2 already has an overlap at its min_time with orb_overlap1
  // So adding orb_bad1 would give orb_overlap2 two overlaps at different ends (which is allowed)
  // But orb_bad1 would overlap orb_overlap2's max_time region, and we need to check
  // if that violates the constraint

  // Actually, let me reconsider the constraint:
  // Each orbit can have at most 1 overlap at min_time and 1 at max_time
  // orb_overlap2 [150, 250):
  //   - has overlap with orb_overlap1 at min_time [150, 200)
  //   - can still have overlap at max_time
  // If we add orb_bad1 [160, 280):
  //   - It overlaps orb_overlap2 at [160, 250) - this is at orb_overlap2's max_time
  //   - So this should be OK as the first max_time overlap

  // Let me create a case that actually violates the constraint
  // Need orbit that creates a second overlap at the same end

  // Better test: Create a situation where an orbit already has an overlap at its max_time,
  // then try to add another that also overlaps there
  boost::shared_ptr<CombineOrbit> corb2(new CombineOrbit());
  corb2->add_orbit(orb_overlap1);  // [100, 200)
  corb2->add_orbit(orb_overlap3);  // [200, 300) - overlaps orb_overlap1 at its max_time

  // Now try to add another orbit that overlaps orb_overlap1's max_time
  // This requires an orbit like [190, 290) which would overlap [190, 200) with orb_overlap1
  boost::shared_ptr<KeplerOrbit> orb_bad2(new KeplerOrbit(Time::time_pgs(190.0), Time::time_pgs(290.0)));

  try {
    corb2->add_orbit(orb_bad2);
    BOOST_FAIL("Should have thrown exception for constraint violation");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(time_with_derivative)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb1);

  Time t = Time::time_pgs(150.0);
  TimeWithDerivative td = TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));

  boost::shared_ptr<OrbitData> od = corb->orbit_data(td);
  BOOST_CHECK(od);

  // Verify position matches
  boost::shared_ptr<CartesianInertial> pos_td = od->position_ci();
  boost::shared_ptr<CartesianInertial> pos_t = orb1->position_ci(t);
  BOOST_CHECK_CLOSE(pos_td->position[0], pos_t->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_td->position[1], pos_t->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_td->position[2], pos_t->position[2], 1e-6);

  // Test derivative calculation (similar to other orbit tests)
  double eps = 1e-3;
  boost::shared_ptr<CartesianInertial> p0 = corb->position_ci(td.value());
  boost::shared_ptr<CartesianInertial> p1 = corb->position_ci(td.value() + eps);
  Array<double, 2> jac_fd(3, 1);
  jac_fd(0, 0) = (p1->position[0] - p0->position[0]) / eps;
  jac_fd(1, 0) = (p1->position[1] - p0->position[1]) / eps;
  jac_fd(2, 0) = (p1->position[2] - p0->position[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  boost::array<AutoDerivative<double>, 3> p2 = corb->position_ci_with_derivative(td);
  jac_calc(0, Range::all()) = p2[0].gradient();
  jac_calc(1, Range::all()) = p2[1].gradient();
  jac_calc(2, Range::all()) = p2[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(empty_orbit_list)
{
  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());

  // Query on empty list should throw exception
  try {
    corb->orbit_data(Time::time_pgs(150.0));
    BOOST_FAIL("Should have thrown exception for empty orbit list");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;

  boost::shared_ptr<CombineOrbit> corb(new CombineOrbit());
  corb->add_orbit(orb1);
  corb->add_orbit(orb2);
  corb->add_orbit(orb3);

  std::string d = serialize_write_string(corb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<CombineOrbit> corb_restored = serialize_read_string<CombineOrbit>(d);

  // Check min/max time
  BOOST_CHECK_CLOSE(corb_restored->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(corb_restored->max_time().pgs(), 600.0, 1e-6);

  // Check orbit_data queries work
  Time t1_query = Time::time_pgs(150.0);
  boost::shared_ptr<OrbitData> od1 = corb_restored->orbit_data(t1_query);
  boost::shared_ptr<CartesianInertial> pos1 = od1->position_ci();
  boost::shared_ptr<CartesianInertial> pos1_expected = orb1->position_ci(t1_query);
  BOOST_CHECK_CLOSE(pos1->position[0], pos1_expected->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos1->position[1], pos1_expected->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos1->position[2], pos1_expected->position[2], 1e-6);

  Time t2_query = Time::time_pgs(350.0);
  boost::shared_ptr<OrbitData> od2 = corb_restored->orbit_data(t2_query);
  boost::shared_ptr<CartesianInertial> pos2 = od2->position_ci();
  boost::shared_ptr<CartesianInertial> pos2_expected = orb2->position_ci(t2_query);
  BOOST_CHECK_CLOSE(pos2->position[0], pos2_expected->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos2->position[1], pos2_expected->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos2->position[2], pos2_expected->position[2], 1e-6);
}

BOOST_AUTO_TEST_SUITE_END()
