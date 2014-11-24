#include "unit_test_support.h"
#include "image_coordinate.h"
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(image_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  BOOST_CHECK(ic ==ic && ic !=ic2);
  BOOST_CHECK_EQUAL(ic.print_to_string(), "(1, 2)");
}

BOOST_AUTO_TEST_CASE(vicar_image_coordinate)
{
  VicarImageCoordinate ic(1, 2);
  VicarImageCoordinate ic2(2, 3);
  BOOST_CHECK(ic ==ic && ic !=ic2);
  BOOST_CHECK_EQUAL(ic.print_to_string(), "VICAR image coordinate (1, 2)");
  ImageCoordinate ic3(ic);
  ImageCoordinate ic3expect(0, 1);
  VicarImageCoordinate ic4(ImageCoordinate(4, 5));
  VicarImageCoordinate ic4expect(5, 6);
  BOOST_CHECK(ic3 ==ic3expect);
  BOOST_CHECK(ic4 ==ic4expect);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  oa << BOOST_SERIALIZATION_NVP(ic) << BOOST_SERIALIZATION_NVP(ic2);
  //std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  ia >> boost::serialization::make_nvp("ic", icr) 
     >> boost::serialization::make_nvp("ic2", ic2r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
  
}

BOOST_AUTO_TEST_CASE(serialization_text)
{
  // Don't normally need a separate text serialization test, but this
  // is our first serialization so make sure everything works ok.
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);

  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  oa << BOOST_SERIALIZATION_NVP(ic) << BOOST_SERIALIZATION_NVP(ic2);
  //std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::text_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  ia >> boost::serialization::make_nvp("ic", icr) 
     >> boost::serialization::make_nvp("ic2", ic2r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
}

BOOST_AUTO_TEST_SUITE_END()
