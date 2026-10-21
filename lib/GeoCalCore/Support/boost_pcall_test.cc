#include "unit_test_support.h"
#include "boost_pcall.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(boost_pcall, GlobalFixture)

// Uses standard unix utilities (cat, true, false), so this doesn't depend
// on anything geocal installs.

BOOST_AUTO_TEST_CASE(round_trip)
{
  // Binary data, including embedded nulls and a larger than pipe buffer
  // size, to make sure we handle all the data.
  std::string in;
  for(int i = 0; i < 1000000; ++i)
    in += (char) (i % 256);
  std::string out;
  boost_pcall_run("cat", &in, &out);
  BOOST_CHECK(in == out);
}

BOOST_AUTO_TEST_CASE(arguments)
{
  std::string in = "abc";
  std::string out;
  // Arguments are split on whitespace, "-" is cat's stdin
  boost_pcall_run("cat   -", &in, &out);
  BOOST_CHECK_EQUAL(out, "abc");
}

BOOST_AUTO_TEST_CASE(output_only)
{
  std::string out;
  boost_pcall_run("echo hello", 0, &out);
  BOOST_CHECK_EQUAL(out, "hello\n");
}

BOOST_AUTO_TEST_CASE(input_only)
{
  // dd discards stdin and writes nothing to stdout, so this is quiet.
  std::string in(1000000, 'x');
  BOOST_CHECK_NO_THROW(boost_pcall_run("dd of=/dev/null status=none", &in, 0));
}

BOOST_AUTO_TEST_CASE(failure)
{
  std::string out;
  BOOST_CHECK_THROW(boost_pcall_run("false", 0, &out), Exception);
  BOOST_CHECK_THROW(boost_pcall_run("", 0, &out), Exception);
  BOOST_CHECK_THROW(boost_pcall_run("not_a_real_program_xyz", 0, &out),
		    Exception);
}

BOOST_AUTO_TEST_SUITE_END()
