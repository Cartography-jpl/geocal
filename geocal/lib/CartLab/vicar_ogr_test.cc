#include "unit_test_support.h"
#include "vicar_ogr.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(vicar_ogr, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  if(!VicarFile::vicar_available())
    return;
  VicarOgr v;
  boost::shared_ptr<OgrCoordinateConverter> 
    cv(new OgrCoordinateConverter(OgrWrapper::from_epsg(32612)));
  MapInfo mi(cv, 10000, 20000, 10000 + 100 * 20, 20000 + 200 * 30, 100, 200);
  {
    VicarFile f("test_out/vicar_ogr_write.img", 200, 100);
    v.to_vicar(mi, f);
  } // Destructor forces file to be flushed.
  VicarFile f2("test_out/vicar_ogr_write.img");
  MapInfo mi2 = v.from_vicar(f2);
  OgrCoordinateConverter& ogr = *(dynamic_cast<OgrCoordinateConverter*>(mi.coordinate_converter_ptr().get()));
  BOOST_CHECK(ogr.ogr().ogr().IsSame(&cv->ogr().ogr()));
}

BOOST_AUTO_TEST_SUITE_END()
