// This creates the "main()" needed to run all the tests. This
// automatically done by boost, we just provide one file to do it with.

#define BOOST_TEST_MODULE "GeoCal Unit Tests"
#include <boost/test/included/unit_test.hpp>

// Also set up the serialization stuff we need for testing. This
// *has* to be done once - not in each test. So we do that here.
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "geocal_serialize.h"
