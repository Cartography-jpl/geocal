#include "unit_test_support.h"
#include "ogr_coordinate.h"
#include "ecr.h"
#include "planet_coordinate.h"
#include <boost/make_shared.hpp>

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
  BOOST_CHECK(c3.utm_zone() == 29);
  // Compare a direct convert of Geodetic to Ecr (which uses different
  // code unrelated to Ogr) to going through UTM. Results should be
  // close to the same.
  Geodetic gp(10,20,100);
  Ecr e1(gp);
  OgrCoordinate c4 = OgrCoordinate::to_utm(gp);
  boost::shared_ptr<CartesianFixed> e2 = c4.convert_to_cf();
  BOOST_CHECK(distance(e1, *e2) < 1);
}

BOOST_AUTO_TEST_CASE(mars_coordinate)
{
  // Test mars coordinates
  std::string mars_wkt="PROJCS[\"SINUSOIDAL MARS\",\
    GEOGCS[\"GCS_MARS\",\
        DATUM[\"D_MARS\",\
            SPHEROID[\"MARS\",3396190,0]],\
        PRIMEM[\"Reference_Meridian\",0],\
        UNIT[\"degree\",0.0174532925199433]],\
    PROJECTION[\"Sinusoidal\"],\
    PARAMETER[\"longitude_of_center\",339.97692848049],\
    PARAMETER[\"false_easting\",0],\
    PARAMETER[\"false_northing\",0],\
    UNIT[\"metre\",1,\
        AUTHORITY[\"EPSG\",\"9001\"]]]";
  boost::shared_ptr<OgrWrapper> mars_sinusoidal(new OgrWrapper(mars_wkt));
  OgrCoordinate c(mars_sinusoidal, 8954.539525824, 1309729.025467, 100.0);
  boost::shared_ptr<CartesianFixed> cf = c.convert_to_cf();
  Planetocentric mf(22.095920859859, -19.860028385975, 100,
		 PlanetConstant::MARS_NAIF_CODE);
  BOOST_CHECK(distance(mf, *cf) < 1.0);
  BOOST_CHECK_CLOSE(c.latitude(), 22.095920859859, 1e-4);
  BOOST_CHECK_CLOSE(c.longitude(), -19.860028385975, 1e-4);
  BOOST_CHECK_CLOSE(c.height_reference_surface(), 100, 1e-4);
  OgrCoordinate c2(mars_sinusoidal, mf);
  BOOST_CHECK_CLOSE(c2.x, 8954.539525824, 1e-4);
  BOOST_CHECK_CLOSE(c2.y, 1309729.025467, 1e-4);
  BOOST_CHECK_CLOSE(c2.z, 100.0, 1e-4);
  OgrCoordinate c3(mars_sinusoidal, *mf.convert_to_cf());
  BOOST_CHECK_CLOSE(c3.x, 8954.539525824, 1e-4);
  BOOST_CHECK_CLOSE(c3.y, 1309729.025467, 1e-4);
  BOOST_CHECK_CLOSE(c3.z, 100.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(mars_coordinate_isis_problem)
{
  // Illustrate a problem we had with SimpleCylindrical handling to
  // agree with ISIS software. The expected results comes from the
  // the ISIS program mappt
  boost::shared_ptr<OgrWrapper> owrap =
    boost::make_shared<OgrWrapper>("PROJCS[\"SimpleCylindrical Mars\",\
    GEOGCS[\"GCS_Mars\",\
        DATUM[\"D_Mars\",\
            SPHEROID[\"Mars\",3396190,0]],\
        PRIMEM[\"Reference_Meridian\",0],\
        UNIT[\"degree\",0.0174532925199433]],\
    PROJECTION[\"Equirectangular\"],\
    PARAMETER[\"latitude_of_origin\",0],\
    PARAMETER[\"central_meridian\",180],\
    PARAMETER[\"standard_parallel_1\",0],\
    PARAMETER[\"false_easting\",0],\
    PARAMETER[\"false_northing\",0]]");
  OgrCoordinate pt(owrap, 9487606.7892057, 1309729.025467, 0);
  Planetocentric pt2(pt);
  BOOST_CHECK_CLOSE(pt2.latitude(), 22.095920859859, 1e-4);
  BOOST_CHECK_CLOSE(pt2.longitude(), -19.938334810139, 1e-4);
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
  boost::shared_ptr<OgrCoordinateConverter> cv4 =
    OgrCoordinateConverter::utm_converter(29);
  cv4->convert_to_coordinate(Geodetic(9, -7), x, y, z);
  BOOST_CHECK_CLOSE(x, 719870.729943312, 1e-6);
  BOOST_CHECK_CLOSE(y, 995452.6722696, 1e-6);
  BOOST_CHECK_CLOSE(z, 0.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(serialize_ogr_wrapper)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<OgrWrapper> ogrw = OgrWrapper::from_epsg(32612);
  BOOST_CHECK_EQUAL(ogrw->projected_cs_type_geo_key(), std::string("32612"));
  BOOST_CHECK_EQUAL(ogrw->pcs_citation_geo_key(), 
		    std::string("WGS 84 / UTM zone 12N"));
  std::string d = serialize_write_string(ogrw);
  if(false)
    std::cerr << d;
  boost::shared_ptr<OgrWrapper> ogrwr = 
    serialize_read_string<OgrWrapper>(d);
  BOOST_CHECK_EQUAL(ogrwr->projected_cs_type_geo_key(), std::string("32612"));
  BOOST_CHECK_EQUAL(ogrwr->pcs_citation_geo_key(), 
		    std::string("WGS 84 / UTM zone 12N"));
}

BOOST_AUTO_TEST_CASE(serialize_ogr_coordinate_converter)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<OgrWrapper> ogrw = OgrWrapper::from_epsg(32612);
  boost::shared_ptr<CoordinateConverter> cv(new OgrCoordinateConverter(ogrw));
  std::string d = serialize_write_string(cv);
  if(false)
    std::cerr << d;
  boost::shared_ptr<CoordinateConverter> cvr = 
    serialize_read_string<CoordinateConverter>(d);
  BOOST_CHECK(cvr->is_same(*cv));
}

BOOST_AUTO_TEST_CASE(serialize_ogr_coordinate)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<OgrCoordinate> c(new OgrCoordinate(OgrCoordinate::to_utm(Geodetic(9, -7))));
  BOOST_CHECK_CLOSE(c->x, 719870.729943312, 1e-6);
  BOOST_CHECK_CLOSE(c->y, 995452.6722696, 1e-6);
  BOOST_CHECK_CLOSE(c->z, 0.0, 1e-6);
  BOOST_CHECK(c->utm_zone() == 29);
  std::string d = serialize_write_string(c);
  if(false)
    std::cerr << d;
  boost::shared_ptr<OgrCoordinate> cr = 
    serialize_read_string<OgrCoordinate>(d);
  BOOST_CHECK_CLOSE(cr->x, 719870.729943312, 1e-6);
  BOOST_CHECK_CLOSE(cr->y, 995452.6722696, 1e-6);
  BOOST_CHECK_CLOSE(cr->z, 0.0, 1e-6);
  BOOST_CHECK(cr->utm_zone() == 29);
}

BOOST_AUTO_TEST_SUITE_END()
