#include "unit_test_support.h"
#include "mspi_gimbal.h"

#include "constant.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_gimbal, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiGimbal gim(test_data_dir() + "AIRMSPI_CONFIG_CAMERA_MODEL_0003.config");
  BOOST_CHECK_CLOSE(gim.epsilon(), 1.250, 1e-8);
  BOOST_CHECK_CLOSE(gim.psi(), 0.000, 1e-8);
  BOOST_CHECK_CLOSE(gim.theta(), 0.059, 1e-8);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<MspiGimbal> gim(new MspiGimbal(test_data_dir() + 
		    "AIRMSPI_CONFIG_CAMERA_MODEL_0003.config"));
  std::string d = serialize_write_string(gim);
  if(false)
    std::cerr << d;
  boost::shared_ptr<MspiGimbal> gimr = serialize_read_string<MspiGimbal>(d);
  BOOST_CHECK_CLOSE(gimr->epsilon(), 1.250, 1e-8);
  BOOST_CHECK_CLOSE(gimr->psi(), 0.000, 1e-8);
  BOOST_CHECK_CLOSE(gimr->theta(), 0.059, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
