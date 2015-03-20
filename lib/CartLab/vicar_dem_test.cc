#include "unit_test_support.h"
#include "vicar_dem.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(vicar_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  VicarDem d(fname);
  Geodetic g1(35.9, 44.800, 100);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 1.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  if(!VicarFile::vicar_available())
    return;
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  std::string fname = test_data_dir() + "vicar.img";
  boost::shared_ptr<Dem> d(new VicarDem(fname));
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Dem> dr;
  ia >> GEOCAL_NVP(dr);

  Geodetic g1(35.9, 44.800, 100);
  BOOST_CHECK_CLOSE(dr->height_reference_surface(g1), 1.0, 1e-4);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
