#include "unit_test_support.h"
#include "location_to_file.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(location_to_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
    LocationToFile lf;
    lf.add(1,1,3,4,"file1");
    lf.add(3,4,5,6,"file2");
    lf.add(7,7,8,9,"file3");
    BOOST_CHECK_EQUAL(lf.find(1,1), "file1");
    BOOST_CHECK_EQUAL(lf.find(3,4), "file2");
    BOOST_CHECK_EQUAL(lf.find(5,4), "");
    BOOST_CHECK_EQUAL(lf.find(6,7), "");
    BOOST_CHECK_EQUAL(lf.find(7,7), "file3");
}

BOOST_AUTO_TEST_SUITE_END()
