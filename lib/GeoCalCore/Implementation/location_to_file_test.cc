#include "unit_test_support.h"
#include "location_to_file.h"
#include <algorithm>

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
    std::vector<std::string> flist = lf.find_region(2,2,2,3);
    // Sort so we can test files w/o worrying about order they are returned.
    std::sort(flist.begin(), flist.end());
    BOOST_CHECK_EQUAL((int) flist.size(), 2);
    BOOST_CHECK_EQUAL(flist[0], "file1");
    BOOST_CHECK_EQUAL(flist[1], "file2");
    flist = lf.find_region(7,4,1,1);
    BOOST_CHECK_EQUAL((int) flist.size(), 0);
    flist = lf.find_region(2,3,10,10);
    std::sort(flist.begin(), flist.end());
    BOOST_CHECK_EQUAL((int) flist.size(), 3);
    BOOST_CHECK_EQUAL(flist[0], "file1");
    BOOST_CHECK_EQUAL(flist[1], "file2");
    BOOST_CHECK_EQUAL(flist[2], "file3");
}

BOOST_AUTO_TEST_SUITE_END()
