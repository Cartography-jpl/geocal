#include "air_mspi_file.h"
#include "unit_test_support.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <cstdlib>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::filesystem::path cp = boost::filesystem::current_path();
  boost::filesystem::path t = cp / "mspi_temp";
  // Make sure directory doesn't exist, we'll create this shortly
  boost::filesystem::remove_all(t);
  setenv("MSPI_TEMP_DIR", t.string().c_str(), 1);
  // Check that we handle being passed the permanent name and
  // returning it in canonical form.
  BOOST_CHECK_EQUAL(air_mspi_permanent_file_name("./foo.txt"),
		    (cp / "foo.txt").string());
  // Check that we handle being passed the local name and returning
  // the permanent name
  BOOST_CHECK_EQUAL(air_mspi_permanent_file_name((t / cp / "./foo.txt").string()),
		    (cp / "foo.txt").string());
  // Check that if file doesn't exist we return the permanent name
  BOOST_CHECK_EQUAL(air_mspi_local_file_name("./foo.txt"),
		    (cp / "foo.txt").string());
  boost::filesystem::create_directories(t / cp);
  std::ofstream f((t / cp / "foo.txt").string().c_str());
  f << "hi there\n";
  f.close();
  // Check that if file does exist we return the local file name
  BOOST_CHECK_EQUAL(air_mspi_local_file_name("./foo.txt"),
		    (t / cp / "foo.txt").string());
  // Clean up directory
  boost::filesystem::remove_all(t);
}

BOOST_AUTO_TEST_SUITE_END()
