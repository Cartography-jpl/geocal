// These inclusions are not generally needed to test serialization,
// but we have extra tests for this particular class since we used
// this for developing the serialization code
#include <boost/archive/polymorphic_text_iarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include "geocal_serialize_support.h"

#include "unit_test_support.h"
#include "image_coordinate.h"
#include <fstream>
using namespace GeoCal;

// Create a new object to serialize, and make sure this gets
// registered in.
namespace GeoCal {
class FakeClass : public GeoCal::GenericObject {
public:
  FakeClass(int I1, int I2) : i1(I1), i2(I2) {}
  virtual ~FakeClass() {}
  int i1, i2;
private:
  FakeClass() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(FakeClass);

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

BOOST_AUTO_TEST_CASE(serialize_function_file)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  // Test the generic interface for reading and writing a file. Note
  // that most unit tests don't need to do this, we just have this
  // here since ImageCoordinate is our first serialization test and we
  // want to check everything.
  boost::shared_ptr<ImageCoordinate> ic(new ImageCoordinate(10, 20));
  serialize_write("image_coordinate_test.xml", ic);
  boost::shared_ptr<ImageCoordinate> ic2 = 
    serialize_read<ImageCoordinate>("image_coordinate_test.xml");
  BOOST_CHECK(*ic == *ic2);
  int status = unlink("image_coordinate_test.xml");
  // Ignore status, ok if deleting fails.
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
  // This is the form that most classes should have for testing
  // serialization.
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  boost::shared_ptr<ImageCoordinate> ic(new ImageCoordinate(10, 20));
  std::string d = serialize_write_string(ic);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<ImageCoordinate> ic2 = 
    serialize_read_string<ImageCoordinate>(d);
  BOOST_CHECK(*ic == *ic2);
#endif  
}

BOOST_AUTO_TEST_CASE(serialization_new_class)
{
  // Check that we can register and use a class defined outside of
  // the GeoCal library
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  boost::shared_ptr<FakeClass> fc(new FakeClass(10, 20));
  std::string d = serialize_write_string(fc);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<FakeClass> fc2 = 
    serialize_read_string<FakeClass>(d);
  BOOST_CHECK_EQUAL(fc->i1, fc2->i1);
  BOOST_CHECK_EQUAL(fc->i2, fc2->i2);
#endif  
}

BOOST_AUTO_TEST_CASE(serialization2)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ostringstream os;
  boost::archive::polymorphic_xml_oarchive oa(os);

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
  boost::archive::polymorphic_xml_iarchive ia(is);
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
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
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
  boost::archive::polymorphic_xml_oarchive oa(os);

  boost::shared_ptr<GenericObject> ic1(new ImageCoordinate(4, 5));
  boost::shared_ptr<GenericObject> ic2(new ImageCoordinate(6, 7));
  oa << GEOCAL_NVP(ic1) << GEOCAL_NVP(ic2);
#endif
}

BOOST_AUTO_TEST_CASE(serialization_load)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::string fname = test_data_dir() + "image_coordinate_test.xml";
  std::ifstream is(fname.c_str());
  boost::archive::polymorphic_xml_iarchive ia(is);
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
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  // Don't normally need a separate text serialization test, but this
  // is our first serialization so make sure everything works ok.
  std::ostringstream os;
  boost::archive::polymorphic_text_oarchive oa(os);

  ImageCoordinate ic(1, 2);
  ImageCoordinate ic2(2, 3);
  oa << GEOCAL_NVP(ic) << GEOCAL_NVP(ic2);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << os.str();
  std::istringstream is(os.str());
  boost::archive::polymorphic_text_iarchive ia(is);
  ImageCoordinate icr, ic2r;
  ia >> GEOCAL_NVP(icr) >> GEOCAL_NVP(ic2r);
  BOOST_CHECK(ic ==icr);
  BOOST_CHECK(ic2 ==ic2r);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
