#include "unit_test_support.h"
#include "mspi_paraxial_transform.h"
#include <iostream>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_paraxial_transform, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiParaxialTransform p(test_data_dir() + "mspi_camera_test.config");
  double real_x, real_y;
  p.paraxial_to_real(32, 0.1, -0.2, real_x, real_y);
  BOOST_CHECK_CLOSE(real_x, 1.05852897147680050094e-01, 1e-8);
  BOOST_CHECK_CLOSE(real_y, -1.98878552619107251687e-01, 1e-8);
  double paraxial_x, paraxial_y;
  p.real_to_paraxial(32, real_x, real_y, paraxial_x, paraxial_y);
  BOOST_CHECK(fabs(paraxial_x -  0.1) < 0.01);
  BOOST_CHECK(fabs(paraxial_y - -0.2) < 0.01);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<MspiParaxialTransform> p
    (new MspiParaxialTransform(test_data_dir() + "mspi_camera_test.config"));
  std::string d = serialize_write_string(p);
  if(false)
    std::cerr << d;
  boost::shared_ptr<MspiParaxialTransform> pr =
    serialize_read_string<MspiParaxialTransform>(d);
  double real_x, real_y;
  pr->paraxial_to_real(32, 0.1, -0.2, real_x, real_y);
  BOOST_CHECK_CLOSE(real_x, 1.05852897147680050094e-01, 1e-8);
  BOOST_CHECK_CLOSE(real_y, -1.98878552619107251687e-01, 1e-8);
  double paraxial_x, paraxial_y;
  pr->real_to_paraxial(32, real_x, real_y, paraxial_x, paraxial_y);
  BOOST_CHECK(fabs(paraxial_x -  0.1) < 0.01);
  BOOST_CHECK(fabs(paraxial_y - -0.2) < 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
