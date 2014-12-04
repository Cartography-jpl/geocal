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
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<GenericObject> p(new MspiParaxialTransform(test_data_dir() + "mspi_camera_test.config"));
  oa << GEOCAL_NVP(p);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> pr;
  ia >> GEOCAL_NVP(pr);
  boost::shared_ptr<MspiParaxialTransform> pr2 =
    boost::dynamic_pointer_cast<MspiParaxialTransform>(pr);

  double real_x, real_y;
  pr2->paraxial_to_real(32, 0.1, -0.2, real_x, real_y);
  BOOST_CHECK_CLOSE(real_x, 1.05852897147680050094e-01, 1e-8);
  BOOST_CHECK_CLOSE(real_y, -1.98878552619107251687e-01, 1e-8);
  double paraxial_x, paraxial_y;
  pr2->real_to_paraxial(32, real_x, real_y, paraxial_x, paraxial_y);
  BOOST_CHECK(fabs(paraxial_x -  0.1) < 0.01);
  BOOST_CHECK(fabs(paraxial_y - -0.2) < 0.01);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
