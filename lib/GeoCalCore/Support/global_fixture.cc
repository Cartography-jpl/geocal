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

