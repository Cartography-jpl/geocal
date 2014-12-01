#include "unit_test_support.h"
#include "image_coordinate.h"
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
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
  boost::shared_ptr<ImageCoordinate> ic3(new ImageCoordinate(4, 5));
  boost::shared_ptr<GenericObject> ic4(new ImageCoordinate(6, 7));
  oa << BOOST_SERIALIZATION_NVP(ic) << BOOST_SERIALIZATION_NVP(ic2)
     << BOOST_SERIALIZATION_NVP(ic3) << BOOST_SERIALIZATION_NVP(ic4);
  std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  boost::shared_ptr<ImageCoordinate> ic3r;
  boost::shared_ptr<GenericObject> ic4r;
  ia >> BOOST_SERIALIZATION_NVP(icr) >> BOOST_SERIALIZATION_NVP(ic2r)
     >> BOOST_SERIALIZATION_NVP(ic3r) >> BOOST_SERIALIZATION_NVP(ic4r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
  BOOST_CHECK(*ic3 ==*ic3r);
  boost::shared_ptr<ImageCoordinate> ic4r2 =
    boost::dynamic_pointer_cast<ImageCoordinate>(ic4r);
  BOOST_CHECK(*ic4r2 == ImageCoordinate(6,7));
}

BOOST_AUTO_TEST_CASE(serialization_save)
{
  // Test where we write out the XML file, and then read this in 
  // a separate test. We use this to make sure the registration occurs
  // correctly for this.
  std::ofstream os("image_coordinate_test.xml");
  boost::archive::xml_oarchive oa(os);

  //  boost::shared_ptr<ImageCoordinate> ic1(new ImageCoordinate(4, 5));
  // boost::shared_ptr<ImageCoordinate> ic2(new ImageCoordinate(6,
  // 7));
  std::map<std::string, boost::shared_ptr<GenericObject> > m;
  m["ic1"] = boost::shared_ptr<GenericObject>(new ImageCoordinate(4, 5));
  m["ic2"] = boost::shared_ptr<GenericObject>(new ImageCoordinate(6, 7));
  oa << BOOST_SERIALIZATION_NVP(m);
}

BOOST_AUTO_TEST_CASE(serialization_load)
{
  // std::ostringstream os;
  // boost::archive::xml_oarchive oa(os);
  // boost::shared_ptr<ImageCoordinate> dummy(new ImageCoordinate(10, 20));
  // oa << BOOST_SERIALIZATION_NVP(dummy);

  std::ifstream is("image_coordinate_test.xml");
  boost::archive::xml_iarchive ia(is);
  // boost::shared_ptr<ImageCoordinate> ic1;
  // boost::shared_ptr<ImageCoordinate> ic2;
  boost::shared_ptr<GenericObject> ic1;
  boost::shared_ptr<GenericObject> ic2;
  std::map<std::string, boost::shared_ptr<GenericObject> > m;
  ia >> BOOST_SERIALIZATION_NVP(m);
  //BOOST_CHECK(*ic1 ==ImageCoordinate(4, 5));
  boost::shared_ptr<ImageCoordinate> ic2r =
    boost::dynamic_pointer_cast<ImageCoordinate>(m["ic2"]);
  BOOST_CHECK(*ic2r == ImageCoordinate(6,7));
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
