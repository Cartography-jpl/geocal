#include "global_fixture.h"
#include <boost/test/unit_test.hpp>
#include <cstdlib>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Setup for all unit tests.
//-----------------------------------------------------------------------

GlobalFixture::GlobalFixture() 
{
  set_default_value();
  // Some environment variables various unit tests depend on.
  setenv("PGS_PC_INFO_FILE", (test_data_dir() + "PCF").c_str(), 1);
  setenv("SPICEDATA", (test_data_dir() + "spice").c_str(), 1);
}

//-----------------------------------------------------------------------
/// Directory where test data is. This already includes the trailing
/// slash, so you can just do test_data_data() + "foo.txt" in your 
/// unit tests.
//-----------------------------------------------------------------------

std::string GlobalFixture::test_data_dir() const
{
  char* srcdir = getenv("abs_top_srcdir");
  // This should get set in set_default_value, but just in case
  // something odd happens print an error message.
  if(!srcdir)
    BOOST_FAIL("To run this test, you must set the 'abs_top_srcdir' environment\n"
               "variable to the top of the source tree. This is automatically\n"
               "done if you are running 'make check', but you need to\n"

               "running in a debugger");
  return std::string(srcdir) + "/src/geocal/unit_test_data/";
}
