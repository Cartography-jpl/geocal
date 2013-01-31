#include "unit_test_support.h"
#include "ogr_coordinate.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(ogr_coordinate, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
// This is UTM Zone 12N. You can look this code up online at 
// http://www.epsg-registry.org
  boost::shared_ptr<OgrWrapper> ogrw = OgrWrapper::from_epsg(32612);
  OgrCoordinate c(ogrw, 10000, 20000, 100);
  BOOST_CHECK_CLOSE(c.x, 10000.0, 1e-4);
  BOOST_CHECK_CLOSE(c.y, 20000.0, 1e-4);
  BOOST_CHECK_CLOSE(c.z, 100.0, 1e-4);
  BOOST_CHECK_EQUAL(ogrw.get(), c.ogr_ptr().get());
// We check the conversion by using the online tool available at 
// http://home.hiwaay.net/~taylorc/toolbox/geography/geoutm.html
  BOOST_CHECK_CLOSE(c.latitude(), 0.1804095808791086, 1e-4);
  BOOST_CHECK_CLOSE(c.longitude(), -115.3991701239089, 1e-4);
  BOOST_CHECK_CLOSE(c.height_reference_surface(), 100.0, 1e-4);
  Geodetic g(0.1804095808791086, -115.3991701239089, 100.0);
  OgrCoordinate c2(ogrw, g);
  BOOST_CHECK_CLOSE(c2.x, c.x, 1e-4);
  BOOST_CHECK_CLOSE(c2.y, c.y, 1e-4);
  BOOST_CHECK_CLOSE(c2.z, c.z, 1e-4);
// We check the conversion by using the online tool
// http://www.apsalin.com/convert-geodetic-to-universal-transverse-mercator.aspx.
  OgrCoordinate c3 = OgrCoordinate::to_utm(Geodetic(9, -7));
  BOOST_CHECK_CLOSE(c3.x, 719870.729943312, 1e-6);
  BOOST_CHECK_CLOSE(c3.y, 995452.6722696, 1e-6);
  BOOST_CHECK_CLOSE(c3.z, 0.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(ogr_wrapper)
{
  boost::shared_ptr<OgrWrapper> ogrw = OgrWrapper::from_epsg(32612);
  BOOST_CHECK_EQUAL(ogrw->projected_cs_type_geo_key(), std::string("32612"));
  BOOST_CHECK_EQUAL(ogrw->pcs_citation_geo_key(), 
		    std::string("WGS 84 / UTM zone 12N"));
  OgrWrapper ogrw2(ogrw->wkt());
  BOOST_CHECK_EQUAL(ogrw2.projected_cs_type_geo_key(), std::string("32612"));
  BOOST_CHECK_EQUAL(ogrw2.pcs_citation_geo_key(), 
		    std::string("WGS 84 / UTM zone 12N"));
}

BOOST_AUTO_TEST_CASE(ogr_coordinate_converter)
{
  boost::shared_ptr<OgrWrapper> ogrw = OgrWrapper::from_epsg(32612);
  OgrCoordinateConverter cv(ogrw);
  BOOST_CHECK_EQUAL(cv.ogr_ptr().get(), ogrw.get());
  boost::shared_ptr<GroundCoordinate> c = 
    cv.convert_from_coordinate(10000, 20000, 100);
  OgrCoordinate& cogr = dynamic_cast<OgrCoordinate&>(*c);
  BOOST_CHECK_CLOSE(cogr.x, 10000.0, 1e-8);
  BOOST_CHECK_CLOSE(cogr.y, 20000.0, 1e-8);
  BOOST_CHECK_CLOSE(cogr.z, 100.0, 1e-8);
  BOOST_CHECK_CLOSE(c->latitude(), 0.1804095808791086, 1e-4);
  BOOST_CHECK_CLOSE(c->longitude(), -115.3991701239089, 1e-4);
  BOOST_CHECK_CLOSE(c->height_reference_surface(), 100.0, 1e-4);
  Geodetic g(0.1804095808791086, -115.3991701239089, 100.0);
  double x, y, z;
  cv.convert_to_coordinate(g, x, y, z);
  BOOST_CHECK_CLOSE(x, 10000.0, 1e-4);
  BOOST_CHECK_CLOSE(y, 20000.0, 1e-4);
  BOOST_CHECK_CLOSE(z, 100.0, 1e-4);
  // Also UTM 12 N, but NAD83 instead of WGS84
  boost::shared_ptr<OgrWrapper> ogrw2 = OgrWrapper::from_epsg(2152);
  OgrCoordinateConverter cv2(ogrw2);
  GeodeticConverter cv3;
  BOOST_CHECK(cv.is_same(cv));
  BOOST_CHECK(cv2.is_same(cv2));
  BOOST_CHECK(!cv.is_same(cv2));
  BOOST_CHECK(!cv2.is_same(cv));
  BOOST_CHECK(!cv.is_same(cv3));
}

BOOST_AUTO_TEST_SUITE_END()
