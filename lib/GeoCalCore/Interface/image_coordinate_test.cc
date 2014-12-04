#include "unit_test_support.h"
#include "image_coordinate.h"
#include <fstream>
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
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  boost::shared_ptr<ImageCoordinate> ic3(new ImageCoordinate(4, 5));
  boost::shared_ptr<GenericObject> ic4(new ImageCoordinate(6, 7));
  oa << GEOCAL_NVP(ic) << GEOCAL_NVP(ic2)
     << GEOCAL_NVP(ic3) << GEOCAL_NVP(ic4);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  boost::shared_ptr<ImageCoordinate> ic3r;
  boost::shared_ptr<GenericObject> ic4r;
  ia >> GEOCAL_NVP(icr) >> GEOCAL_NVP(ic2r)
     >> GEOCAL_NVP(ic3r) >> GEOCAL_NVP(ic4r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
  BOOST_CHECK(*ic3 ==*ic3r);
  boost::shared_ptr<ImageCoordinate> ic4r2 =
    boost::dynamic_pointer_cast<ImageCoordinate>(ic4r);
  BOOST_CHECK(*ic4r2 == ImageCoordinate(6,7));
#endif
}

BOOST_AUTO_TEST_CASE(serialization_save)
{
#ifdef HAVE_BOOST_SERIALIZATON
  // Test where we write out the XML file, and then read this in 
  // a separate test. We use this to make sure the registration occurs
  // correctly for this, and format doesn't change.
  //
  // We write this out using a the base class "GenericObject", just so
  // we can test proper handling of derived classes. We wouldn't
  // normally use this general of a class type.
  //
  // We only need to run this test to regenerate the xml code, the
  // load test can then run every time.
  if(true)
    return;
  std::string fname = test_data_dir() + "image_coordinate_test.xml";
  std::ofstream os(fname.c_str());
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<GenericObject> ic1(new ImageCoordinate(4, 5));
  boost::shared_ptr<GenericObject> ic2(new ImageCoordinate(6, 7));
  oa << GEOCAL_NVP(ic1) << GEOCAL_NVP(ic2);
#endif
}

BOOST_AUTO_TEST_CASE(serialization_load)
{
#ifdef HAVE_BOOST_SERIALIZATON
  std::string fname = test_data_dir() + "image_coordinate_test.xml";
  std::ifstream is(fname.c_str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> ic1r;
  boost::shared_ptr<GenericObject> ic2r;
  ia >> GEOCAL_NVP(ic1r) >> GEOCAL_NVP(ic2r);
  boost::shared_ptr<ImageCoordinate> ic1 = 
    boost::dynamic_pointer_cast<ImageCoordinate>(ic1r);
  boost::shared_ptr<ImageCoordinate> ic2 = 
    boost::dynamic_pointer_cast<ImageCoordinate>(ic2r);
  BOOST_CHECK(*ic1 ==ImageCoordinate(4, 5));
  BOOST_CHECK(*ic2 == ImageCoordinate(6,7));
#endif
}

BOOST_AUTO_TEST_CASE(serialization_text)
{
#ifdef HAVE_BOOST_SERIALIZATON
  // Don't normally need a separate text serialization test, but this
  // is our first serialization so make sure everything works ok.
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);

  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  oa << GEOCAL_NVP(ic) << GEOCAL_NVP(ic2);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::text_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  ia >> GEOCAL_NVP(icr) >> GEOCAL_NVP(ic2r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
