#include "mspi_config_file.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_config_file, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test.txt");
  BOOST_CHECK_CLOSE(config.value<double>("parameter1"), 1.1, 1e-8);
  blitz::Array<double, 1> p2_expect(2);
  p2_expect = 2.1, -3.1;
  blitz::Array<double, 1> p2 = 
    config.value<blitz::Array<double, 1> >("parameter2");
  BOOST_CHECK_MATRIX_CLOSE(p2_expect, p2);
  blitz::Array<int, 1> p3_expect(3);
  p3_expect = 4, -5, 6;
  blitz::Array<int, 1> p3 = 
    config.value<blitz::Array<int, 1> >("parameter3");
  BOOST_CHECK_MATRIX_CLOSE(p3_expect, p3);
  blitz::Array<bool, 2> p4 =
    config.value<blitz::Array<bool, 2> >("parameter4");
  blitz::Array<bool, 2> p4_expect(2, 6);
  p4_expect = 
    true, false, true, false, true, false,
    true, false, true, false, true, false;
  BOOST_CHECK(p4_expect.rows() == p4.rows());
  BOOST_CHECK(p4_expect.cols() == p4.cols());
  for(int i = 0; i < p4.rows(); ++i)
    for(int j = 0; j < p4.cols(); ++j)
      BOOST_CHECK(p4(i,j) == p4_expect(i,j));
  BOOST_CHECK_CLOSE(config.value<double>("parameter6"), 1.00000001, 1e-8);
  std::vector<std::string> p7 = config.value<std::vector<std::string> >("parameter7");
  BOOST_CHECK(p7[0] == "alpha");
  BOOST_CHECK(p7[1] == "beta");
  BOOST_CHECK(p7[2] == "gamma");
  blitz::Array<int, 2> a1 = config.value<blitz::Array<int, 2> >("array1");
  blitz::Array<int, 2> a1_expect(3, 4);
  a1_expect = 
    100, 101, 102, 103,
    200, 201, 202, 203,
    300, 301, 302, 303;
  BOOST_CHECK_MATRIX_CLOSE(a1, a1_expect);


  // Test adding to file
  BOOST_CHECK_CLOSE(config.value<double>("parameter6"), 1.00000001, 1e-8);
  BOOST_CHECK(!config.have_key("parameter10"));
  config.add_file(test_data_dir() + "mspi_config_file_test2.txt");
  BOOST_CHECK_CLOSE(config.value<double>("parameter6"), 2.00000001, 1e-8);
  BOOST_CHECK_EQUAL(config.value<int>("parameter10"), 3);
  BOOST_CHECK_EQUAL(config.value<int>("parameter10-11"), 4);
}

BOOST_AUTO_TEST_CASE(table_test)
{
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test.txt");
  config.add_file(test_data_dir() + "mspi_config_file_test2.txt");
  MspiConfigTable tab(config, "L1B1");
  BOOST_CHECK_EQUAL(tab.value<int>(0, "view_number"), 6);
  BOOST_CHECK_EQUAL(tab.value<int>(1, "view_number"), 7);
  BOOST_CHECK_EQUAL(tab.value<std::string>(0, "l1b1_file"), "file1.hdf");
  BOOST_CHECK_EQUAL(tab.value<std::string>(1, "l1b1_file"), "file2.hdf");
}

BOOST_AUTO_TEST_CASE(basic_test2)
{
  // This is a second configuration file that we had trouble reading
  // at one point. Have this test in place to illustrate the problem
  // and that we have fixed it.
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test3.txt");
  BOOST_CHECK(config.value<int>("last_unshielded") == 1434);
  BOOST_CHECK(config.value<std::string>("epoch") == "1601-01-01T:00:00:00:000000Z");
}

BOOST_AUTO_TEST_CASE(base_version_override)
{
  // Test having a file with a "base_version" entry, for doing
  // cascading configuration files.
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test4.txt");
  BOOST_CHECK(config.value<int>("polarization_angle_Q") == 100);
  BOOST_CHECK(config.value<int>("last_unshielded") == 1434);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<MspiConfigFile> config(new MspiConfigFile(test_data_dir() + "mspi_config_file_test.txt"));
  std::string d = serialize_write_string(config);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<MspiConfigFile> config2 = 
    serialize_read_string<MspiConfigFile>(d);
  BOOST_CHECK_CLOSE(config2->value<double>("parameter1"), 1.1, 1e-8);
}

BOOST_AUTO_TEST_CASE(serialization_table)
{
  if(!have_serialize_supported())
    return;
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test.txt");
  config.add_file(test_data_dir() + "mspi_config_file_test2.txt");
  boost::shared_ptr<MspiConfigTable> tab(new MspiConfigTable(config, "L1B1"));
  std::string d = serialize_write_string(tab);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<MspiConfigTable> tab2 = 
    serialize_read_string<MspiConfigTable>(d);

  BOOST_CHECK_EQUAL(tab2->value<int>(0, "view_number"), 6);
  BOOST_CHECK_EQUAL(tab2->value<int>(1, "view_number"), 7);
  BOOST_CHECK_EQUAL(tab2->value<std::string>(0, "l1b1_file"), "file1.hdf");
  BOOST_CHECK_EQUAL(tab2->value<std::string>(1, "l1b1_file"), "file2.hdf");

}

BOOST_AUTO_TEST_SUITE_END()
